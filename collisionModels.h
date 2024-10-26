#ifndef COLLISIONMODELS_H
#define COLLISIONMODELS_H

enum class CollisionModels;
/**
 * Defines different methods of modeling collisions between particles
*/

enum class CollisionModels {
    Elastic, 
    Inelastic, 
    NoCollision
};

#endif // COLLISIONMODELS_H