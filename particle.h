#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER

#include <vector>

class Particle{
    public:
        Particle();    // default (random) constructor

        Particle(float, float, float, std::vector<int>, std::vector<float>, std::vector<float>, std::vector<float>);

        std::vector<float> getPosition();

        float getRadius();

        float getMass();

        std::vector<int> getColor();

        std::vector<float> getVelocity();

        std::vector<float> getAcceleration();

        void setAcceleration(std::vector<float>);

        void updateParticle(float);

    private:
        float _radius;
        float _mass;
        float _previous_time;
        std::vector<int> _rgb;
        std::vector<float> _position;
        std::vector<float> _velocity;
        std::vector<float> _acceleration;

        float _tolerance;
};

#endif /* !PARTICLE_HEADER */