#ifndef CONFIGURATION_MENU_H
#define CONFIGURATION_MENU_H

#include "particle_sim.h"


class ConfigurationMenu {
    public:
        ConfigurationMenu();

        void addTrackbar(void(*)(double));
        void addToggle(void(*)(bool));

    private:
        ParticleSim * particleSim;

        void _evaluateMenu();

};


#endif // CONFIGURATION_MENU_H