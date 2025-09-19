//
//  interact.h
//  New Test Project
//
//  Created by Calvin Richards on 8/30/25.
//
#pragma once
#include "world.h"
#include "player.h"


struct RaycastHit {
    glm::ivec3 blockPos;    // World coordinates of the hit block
    Face face;              // Which face was hit
    bool hit;               // Whether anything was hit
    glm::vec2 chunkCoords;  // Which chunk contains the hit block
};

struct AABB_Directions {
    // an AABB with min 0 and max 0 will be ignored
    AABB up;
    AABB down;
    AABB left;
    AABB right;
    AABB front;
    AABB back;
    // none of these have to be modified, only set
};

RaycastHit raycast(const glm::vec3 &origin, const glm::vec3 &dir, const Chunk chunk, float maxDistance);

AABB_Directions * getCollisionDirections(const Player& p, const World& w); // returning a pointer for memory bandwidth
// takes in the player and the world
// make a highlight function to test this

void aabb_collide(keyInputs& k, World& w, Player& p);

glm::vec3 worldInteract(Player& p, World& w, keyInputs keys, float delta);

