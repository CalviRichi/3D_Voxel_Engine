//
//  world.h
//  New Test Project
//
//  Created by Calvin Richards on 8/29/25.
//
#pragma once

#include "chunk.h"
#include <unordered_map>
#include <functional>

struct Vec2Hash {
    std::size_t operator()(const glm::ivec2& v) const noexcept {
        std::size_t hx = std::hash<int>()(v.x);
        std::size_t hz = std::hash<int>()(v.y);
        return hx ^ (hz << 1);
    }
};

class World {
    
private:
    
public:
    
    World(int rad_x, int rad_z);
    ~World() = default;
    
    Chunk* getPlayerChunk(glm::vec3 player_pos);
    
    static glm::vec2 worldToChunkCoords(const glm::vec3& worldPos) {
            int chunkX = std::floor(worldPos.x / CHUNK_X);
            int chunkZ = std::floor(worldPos.z / CHUNK_Z);
            return glm::vec2(chunkX, chunkZ);
        }
        
        static glm::ivec3 worldToLocalCoords(const glm::ivec3& worldPos) {
            int localX = worldPos.x % CHUNK_X;
            int localZ = worldPos.z % CHUNK_Z;
            if (localX < 0) localX += CHUNK_X;
            if (localZ < 0) localZ += CHUNK_Z;
            return glm::ivec3(localX, worldPos.y, localZ);
        }
    
    Chunk* getChunk(const glm::ivec2& coords) {
        auto it = chunks.find(coords);
        return (it != chunks.end()) ? &it->second : nullptr;
    }

    Chunk::Neighbors makeNeighbors(glm::vec2 c);
    
    std::unordered_map<glm::ivec2, Chunk, Vec2Hash> chunks;
};
