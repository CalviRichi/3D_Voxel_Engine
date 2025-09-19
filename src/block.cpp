//
//  block.cpp
//  New Test Project
//
//  Created by Calvin Richards on 8/28/25.
//

#include "block.h"
#include <array>

UVRect Block::getTileUV(Type t, Face face, bool top) {
    int tx, ty;
    switch (t) {
            
        case GRASS:
            switch (face) {
                case BACK:
                    tx = 2; ty = 0;
                    if (top)
                        tx = 1; ty = 0;
                    break;
                case FRONT:
                    tx = 2; ty = 0;
                    if (top)
                        tx = 1; ty = 0;
                    break;
                case LEFT:
                    tx = 2; ty = 0;
                    if (top)
                        tx = 1; ty = 0;
                    break;
                case RIGHT:
                    tx = 2; ty = 0;
                    if (top)
                        tx = 1; ty = 0;
                    break;
                case TOP:
                    tx = 0; ty = 0;
                    break;
                case BOTTOM:
                    tx = 2; ty = 0;
                    break;
                default:
                    tx = 0; ty = 0;
                    break;
            }
            break;
        case STONE:
            switch (face) {
                case BACK:
                    tx = 3; ty = 0;
                    break;
                case FRONT:
                    tx = 3; ty = 0;
                    break;
                case LEFT:
                    tx = 3; ty = 0;
                    break;
                case RIGHT:
                    tx = 3; ty = 0;
                    break;
                case TOP:
                    tx = 3; ty = 0;
                    break;
                case BOTTOM:
                    tx = 3; ty = 0;
                    break;
                default:
                    tx = 3; ty = 0;
                    break;
            }
        default:
            tx = 3; ty = 0;
            break;
    }
    
    float cell = 1.0f / 16.0f;   // size of each tile
    float u0 = tx * cell;
    float v0 = 1.0f - (ty+1)*cell;
    float u1 = u0 + cell;
    float v1 = 1.0f - ty*cell;
    return {u0, v0, u1, v1};

}

Block::Block()
: clear(false), type(AIR), position({0,0,0})
    {
        aabb = {
            position,
            position + glm::vec3(1)
        };
        
    }

Block::Block(glm::vec3 pos) // eventually this will take in params
    : clear(false), type(GRASS), position(pos)
    {
        
        aabb = {
            position,
            position + glm::vec3(1)
        };
     
    if (position.y < CHUNK_Y-5)
        type = STONE;
    if (type == WATER) {
        clear = true;
    }
         
}

