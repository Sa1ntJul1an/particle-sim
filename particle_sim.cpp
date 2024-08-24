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
    const float TOLERANCE = 0.001;

    if (_particles.size() > 1){

        for (Particle* particle1 : _particles) {
            
            float Fx_total = 0.0;
            float Fy_total = 0.0;

            for (Particle* particle2 : _particles) {

                if (particle1 == particle2){    // we do not want to calculate attraction between particles and themselves
                    continue;
                }

                float delta_x = particle1->getPosition()[0] - particle2->getPosition()[0];
                float delta_y = particle1->getPosition()[1] - particle2->getPosition()[1];

                float euclidean_distance;
                float F = 0;
                float theta = 0;
                if (!(abs(delta_y) < TOLERANCE && abs(delta_x) < TOLERANCE)){
                    euclidean_distance = sqrt(pow(delta_x, 2) + pow(delta_y, 2));
                    theta = (180 / pi) * atan2(delta_y, delta_x);
                    F = (_G * particle1->getMass() * particle2->getMass()) / pow(euclidean_distance, 2);
                }
                
                std::cout << "delta_x: " << std::to_string(delta_x) << " delta_y: " << std::to_string(delta_y) << std::endl;
                std::cout << "theta: " << std::to_string(theta) << std::endl;

                Fx_total += F * (180 / pi) * cos(theta);
                Fy_total += F * (180 / pi) * sin(theta);
            }

            float a_x = Fx_total / particle1->getMass();
            float a_y = Fy_total / particle1->getMass();

            std::vector<float> acceleration = {a_x, a_y};

            particle1->setAcceleration(acceleration);
        }
    }

    for (int i = 0; i < _particles.size(); i++){
        _particles[i]->updateParticlePosition(current_time);
    }

}

ParticleSim::~ParticleSim(){
    for (Particle* particle : _particles) {
        delete particle;
    }
}