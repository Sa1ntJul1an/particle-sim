#include "particle_sim.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <set>

ParticleSim::ParticleSim(float G, float viscosityOfMedium, CollisionModels collisionModel, std::vector<Particle*> particles, int width, int height, bool collideWithWalls, bool isFrictionEnabled){
    _G = G;
    _viscosityOfMedmium = viscosityOfMedium;
    _particles = particles;

    _width = width;
    _height = height;

    _collideWithWalls = collideWithWalls;
    _isFrictionEnabled = isFrictionEnabled;

    _collisionModel = collisionModel;
}

void ParticleSim::addParticle(Particle* particle) {
    _particles.push_back(particle);
}

void ParticleSim::_elasticCollision(Particle* particle1, Particle* particle2) {
    float particle1_mass = particle1->getMass();
    float particle2_mass = particle2->getMass();

    float vel1_ix, vel1_iy, vel2_ix, vel2_iy;
    vel1_ix = particle1->getVelocity()[0];
    vel1_iy = particle1->getVelocity()[1];
    vel2_ix = particle2->getVelocity()[0];
    vel2_iy = particle2->getVelocity()[1];

    float vel1_fx, vel1_fy, vel2_fx, vel2_fy;

    vel2_fx = (2 * particle1_mass * vel1_ix + vel2_ix * (particle2_mass - particle1_mass)) / (particle2_mass + particle1_mass);
    vel2_fy = (2 * particle1_mass * vel1_iy + vel2_iy * (particle2_mass - particle1_mass)) / (particle2_mass + particle1_mass);

    vel1_fx = vel2_ix + vel2_fx - vel1_ix;
    vel1_fy = vel2_iy + vel2_fy - vel1_iy;

    std::vector<float> vel1_f, vel2_f; 
    vel1_f = {vel1_fx, vel1_fy};
    vel2_f = {vel2_fx, vel2_fy};

    particle1->setVelocity(vel1_f);
    particle2->setVelocity(vel2_f);
}

void ParticleSim::_inelasticCollision(Particle * particle1, Particle * particle2) {
    float particle1_mass, particle2_mass, vel1_ix, vel2_ix, vel1_iy, vel2_iy, vel_fx, vel_fy;

    particle1_mass = particle1->getMass();
    particle2_mass = particle2->getMass();

    vel1_ix = particle1->getVelocity()[0];
    vel1_iy = particle1->getVelocity()[1];

    vel2_ix = particle2->getVelocity()[0];
    vel2_iy = particle2->getVelocity()[1];

    vel_fx = (particle1_mass * vel1_ix + particle2_mass * vel2_ix) / (particle1_mass + particle2_mass);
    vel_fy = (particle1_mass * vel1_iy + particle2_mass * vel2_iy) / (particle1_mass + particle2_mass);

    particle1->setRadius(particle1->getRadius() + particle2->getRadius());
    particle1->setMass(particle1->getMass() + particle2->getMass());
    particle1->setVelocity({vel_fx, vel_fy});

    particle1->setIsColliding(false);

    _particles.erase(std::find(_particles.begin(), _particles.end(), particle2));
}

void ParticleSim::updateParticles(float current_time) {
    double pi = M_PI;

    std::set<std::pair<Particle*, Particle*>, _PointerPairComparator> collidingPairs;

    for (Particle* particle1 : _particles) {

        float Fx_total = 0.0;
        float Fy_total = 0.0;

        for (Particle* particle2 : _particles) {

            if (particle1 == particle2){    // we do not want to calculate attraction between particles and themselves
                continue;
            }

            float euclidean_distance = _euclideanDistance(particle1, particle2);

            float F = 0;

            if (euclidean_distance > (particle1->getRadius() + particle2->getRadius())){        // only calculate force of attraction if particles are not touching
                F = (_G * particle1->getMass() * particle2->getMass()) / pow(euclidean_distance, 2);

                if (particle1->getIsColliding() && particle2->getIsColliding()) {
                    particle1->setIsColliding(false);
                    particle2->setIsColliding(false);
                }
            } else {    // collision
                 if (!particle1->getIsColliding() && !particle2->getIsColliding()) {
                    std::pair<Particle*, Particle*> collidingPair = {particle1, particle2};
                    collidingPairs.insert(collidingPair);
                }
                particle1->setIsColliding(true);
                particle2->setIsColliding(true);

                continue;
            }

            Fx_total += _xComponent(F,  _theta(particle1, particle2));
            Fy_total += _yComponent(F,  _theta(particle1, particle2));
        }

        float friction_force_x = 0.0;
        float friction_force_y = 0.0;
        if (_isFrictionEnabled) {
            // stokes law for force of drag on a sphere
            friction_force_x = 6 * M_PI * particle1->getRadius() * _viscosityOfMedmium * particle1->getVelocity()[0];
            friction_force_y = 6 * M_PI * particle1->getRadius() * _viscosityOfMedmium * particle1->getVelocity()[1];
        }

        float a_x = (Fx_total - friction_force_x) / particle1->getMass();
        float a_y = (Fy_total - friction_force_y) / particle1->getMass();

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

    for (std::pair<Particle*, Particle*> collidingPair : collidingPairs){
        switch (_collisionModel) {
            case CollisionModels::Elastic:
                _elasticCollision(collidingPair.first, collidingPair.second);
                break;

            case CollisionModels::Inelastic:
                _inelasticCollision(collidingPair.first, collidingPair.second);
                break;
            
            default:
                break;
            }
    }

    for (int i = 0; i < _particles.size(); i++){
        _particles[i]->updateParticle(current_time);
    }
}

std::vector<Particle*> ParticleSim::getParticles() const {
    return this->_particles;
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

float ParticleSim::_theta(const Particle* particle1, const Particle* particle2) {
    return atan2(_delta_y(particle1, particle2), _delta_x(particle1, particle2));
}

float ParticleSim::_xComponent(const float value, const float thetaRad) {
    return value * float(cos(thetaRad));
}

float ParticleSim::_yComponent(const float value, const float thetaRad) {
    return value * float(sin(thetaRad));
}

void ParticleSim::reset() {
    _particles.clear();
}

ParticleSim::~ParticleSim(){
    for (Particle* particle : _particles) {
        delete particle;
    }
}