//
//  interact.cpp
//  New Test Project
//
//  Created by Calvin Richards on 8/30/25.
//

#include "interact.h"

RaycastHit raycast(const glm::vec3& origin,
                   const glm::vec3& dir, World& world,
                   float maxDistance = 12.0f)
{
    
    glm::vec3 step = glm::sign(dir);
    glm::vec3 tMax, tDelta;
    
    glm::ivec3 voxel = glm::floor(origin);


    for (int i = 0; i < 3; ++i) {
        if (dir[i] != 0.0f) {
            float nextVoxel = voxel[i] + (dir[i] > 0 ? 1.0f : 0.0f);
            tMax[i]   = (nextVoxel - origin[i]) / dir[i];
            tDelta[i] = 1.0f / std::abs(dir[i]);
        } else {
            tMax[i] = tDelta[i] = 1e30f;
        }
    }

    float traveled = 0.0f;
    int stepAxis = -1;
    while (traveled < maxDistance) {
        // Which chunk contains this voxel?
        glm::ivec2 cc(
            voxel.x >= 0 ? voxel.x / CHUNK_X : (voxel.x - (CHUNK_X - 1)) / CHUNK_X,
            voxel.z >= 0 ? voxel.z / CHUNK_Z : (voxel.z - (CHUNK_Z - 1)) / CHUNK_Z
        );

        const Chunk* ch = world.getChunk(cc);
        if (ch) {
            // convert world -> local indices **by subtracting the chunk base**, not modulo
            glm::vec2 base = ch->getCoords(); // already in world units
            int lx = voxel.x - (int)base.x;
            int ly = voxel.y;
            int lz = voxel.z - (int)base.y;

            if (lx >= 0 && lx < CHUNK_X &&
                ly >= 0 && ly < CHUNK_Y &&
                lz >= 0 && lz < CHUNK_Z)
            {
                if (ch->blocks[lx][ly][lz].getType() != AIR) {
                    //Face hitFace;
                    //if (tMax.x < tMax.y && tMax.x < tMax.z)      hitFace = (step.x > 0) ? LEFT : RIGHT;
                    //else if (tMax.y < tMax.z)                    hitFace = (step.y > 0) ? BOTTOM : TOP;
                    //else                                         hitFace = (step.z > 0) ? BACK   : FRONT;
                    Face hitFace;
                    switch(stepAxis) {
                        case 0: hitFace = (step.x > 0) ? LEFT : RIGHT; break;
                        case 1: hitFace = (step.y > 0) ? BOTTOM : TOP; break;
                        case 2: hitFace = (step.z > 0) ? FRONT : BACK; break;
                        default: hitFace = FRONT; // fallback
                    }
                    return { voxel, hitFace, true, cc };
                }
            }
            
            
        }

        // advance DDA
        
        //if (tMax.x < tMax.y && tMax.x < tMax.z) { voxel.x += (int)step.x; traveled = tMax.x; tMax.x += tDelta.x; }
        //else if (tMax.y < tMax.z)               { voxel.y += (int)step.y; traveled = tMax.y; tMax.y += tDelta.y; }
        //else { voxel.z += (int)step.z; traveled = tMax.z; tMax.z += tDelta.z; }
        if (tMax.x < tMax.y && tMax.x < tMax.z) {
            voxel.x += (int)step.x;
            traveled = tMax.x;
            tMax.x += tDelta.x;
            stepAxis = 0;
        } else if (tMax.y < tMax.z) {
            voxel.y += (int)step.y;
            traveled = tMax.y;
            tMax.y += tDelta.y;
            stepAxis = 1;
        } else {
            voxel.z += (int)step.z;
            traveled = tMax.z;
            tMax.z += tDelta.z;
            stepAxis = 2;
        }
    }

    return { {0,0,0}, BACK, false, {0,0} };
}

AABB_Directions * getCollisionDirections(const Player& p, const World& w) {
    AABB_Directions aabb_d;
    World world = w;
    
    glm::vec3 player = p.getPosition();
    Chunk * a = world.getPlayerChunk(player);
    
    /*
     
     */
    
    
    AABB_Directions * b = &aabb_d;
    return b;
}

void aabb_collide(keyInputs& k, World& w, Player& p) {
    
    
    /*
     
     */
    
    Chunk* ch = w.getPlayerChunk(p.getPosition());
    if (!ch) return;

    int px = (int)p.getPosition().x - ch->getCoords().x;
    int py = (int)p.getPosition().y;
    int pz = (int)p.getPosition().z - ch->getCoords().y;
    
    /*
    const glm::ivec3 dirs[6] = {
        { 0, 0, -1}, // BACK
        { 0, 0,  1}, // FRONT
        {-1, 0,  0}, // LEFT
        { 1, 0,  0}, // RIGHT
        { 0, 1,  0}, // TOP
        { 0,-1,  0}  // BOTTOM
    };
     */
    for (int i = 0; i < 6; i++) {
        int nx = px + dirs[i].x;
        int ny = py + dirs[i].y;
        int nz = pz + dirs[i].z;

        // bounds check
        if (nx < 0 || nx >= CHUNK_X || ny < 0 || ny >= CHUNK_Y || nz < 0 || nz >= CHUNK_Z)
            continue;

        const Block& block = ch->blocks[nx][ny][nz];
        if (block.getType() == AIR) continue;

        const AABB& a = block.getAABB();
        const AABB& b = p.getAABB();

        if ((a.min.x <= b.max.x && a.max.x >= b.min.x) &&
            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
            (a.min.z <= b.max.z && a.max.z >= b.min.z))
        {
            switch (i) {
                case 0: k.W_pressed = false; std::cout << "w\n"; break;
                case 1: k.S_pressed = false; std::cout << "s\n"; break;
                case 2: k.A_pressed = false; std::cout << "a\n"; break;
                case 3: k.D_pressed = false; std::cout << "d\n"; break;
                case 4: k.Space_pressed = false; std::cout << "space\n"; break;
                case 5: k.Control_pressed = false; std::cout << "control\n"; break;
            }
        }
    }
}

