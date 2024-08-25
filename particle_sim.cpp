#include "particle_sim.h"
#include <cmath>
#include <vector>
#include <iostream>

ParticleSim::ParticleSim(float G, float coefficientOfFriction, std::vector<Particle*> particles){
    _G = G;
    _coefficientOfFriction = coefficientOfFriction;
    _particles = particles;
}

void ParticleSim::addParticle(Particle* particle){
    _particles.push_back(particle);
}

void ParticleSim::updateParticles(float current_time) {
    double pi = M_PI;
    const float TOLERANCE = 0.01;

    if (_particles.size() > 1){

        for (Particle* particle1 : _particles) {

            //particle1->setAcceleration({0.0, 0.0});
            
            float Fx_total = 0.0;
            float Fy_total = 0.0;

            std::cout << "============================================" << std::endl;

            for (Particle* particle2 : _particles) {

                if (particle1 == particle2){    // we do not want to calculate attraction between particles and themselves
                    continue;
                }

                float delta_x = particle2->getPosition()[0] - particle1->getPosition()[0];
                float delta_y = particle2->getPosition()[1] - particle1->getPosition()[1];

                float euclidean_distance = sqrt(pow(delta_x, 2) + pow(delta_y, 2));

                float F = 0;
                float theta = 0;
                if (euclidean_distance > TOLERANCE){
                    theta = (180 / pi) * atan2(delta_y, delta_x);
                    F = (_G * particle1->getMass() * particle2->getMass()) / pow(euclidean_distance, 2);
                }

                Fx_total += F * (180 / pi) * cos(theta);
                Fy_total += F * (180 / pi) * sin(theta);

                std::cout << "particle 1: " << particle1 << std::endl;
                std::cout << "position: " << "(" << std::to_string(particle1->getPosition()[0]) << ", " << std::to_string(particle1->getPosition()[1]) << ")" << std::endl;
                std::cout << "velocity: " << "(" << std::to_string(particle1->getVelocity()[0]) << ", " << std::to_string(particle1->getVelocity()[1]) << ")" << std::endl;
                std::cout << "acceleration: " << "(" << std::to_string(particle1->getAcceleration()[0]) << ", " << std::to_string(particle1->getAcceleration()[1]) << ")" << std::endl << std::endl;

                std::cout << "particle 2: " << particle2 << std::endl;
                std::cout << "position: " << "(" << std::to_string(particle2->getPosition()[0]) << ", " << std::to_string(particle2->getPosition()[1]) << ")" << std::endl;

                std::cout << "deltas: " << std::to_string(delta_x) << " " << std::to_string(delta_y) << std::endl;
                std::cout << "euclidean distance: " << std::to_string(euclidean_distance) << std::endl;

                std::cout << "Theta: " << std::to_string(theta) << std::endl;
                std::cout << "F: " << std::to_string(F) << std::endl;

                std::cout << "Fx_total: " << std::to_string(Fx_total) << std::endl;
                std::cout << "Fy_total: " << std::to_string(Fy_total) << std::endl;

                std::cout << "============================================" << std::endl << std::endl;
            }

            float a_x = Fx_total / particle1->getMass();
            float a_y = Fy_total / particle1->getMass();

            std::vector<float> acceleration = {a_x, a_y};

            particle1->setAcceleration(acceleration);
        }
    } else if (_particles.size() == 1){
        _particles[0]->setAcceleration({0.0, 0.0});
    }

    for (int i = 0; i < _particles.size(); i++){
        _particles[i]->updateParticle(current_time);
    }

}

ParticleSim::~ParticleSim(){
    for (Particle* particle : _particles) {
        delete particle;
    }
}