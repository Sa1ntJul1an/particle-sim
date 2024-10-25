#ifndef PARTICLE_SIM_HEADER
#define PARTICLE_SIM_HEADER

#include "particle.h"
#include <vector>

class ParticleSim {
    public: 
        ParticleSim(float G, float coefficientOfFriction, std::vector<Particle*> particles, int width, int height, bool collideWithWalls, bool collideWithParticles);

        ~ParticleSim();

        void addParticle(Particle*);

        void elasticCollision(Particle*, Particle*);

        void updateParticles(float);

    private:
        float _G;
        float _coefficientOfFriction;

        int _width;
        int _height;

        bool _collideWithWalls;
        bool _collideWithParticles;

        std::vector<Particle*> _particles;

        float _delta_x(const Particle*, const Particle*);
        float _delta_y(const Particle*, const Particle*);
        float _euclideanDistance(const Particle*, const Particle*);
        float _xComponent(const float, const float);
        float _yComponent(const float, const float);
};


#endif /* !PARTICLE_SIM_HEADER */