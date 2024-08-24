#ifndef PARTICLE_SIM_HEADER
#define PARTICLE_SIM_HEADER

#include "particle.h"
#include <vector>

class ParticleSim {
    public: 
        ParticleSim(float, float, std::vector<Particle*>);

        ~ParticleSim();

        void addParticle(Particle*);

        void updateParticles(float);

    private:
        float _G;
        float _coefficientOfFriction;

        std::vector<Particle*> _particles;

};


#endif /* !PARTICLE_SIM_HEADER */