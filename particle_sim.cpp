#include "particle_sim.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <set>

ParticleSim::ParticleSim(float G, float coefficientOfFriction, std::vector<Particle*> particles, int width, int height, bool collideWithWalls, bool collideWithParticles){
    _G = G;
    _coefficientOfFriction = coefficientOfFriction;
    _particles = particles;

    _width = width;
    _height = height;

    _collideWithWalls = collideWithWalls;
    _collideWithParticles = collideWithParticles;
}

void ParticleSim::addParticle(Particle* particle) {
    _particles.push_back(particle);
}

void ParticleSim::elasticCollision(Particle* particle1, Particle* particle2) {
    std::vector<float> particle1_pos, particle2_pos, particle1_vel, particle2_vel;


}

void ParticleSim::updateParticles(float current_time) {
    double pi = M_PI;

    std::set<std::pair<Particle*, Particle*>> collidingPairs;

    if (_particles.size() > 1){

        for (Particle* particle1 : _particles) {
          
            float Fx_total = 0.0;
            float Fy_total = 0.0;

            for (Particle* particle2 : _particles) {

                if (particle1 == particle2){    // we do not want to calculate attraction between particles and themselves
                    continue;
                }

                float euclidean_distance = _euclideanDistance(particle1, particle2);

                float F = 0;

                float delta_x = _delta_x(particle1, particle2);
                float delta_y = _delta_y(particle1, particle2);

                float theta = atan2(delta_y, delta_x);
                if (euclidean_distance > (particle1->getRadius() + particle2->getRadius())){        // only calculate force of attraction if particles are not touching
                    F = (_G * particle1->getMass() * particle2->getMass()) / pow(euclidean_distance, 2);
                } else {    // collision
                    std::pair<Particle*, Particle*> collidingPair = {particle1, particle2};
                    collidingPairs.insert(collidingPair);
                    continue;
                }
                // TODO : custom pair comparator:
                // struct PairComparator {
                //     bool operator()(const std::pair<Particle*, Particle*>& p1, const std::pair<Particle*, Particle*>& p2) const {
                //         return p1.first < p2.first || (p1.first == p2.first && p1.second < p2.second);
                //     }
                // };

                // std::set<std::pair<Particle*, Particle*>, PairComparator> collidingPairs;

                Fx_total += _xComponent(F, theta);
                Fy_total += _yComponent(F, theta);
            }

            float a_x = Fx_total / particle1->getMass();
            float a_y = Fy_total / particle1->getMass();

            std::vector<float> acceleration = {a_x, a_y};

            particle1->setAcceleration(acceleration);

            // check for collisions with walls
            if (_collideWithWalls){
                if (particle1->getPosition()[0] <= 0 || particle1->getPosition()[0] >= _width){                      // left or right wall collision
                    particle1->negateXVelocity();
                }

                if (particle1->getPosition()[1] <= 0 || particle1->getPosition()[1] >= _height) {                     // top or bottom wall collision
                    particle1->negateYVelocity();
                }
            }
    }
    } else if (_particles.size() == 1){
        _particles[0]->setAcceleration({0.0, 0.0});
    }

    for (int i = 0; i < _particles.size(); i++){
        _particles[i]->updateParticle(current_time);
    }
}

float ParticleSim::_delta_x(const Particle* particle1, const Particle* particle2) {
    return particle2->getPosition()[0] - particle1->getPosition()[0];
}

float ParticleSim::_delta_y(const Particle* particle1, const Particle* particle2) {
    return particle2->getPosition()[1] - particle1->getPosition()[1];
}

float ParticleSim::_euclideanDistance(const Particle* particle1, const Particle* particle2) {
    return sqrt(pow(_delta_x(particle1, particle2), 2) + pow(_delta_y(particle1, particle2), 2));
}

float ParticleSim::_xComponent(const float value, const float thetaRad) {
    return value * float(cos(thetaRad));
}

float ParticleSim::_yComponent(const float value, const float thetaRad) {
    return value * float(sin(thetaRad));
}

ParticleSim::~ParticleSim(){
    for (Particle* particle : _particles) {
        delete particle;
    }
}