#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER

#include <vector>

class Particle{
    public:
        Particle();    // default (random) constructor

        Particle(float, float, float, std::vector<int>, std::vector<float>, std::vector<float>, std::vector<float>);

        std::vector<float> getPosition();

        float getSize();

        float getMass();

        std::vector<int> getColor();

        std::vector<float> getVelocity();

        std::vector<float> getAcceleration();

        void setAcceleration(std::vector<float>);

        void updateParticlePosition(float);

    private:
        float _size;
        float _mass;
        float _previous_time;
        std::vector<int> _rgb;
        std::vector<float> _position;
        std::vector<float> _velocity;
        std::vector<float> _acceleration;
};

#endif /* !PARTICLE_HEADER */