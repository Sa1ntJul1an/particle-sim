#ifndef PARTICLE_HEADER
#define PARTICLE_HEADER

#include <vector>

class Particle{
    public:
        Particle();    // default (random) constructor

        Particle(float, float, float, std::vector<int>, std::vector<float>, std::vector<float>, std::vector<float>);

        Particle(Particle&);

        std::vector<float> getPosition();

        void setPosition(std::vector<float>);

        float getRadius();

        float getMass();

        float getPreviousTime();

        void setPreviousTime(float);
        
        std::vector<int> getColor();

        std::vector<float> getVelocity();

        void setVelocity(std::vector<float>);

        void negateXVelocity();

        void negateYVelocity();

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
};

#endif /* !PARTICLE_HEADER */