glm::vec3 worldInteract(Player& p, World& w, keyInputs keys, float delta) {
    
    glm::vec3 adjustPos = p.getPosition();
    //adjustPos.y += 2.0f; // add CHUNK_Y back in
    auto hit = raycast(adjustPos, p.getDirection(), w);
    
    /*
     move the player move function in here 
     
     */
    aabb_collide(keys, w, p);
    p.move(keys, delta);
    

    Chunk* ch = w.getChunk(hit.chunkCoords);
    if (!ch) return glm::vec3(0);
  
    glm::vec2 base = ch->getCoords(); // world-space base of this chunk
    int lx = hit.blockPos.x - (int)base.x;
    int ly = hit.blockPos.y;
    int lz = hit.blockPos.z - (int)base.y;

    if (lx < 0 || lx >= CHUNK_X || ly < 0 || ly >= CHUNK_Y || lz < 0 || lz >= CHUNK_Z)
        return glm::vec3(0);
    
    if (p.leftClick() && hit.hit) {
        std::cout << "X: " << hit.blockPos.x << ", Y: " << hit.blockPos.y << ", Z: " << hit.blockPos.z << std::endl;
        std::cout << "CHUNK X: " << hit.chunkCoords.x << ", CHUNK Y: " << hit.chunkCoords.y << " face: " << hit.face <<  std::endl;
        ch->blocks[lx][ly][lz].setType(AIR);
        //Chunk::Neighbors n = w.makeNeighbors(ch->getCoords());
        //ch->bake(n);
        ch->setReloadStatus(true);
    }
    else if (p.rightClick() && hit.hit) {
        
        Face f = hit.face;
        switch (f) {
            case TOP:
                std::cout << "top" << std::endl;
                ly+=1;
                break;
            case BOTTOM:
                std::cout << "bottom" << std::endl;
                ly-=1;
                break;
            case LEFT:
                std::cout << "left" << std::endl;
                lx-=1;
                break;
            case RIGHT:
                std::cout << "right" << std::endl;
                lx+=1;
                break;
            case FRONT:
                std::cout << "front" << std::endl;
                lz-=1;
                break;
            case BACK:
                std::cout << "back" << std::endl;
                lz+=1;
                break;
        }
        if (lx >= 0 && lx < CHUNK_X &&
            ly >= 0 && ly < CHUNK_Y &&
            lz >= 0 && lz < CHUNK_Z)
        {
            std::cout << "X: " << lx << ", Y: " << ly << ", Z: " << lz << std::endl;
            ch->blocks[lx][ly][lz] = Block({lx + ch->getCoords().x, ly, lz + ch->getCoords().y});
            ch->blocks[lx][ly][lz].setType(GRASS);
            std::cout << "face " << hit.face << std::endl;
            //Chunk::Neighbors n = w.makeNeighbors(ch->getCoords());
            //ch->bake(n);
            ch->setReloadStatus(true);
        }
        else {
            glm::vec2 coordChange = hit.chunkCoords;
            std::cout << "out of bounds: " << lx << ", " << ly << ", " << lz << std::endl;
            if (lx < 0) {
               
                coordChange.x -=1;
                lx = CHUNK_X - 1;
            }
            else if (lx >= CHUNK_X) {
              
                coordChange.x +=1;
                lx = 0;
            }
            if (lz < 0) { // functions correctly
               // std::cout << "this breaks" << std::endl;
                coordChange.y -=1;
                lz = CHUNK_Z -1;
            }
            else if (lz >= CHUNK_Z) {
                
                coordChange.y +=1;
                lz = 0;
            }
            
            ch = w.getChunk(coordChange);
            if (!ch) return glm::vec3(0);
            
            
            std::cout << "CHUNK X: " << hit.chunkCoords.x << ", CHUNK Y: " << hit.chunkCoords.y << " face: " << hit.face <<  std::endl;
            
            if (ly >= 0 && ly < CHUNK_Y) {
                ch->blocks[lx][ly][lz] = Block({lx + ch->getCoords().x, ly, lz + ch->getCoords().y});
                ch->blocks[lx][ly][lz].setType(GRASS);
                std::cout << "face " << hit.face << std::endl;
                //Chunk::Neighbors n = w.makeNeighbors(ch->getCoords());
                //ch->bake(n);
                ch->setReloadStatus(true);
            }
            else {
                std::cout << "outside of build limit" << std::endl;
            }
            
        }
    }
    glm::vec3 highlight = glm::vec3(lx,ly,lz);
    highlight.x += (int)base.x; highlight.z += (int)base.y;
    //highlight.y -= 34; // this is probably caused by the translation in the main file
    if (hit.hit) {
        //std::cout << "X: " << highlight.x << ", Y: " << highlight.y << ", Z: " << highlight.z << std::endl;
        return highlight;
    }
   
    return glm::vec3(0);
}
