#ifndef PARTICLE_SIM_HEADER
#define PARTICLE_SIM_HEADER

#include "particle.h"
#include <vector>
#include <algorithm>

class ParticleSim {
    public: 
        ParticleSim(float G, float viscosityOfMedmium, std::vector<Particle*> particles, int width, int height, bool collideWithWalls, bool collideWithParticles, bool isFrictionEnabled);

        ~ParticleSim();

        void addParticle(Particle*);

        void updateParticles(float);

        void reset();

    private:
        float _G;
        float _viscosityOfMedmium;

        int _width;
        int _height;

        bool _collideWithWalls;
        bool _collideWithParticles;
        bool _isFrictionEnabled;

        std::vector<Particle*> _particles;

        float _delta_x(const Particle*, const Particle*);
        float _delta_y(const Particle*, const Particle*);
        float _euclideanDistance(const Particle*, const Particle*);
        float _theta(const Particle*, const Particle*);
        float _xComponent(const float, const float);
        float _yComponent(const float, const float);
        void _elasticCollision(Particle*, Particle*);

        // custom pair comparator:
        struct _PointerPairComparator {
            bool operator()(const std::pair<Particle*, Particle*>& a, const std::pair<Particle*, Particle*>& b) const {
                // Canonicalize the pairs by sorting the pointers
                auto canonical_a = std::minmax(a.first, a.second);
                auto canonical_b = std::minmax(b.first, b.second);
                // Compare the canonicalized pairs  
                return canonical_a < canonical_b;
            }
        };
};


#endif /* !PARTICLE_SIM_HEADER */