//
//  world.cpp
//  New Test Project
//
//  Created by Calvin Richards on 8/29/25.
//

#include "world.h"

Chunk::Neighbors World::makeNeighbors(glm::vec2 c) {
    
    Chunk * l = getChunk(glm::vec2((c.x / CHUNK_X) -1, (c.y / CHUNK_Z)));
    Chunk * r = getChunk(glm::vec2((c.x / CHUNK_X) +1, (c.y / CHUNK_Z)));
    Chunk * f = getChunk(glm::vec2((c.x / CHUNK_X), (c.y / CHUNK_Z) +1));
    Chunk * b = getChunk(glm::vec2((c.x / CHUNK_X), (c.y / CHUNK_Z) -1));
    return Chunk::Neighbors(l,r,f,b);
}

World::World(int rad_x, int rad_z) {
    for (int x = -rad_x; x < rad_x; x++) {
        for (int z = -rad_z; z < rad_z; z++) {
            glm::ivec2 coord(x, z);
            //chunks.emplace(coord, Chunk(coord));
            chunks.try_emplace(coord, coord);
            //std::cout << "another test message" << std::endl;
        }
    }
   // std::cout << "begin baking" << std::endl;
    for (auto& ch : chunks) {
        auto& chunk = ch.second;
       // std::cout << "x: " << chunk.getCoords().x << ", y: " << chunk.getCoords().y << std::endl;
        Chunk::Neighbors n = makeNeighbors(chunk.getCoords());
        chunk.bake(n);
    }
}

Chunk* World::getPlayerChunk(glm::vec3 player_pos) {
    int cx = static_cast<int>(floor(player_pos.x / CHUNK_X));
    int cz = static_cast<int>(floor(player_pos.z / CHUNK_Z));
    
    auto it = chunks.find({cx, cz});
        if (it != chunks.end())
            return &it->second; // pointer to the chunk
    return nullptr;        // could be missing
}


