//
//  chunk.h
//  New Test Project
//
//  Created by Calvin Richards on 8/29/25.
//

#pragma once
#include "block.h"

const glm::ivec3 dirs[6] = {
    { 0, 0, -1},  // BACK  (-Z)
    { 0, 0,  1},  // FRONT (+Z)
    {-1, 0,  0},  // LEFT  (-X)
    { 1, 0,  0},  // RIGHT (+X)
    { 0, 1,  0},  // TOP   (+Y)
    { 0,-1,  0}   // BOTTOM(-Y)
};

class Chunk {
private:
    
    bool reload;
    
    glm::vec2 coord;
    
    VertexArray vao;
    VertexBuffer vbo;
    IndexBuffer ibo;
    
    AABB aabb;
   
public:
    
    struct Neighbors {
        Chunk * left;
        Chunk * right;
        Chunk * front;
        Chunk * back;
    };
    
    AABB getAABB() {
        return aabb;
    }
    
    Block blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
    
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    VertexArray& getVa() {
        return vao;
    }
    
    IndexBuffer& getIb() {
        return ibo;
    }
    
    void setReloadStatus(bool f) {
        reload = f;
    }
    
    bool getReloadStatus() {
        return reload;
    }
    
    glm::vec2 getCoords() const {
        return coord;
    }
    
    VertexArray getVao() { // most of these can probably be marked const
        return vao;
    }
    
    VertexBuffer getVbo() {
        return vbo;
    }
    
    IndexBuffer getIbo() {
        return ibo;
    }
    
    void uploadMesh();
    
    Chunk(glm::vec2 coordinates);
    ~Chunk() = default;
    
    void addFace(const Block& b, Face f, bool top);
    
    std::array<Vertex, 4> buildFaceVerts(glm::vec3 pos, const UVRect& uv, Face face);
    
    bool isAir(int x, int y, int z, const Neighbors& n);
    
    void bake(Neighbors n); // maybe return an int for error checking
    
    
};


