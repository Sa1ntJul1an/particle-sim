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

    if (_particles.size() > 1){

        for (Particle* particle1 : _particles) {

            //particle1->setAcceleration({0.0, 0.0});
            
            float Fx_total = 0.0;
            float Fy_total = 0.0;

            for (Particle* particle2 : _particles) {

                if (particle1 == particle2){    // we do not want to calculate attraction between particles and themselves
                    continue;
                }

                float delta_x = particle2->getPosition()[0] - particle1->getPosition()[0];
                float delta_y = particle2->getPosition()[1] - particle1->getPosition()[1];

                float euclidean_distance = sqrt(pow(delta_x, 2) + pow(delta_y, 2));

                float F = 0;
                float theta = 0;
                if (euclidean_distance > (particle1->getRadius() + particle2->getRadius())){        // only calculate force of attraction if particles are not touching
                    theta = atan2(delta_y, delta_x);
                    F = (_G * particle1->getMass() * particle2->getMass()) / pow(euclidean_distance, 2);
                }

                Fx_total += F * cos(theta);
                Fy_total += F * sin(theta);
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