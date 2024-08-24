#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>

#include "particle.h"
#include "particle_sim.h"

using namespace sf;
using namespace std;

const int WIDTH = 1000;
const int HEIGHT = 800;

const int MENU_WIDTH = 400;
const int MENU_HEIGHT = 800;

mt19937 mt(time(nullptr));

struct testParticle{
    float size = 4.0;
    float mass = 50000000000000.0;
    float previous_time = 0.0;
    vector<int> rgb = {255, 0, 0};
    vector<float> position = {int(WIDTH / 2), int(HEIGHT / 2)};
    vector<float> velocity = {0.0, 0.0};
    vector<float> acceleration = {0.0, 0.0};
};


/* IDEAS

    on collision with wall, reverse velocity in direction perpendicular to wall

    for collision with other particles, we need to do some math 

    use displayInfo bool to determine if trajectory vector shown + current vel and acc

*/

int main(){

    bool displayInfo = true; 

    float G = 0.0000000000667;
    float coefficientOfFriction = 0;

    vector<Particle> particles;


    vector<float> mousePosition;
    bool particleSpawned = false;

    testParticle particle_struct;

    // RENDER WINDOWS 
    // =======================================================================
    RenderWindow particleWindow(VideoMode(WIDTH, HEIGHT), "Particle Sim");
    particleWindow.setFramerateLimit(60);
    RenderWindow menuWindow(VideoMode(MENU_WIDTH, MENU_HEIGHT), "Menu");
    menuWindow.setFramerateLimit(60);
    // =======================================================================

    Clock renderTime;

    vector<CircleShape> particle_shapes;

    ParticleSim particleSim(G, coefficientOfFriction, particles);

    while(particleWindow.isOpen() && menuWindow.isOpen()){

        Time time = renderTime.getElapsedTime();
        float time_seconds = time.asSeconds();

        mousePosition = {float(Mouse::getPosition(particleWindow).x), float(Mouse::getPosition(particleWindow).y)};

        if (Mouse::isButtonPressed(Mouse::Left) && !particleSpawned){
            
            Particle particle(particle_struct.size, particle_struct.mass, time_seconds, particle_struct.rgb, mousePosition, particle_struct.velocity, particle_struct.acceleration);
            particles.push_back(particle);

            CircleShape shape(particle.getSize());
            shape.setFillColor(Color(particle.getColor()[0], particle.getColor()[1], particle.getColor()[2]));
            shape.setPosition(Vector2f(particle.getPosition()[0], particle.getPosition()[1]));

            particle_shapes.push_back(shape);

            particleSpawned = true;

        } else if (!Mouse::isButtonPressed(Mouse::Left))
        {
            particleSpawned = false;
        }


        // CLOSE WINDOWS IF X PRESSED
        // ==========================================================
        Event particleWindowEvent;
        Event menuWindowEvent;

        while(particleWindow.pollEvent(particleWindowEvent)){
            
            if(particleWindowEvent.type == Event::Closed){
                particleWindow.close();
                menuWindow.close();
            }
        }

        while(menuWindow.pollEvent(menuWindowEvent)){
            
            if(menuWindowEvent.type == Event::Closed){
                particleWindow.close();
                menuWindow.close();
            }
        }
        // ==========================================================

        // UPDATE PARTICLES AND CHANGE SHAPE POSITIONS TO MATCH
        // ==========================================================

        particleSim.updateParticles(time_seconds);
        for (int i = 0; i < particles.size(); i ++){
            Particle particle = particles[i];

            //cout << "velocity: " << to_string(particle.getVelocity()[0]) << " " << to_string(particle.getVelocity()[1]) << endl;

            particle_shapes[i].setPosition(Vector2f(particle.getPosition()[0], particle.getPosition()[1]));
        }

        // ==========================================================


        // DRAW ALL PARTICLES 
        // ==========================================================
        particleWindow.clear();

        for (int i = 0; i < particle_shapes.size(); i++){
            particleWindow.draw(particle_shapes[i]);
        }
        particleWindow.display();
        // ==========================================================


        menuWindow.clear();
        menuWindow.display();
    }

    return 0;
}