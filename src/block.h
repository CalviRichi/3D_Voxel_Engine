#pragma once

#include "commonInclude.h"
#include "texture.h"
#include <vector>
#include "vendor/glm/glm.hpp"

#define CHUNK_X 16
#define CHUNK_Y 32
#define CHUNK_Z 16

#define CHUNK_SIZE (CHUNK_X * CHUNK_Y * CHUNK_Z)

struct Vertex {
    glm::vec3 pos;
    glm::vec2 texCoord;
};

struct UVRect {
    float u0, v0, u1, v1;
};

enum Type {
    GRASS,
    STONE,
    WATER,
    AIR
};

enum Face {
    BACK = 0,
    FRONT,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

class Block {

  private:
    
    AABB aabb;
    
    bool clear;
    Type type;
    glm::vec3 position;
    bool top = false;
    
  public:
    
    Block();
    Block(glm::vec3);
    ~Block() = default;
    
    AABB getAABB() const {
        return aabb;
    }
    
    bool getTop() {
        return top;
    }
    
    void setTop(bool t) {
        top = t;
    }
    
    Type getType() const {
        return type;
    }

    bool isClear() const {
        return clear;
    }

    glm::vec3 getPos() const {
        return position;
    }
    
    void setType(Type t) {
        type = t;
    }
    void addFace(const Vertex faceVerts[4]);
    
    static UVRect getTileUV(Type t, Face face, bool top);
};
