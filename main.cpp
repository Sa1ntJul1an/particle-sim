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

const Color velVectorColor = Color(0, 255, 0);
const Color accVectorColor = Color(0, 255, 255);


mt19937 mt(time(nullptr));

struct testParticle{
    float radius = 4.0;
    float mass = 5000000000000.0;
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

    vector<Particle*> particles;

    bool drawVelocityVectors = true;
    bool drawAccelerationVectors = true;

    vector<float> mousePosition;
    bool spawningParticle = false;

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

        if (Mouse::isButtonPressed(Mouse::Left) && !spawningParticle){
            
            Particle* particle = new Particle(particle_struct.radius, particle_struct.mass, time_seconds, particle_struct.rgb, mousePosition, particle_struct.velocity, particle_struct.acceleration);
            
            particles.push_back(particle);
            particleSim.addParticle(particle);

            CircleShape shape(particle->getRadius());
            shape.setFillColor(Color(particle->getColor()[0], particle->getColor()[1], particle->getColor()[2]));
            shape.setPosition(Vector2f(particle->getPosition()[0], particle->getPosition()[1]));

            particle_shapes.push_back(shape);

            spawningParticle = true;

        } else if (!Mouse::isButtonPressed(Mouse::Left))
        {
            spawningParticle = false;
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

        // UPDATE PARTICLES AND DISPLAY
        // ==========================================================

        particleSim.updateParticles(time_seconds);

        particleWindow.clear();

        for (int i = 0; i < particles.size(); i ++){
            Particle* particle = particles[i];

            Vector2f particle_position = Vector2f(particle->getPosition()[0] - particle->getRadius(), particle->getPosition()[1] - particle->getRadius());

            particle_shapes[i].setPosition(particle_position);


            if (drawVelocityVectors){
                
                Vector2f velocity_vector;

                velocity_vector.x = particle_position.x + particle->getVelocity()[0] * 60;
                velocity_vector.y = particle_position.y + particle->getVelocity()[1] * 60;

                Vertex line[] =
                {
                    Vertex(particle_position, velVectorColor),
                    Vertex(velocity_vector, velVectorColor)
                };

                particleWindow.draw(line, 2, sf::Lines);
            }
            
            if (drawAccelerationVectors){
                
                Vector2f acceleration_vector;

                acceleration_vector.x = particle_position.x + particle->getAcceleration()[0] * 60;
                acceleration_vector.y = particle_position.y + particle->getAcceleration()[1] * 60;

                Vertex line[] =
                {
                    Vertex(particle_position, accVectorColor),
                    Vertex(acceleration_vector, accVectorColor)
                };

                particleWindow.draw(line, 2, sf::Lines);
            }

            particleWindow.draw(particle_shapes[i]);
        }

        particleWindow.display();
        // ==========================================================


        menuWindow.clear();
        menuWindow.display();
    }

    return 0;
}