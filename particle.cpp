#include "particle.h"
#include <random>
#include <ctime>
#include <cmath>

#include <iostream>

//std::mt19937 mt(std::time(nullptr));

Particle::Particle() {
    _radius = 3;
    _mass = _radius * 100000000;

    _previous_time = 0;

    _rgb = {255, 255, 255};

    _position = {100, 100};
    _velocity = {0.0, 0.0};
    _acceleration = {0.0, 0.0};

    _tolerance = 0.001;
}

Particle::Particle (float radius, float mass, float previous_time, std::vector<int> rgb, std::vector<float> position, std::vector<float> velocity, std::vector<float> acceleration){
    _radius = radius;
    _mass = mass;

    _previous_time = previous_time;

    _rgb = rgb;

    _position = position;
    _velocity = velocity;
    _acceleration = acceleration;

    _tolerance = 0.001;
}

std::vector<float> Particle::getPosition () {
    return _position;
}

float Particle::getRadius() {
    return _radius;
}

float Particle::getMass() {
    return _mass;
}

std::vector<int> Particle::getColor() {
    return _rgb;
}

std::vector<float> Particle::getVelocity() {
    return _velocity;
}

std::vector<float> Particle::getAcceleration() {
    return _acceleration;
}

void Particle::setAcceleration(std::vector<float> acc){
    _acceleration = acc;
}

void Particle::updateParticle(float current_time) {

    float delta_position_x;
    float delta_position_y;

    float elapsed_time = current_time - _previous_time;

    _velocity[0] += _acceleration[0] * elapsed_time;
    _velocity[1] += _acceleration[1] * elapsed_time;

    delta_position_x = _acceleration[0] * elapsed_time + _velocity[0];
    delta_position_y = _acceleration[1] * elapsed_time + _velocity[1];

    _position[0] += delta_position_x;
    _position[1] += delta_position_y;

    _previous_time = current_time;

}