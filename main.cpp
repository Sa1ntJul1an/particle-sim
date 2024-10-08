#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <sstream>

#include "particle.h"
#include "particle_sim.h"

using namespace sf;
using namespace std;

const int WIDTH = 1000;
const int HEIGHT = 1000;

const int MENU_WIDTH = 400;
const int MENU_HEIGHT = 800;

const Color posVectorColor = Color(255, 255, 0);
const Color velVectorColor = Color(255, 0, 255);
const Color accVectorColor = Color(0, 255, 255);


mt19937 mt(time(nullptr));

struct testParticle{
    float radius = 4.0;
    float mass = 50000000000000.0;
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

// convert coordinate from between window and sim coordinates (invert y axis)
vector<float> convertCoords(vector<float> coords){
    return {coords[0], HEIGHT - coords[1]};
}

Vector2f convertCoords(Vector2f coords){
    return {coords.x, HEIGHT - coords.y};
}


int main(){

    float G = 0.0000000000667;
    float coefficientOfFriction = 0;

    vector<Particle*> particles;

    bool drawVelocityVectors = true;
    bool drawAccelerationVectors = true;

    bool displayValues = true;

    bool collideWithWalls = true;
    bool collideWithParticles = true;

    vector<float> mousePosition = {-10.0, -10.0};
    bool spawningParticle = false;

    testParticle particle_struct;

    Font font;
    FileInputStream fontIn;
    fontIn.open("slkscr.ttf");
    font.loadFromStream(fontIn);

    // RENDER WINDOWS 
    // =======================================================================
    RenderWindow particleWindow(VideoMode(WIDTH, HEIGHT), "Particle Sim");
    particleWindow.setFramerateLimit(60);
    RenderWindow menuWindow(VideoMode(MENU_WIDTH, MENU_HEIGHT), "Menu");
    menuWindow.setFramerateLimit(60);
    // =======================================================================

    bool sim_running = true;
    bool space_pressed = false;

    Text pausedIndicator; 
    pausedIndicator.setFont(font);
    pausedIndicator.setFillColor(Color::Red);
    pausedIndicator.setPosition(Vector2f(5, 5));
    pausedIndicator.setCharacterSize(30);
    pausedIndicator.setString("Paused.");

    Clock renderTime;

    vector<CircleShape> particle_shapes;

    ParticleSim particleSim(G, coefficientOfFriction, particles, WIDTH, HEIGHT, collideWithWalls, collideWithParticles);

    // spawn a particle with a fixed position at first, until mouse released then unfix position 
    Particle particle = Particle(particle_struct.radius, particle_struct.mass, 0, particle_struct.rgb, convertCoords(mousePosition), particle_struct.velocity, particle_struct.acceleration);

    Particle * particle_pointer = nullptr;

    while(particleWindow.isOpen() && menuWindow.isOpen()){

        Time time = renderTime.getElapsedTime();
        float time_seconds = time.asSeconds();

        // subtract height from y coord to invert y axis (bottom left of render window is now (0,0))
        mousePosition = {float(Mouse::getPosition(particleWindow).x), float(Mouse::getPosition(particleWindow).y)};

        if (Mouse::isButtonPressed(Mouse::Left) && !spawningParticle && mousePosition[0] < WIDTH && mousePosition[0] >= 0 && mousePosition[1] < HEIGHT && mousePosition[1] >= 0){
            spawningParticle = true;
            
            particle_pointer = new Particle(particle);

            particles.push_back(particle_pointer);
            particleSim.addParticle(particle_pointer);

            particle_pointer->setPosition(convertCoords(mousePosition));
        } else if (spawningParticle  && Mouse::isButtonPressed(Mouse::Left)) {
            particle_pointer->setPosition(convertCoords(mousePosition));
            particle_pointer->setVelocity({0.0, 0.0});
        } else if (spawningParticle && !Mouse::isButtonPressed(Mouse::Left))
        {
            particle_pointer->setPreviousTime(time_seconds);

            particle_pointer = nullptr;

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

        // KEYBOARD EVENTS =========================================
        if (Keyboard::isKeyPressed(Keyboard::Space)){   // space to pause/unpause

            if (!space_pressed) {
                sim_running = !sim_running;
                space_pressed = true;
            }
        } else {
            space_pressed = false;
        }

        particleWindow.clear();

        if (sim_running) {
            particleSim.updateParticles(time_seconds);
        } else {
            particleWindow.draw(pausedIndicator);
        }


        CircleShape particle_shape;
        for (int i = 0; i < particles.size(); i ++){
            Particle* particle = particles[i];

            Vector2f particle_pos_sim = Vector2f(particle->getPosition()[0], particle->getPosition()[1]);
            Vector2f particle_pos_window = convertCoords(particle_pos_sim);

            particle_pos_window = Vector2f(particle_pos_window.x - particle->getRadius(), particle_pos_window.y - particle->getRadius());

            particle_shape.setRadius(particle->getRadius());
            particle_shape.setFillColor(Color(particle->getColor()[0], particle->getColor()[1], particle->getColor()[2]));
            particle_shape.setPosition(particle_pos_window);

            particleWindow.draw(particle_shape);

            if (displayValues){
                int char_size = 15;
                int x_offset = 70;

                Text position_text;
                position_text.setFillColor(posVectorColor);
                position_text.setPosition(Vector2f(particle_pos_window.x - x_offset, particle_pos_window.y + particle->getRadius()));
                position_text.setCharacterSize(char_size);
                position_text.setFont(font);

                stringstream position_stream;
                position_stream.precision(2);
                position_stream << fixed << "pos: (" <<  particle->getPosition()[0] << ", " <<  particle->getPosition()[1] << ")";

                position_text.setString(position_stream.str());

                particleWindow.draw(position_text);

                Text velocity_text;
                velocity_text.setFillColor(velVectorColor);
                velocity_text.setPosition(Vector2f(particle_pos_window.x - x_offset, particle_pos_window.y + particle->getRadius() + char_size));
                velocity_text.setCharacterSize(char_size);
                velocity_text.setFont(font);

                stringstream velocity_stream;
                velocity_stream.precision(2);
                velocity_stream << fixed << "vel: (" <<  particle->getVelocity()[0] << ", " <<  particle->getVelocity()[1] << ")";

                velocity_text.setString(velocity_stream.str());

                particleWindow.draw(velocity_text);

                Text acceleration_text;
                acceleration_text.setFillColor(accVectorColor);
                acceleration_text.setPosition(Vector2f(particle_pos_window.x - x_offset, particle_pos_window.y + particle->getRadius() + char_size * 2));
                acceleration_text.setCharacterSize(15);
                acceleration_text.setFont(font);

                stringstream acceleration_stream;
                acceleration_stream.precision(2);
                acceleration_stream << fixed << "acc: (" << particle->getAcceleration()[0] << ", " << particle->getAcceleration()[1] << ")";

                acceleration_text.setString(acceleration_stream.str());

                particleWindow.draw(acceleration_text);
            }

            if (drawVelocityVectors){
                
                Vector2f velocity_vector;

                velocity_vector.x = particle_pos_sim.x + particle->getVelocity()[0] * 60;
                velocity_vector.y = particle_pos_sim.y + particle->getVelocity()[1] * 60;

                velocity_vector = convertCoords(velocity_vector);                
                
                Vertex line[] =
                {
                    Vertex(convertCoords(particle_pos_sim), velVectorColor),
                    Vertex(velocity_vector, velVectorColor)
                };

                particleWindow.draw(line, 2, sf::Lines);

            }
            
            if (drawAccelerationVectors){
                
                Vector2f acceleration_vector;

                acceleration_vector.x = particle_pos_sim.x + particle->getAcceleration()[0] * 60;
                acceleration_vector.y = particle_pos_sim.y + particle->getAcceleration()[1] * 60;

                acceleration_vector = convertCoords(acceleration_vector);

                Vertex line[] =
                {
                    Vertex(convertCoords(particle_pos_sim), accVectorColor),
                    Vertex(acceleration_vector, accVectorColor)
                };

                particleWindow.draw(line, 2, sf::Lines);
            }
        }

        particleWindow.display();
        // ==========================================================

        menuWindow.clear();
        menuWindow.display();
    }

    return 0;
}