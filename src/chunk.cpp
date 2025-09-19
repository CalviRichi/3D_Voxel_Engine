//
//  chunk.cpp
//  New Test Project
//
//  Created by Calvin Richards on 8/29/25.
//

#include "chunk.h"

Chunk::Chunk(glm::vec2 coordinates)
: coord(coordinates), reload(false)
{
    coord.x *= CHUNK_X; coord.y*=CHUNK_Z;
    
    aabb = {
        glm::vec3(coord.x * CHUNK_X, 0, coord.y * CHUNK_Z),
        glm::vec3((coord.x + 1) * CHUNK_X, CHUNK_Y, (coord.y + 1) * CHUNK_Z)
    };
    
    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < (CHUNK_Y / 2); y++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                blocks[x][y][z] = Block({x + coord.x, y, z + coord.y});
            }
        }
    }
}

std::array<Vertex, 4> Chunk::buildFaceVerts(glm::vec3 pos, const UVRect &uv, Face face) {
    
        std::array<Vertex, 4> faceVerts;
        switch (face) {

        case BACK:
            faceVerts = {{{{-0.5f, -0.5f, -0.5f}, {uv.u0, uv.v0}},
                          {{0.5f, -0.5f, -0.5f}, {uv.u1, uv.v0}},
                          {{0.5f, 0.5f, -0.5f}, {uv.u1, uv.v1}},
                          {{-0.5f, 0.5f, -0.5f}, {uv.u0, uv.v1}}}};
            break;
        case FRONT:
            faceVerts = {{{{-0.5f, -0.5f, 0.5f}, {uv.u0, uv.v0}},
                          {{0.5f, -0.5f, 0.5f}, {uv.u1, uv.v0}},
                          {{0.5f, 0.5f, 0.5f}, {uv.u1, uv.v1}},
                          {{-0.5f, 0.5f, 0.5f}, {uv.u0, uv.v1}}}};
            break;
        case LEFT:
            faceVerts = {{{{-0.5f, -0.5f, -0.5f}, {uv.u0, uv.v0}},
                          {{-0.5f, -0.5f, 0.5f}, {uv.u1, uv.v0}},
                          {{-0.5f, 0.5f, 0.5f}, {uv.u1, uv.v1}},
                          {{-0.5f, 0.5f, -0.5f}, {uv.u0, uv.v1}}}};
            break;
        case RIGHT:
            faceVerts = {{{{0.5f, -0.5f, 0.5f}, {uv.u0, uv.v0}},
                          {{0.5f, -0.5f, -0.5f}, {uv.u1, uv.v0}},
                          {{0.5f, 0.5f, -0.5f}, {uv.u1, uv.v1}},
                          {{0.5f, 0.5f, 0.5f}, {uv.u0, uv.v1}}}};
            break;
        case TOP:
            faceVerts = {{{{-0.5f, 0.5f, 0.5f}, {uv.u0, uv.v0}},
                          {{0.5f, 0.5f, 0.5f}, {uv.u1, uv.v0}},
                          {{0.5f, 0.5f, -0.5f}, {uv.u1, uv.v1}},
                          {{-0.5f, 0.5f, -0.5f}, {uv.u0, uv.v1}}}};
            break;
        case BOTTOM:
            faceVerts = {{{{-0.5f, -0.5f, -0.5f}, {uv.u0, uv.v0}},
                          {{0.5f, -0.5f, -0.5f}, {uv.u1, uv.v0}},
                          {{0.5f, -0.5f, 0.5f}, {uv.u1, uv.v1}},
                          {{-0.5f, -0.5f, 0.5f}, {uv.u0, uv.v1}}}};
            break;
        }
        for (int v = 0; v < 4; v++) {
            glm::vec3 adjust = glm::vec3(0.5f);
            faceVerts[v].pos += pos + adjust;
           // faceVerts[v].pos.x += coord.x;
           // faceVerts[v].pos.z += coord.y;
        }
        
    
    return faceVerts;
}

void Chunk::addFace(const Block& b, Face face, bool top) {
    
    
    //
    UVRect uv = Block::getTileUV(b.getType(), face, top);
    std::array<Vertex, 4> faceVerts = buildFaceVerts(b.getPos(), uv, face);
    unsigned int baseIndex = (unsigned int)vertices.size();
    
    
    // Add vertices
    for (int i = 0; i < 4; i++)
        vertices.push_back(faceVerts[i]);

    // Add indices (2 triangles per face)
    indices.push_back(baseIndex + 0);
    indices.push_back(baseIndex + 1);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 2);
    indices.push_back(baseIndex + 3);
    indices.push_back(baseIndex + 0);
}

bool Chunk::isAir(int x, int y, int z, const Neighbors& n) {
    
    // just checks if either a block is out of bounds and then if not, checks if
    // it is air in the future, rather than returning true on an out of bounds,
    // it would then load an adjacent chunk and do the same check
    if (y < 0 || y >= CHUNK_Y) return true;
   
    if (x < 0 || x >= CHUNK_X || z < 0 || z >= CHUNK_Z) {
       // std::cout << x << ", " << y << ", " << z << std::endl;
         
    }
    if (x < 0) {
        if (!n.left) {
            //std::cout << "no left chunk" << std::endl;
        }
        bool val = (!n.left || n.left->blocks[CHUNK_X - 1][y][z].getType()==AIR);
        if (!val && n.left) { // this works
            n.left->setReloadStatus(true);
        }
        //std::cout << "x < 0: " << val << std::endl;
        return val;
    }
    if (x >= CHUNK_X) {
        return (!n.right || n.right->blocks[0][y][z].getType()==AIR);
    }
    if (z < 0) {
        return (!n.back || n.back->blocks[x][y][CHUNK_Z-1].getType()==AIR);
    }
    if (z >= CHUNK_Z) {
        return (!n.front || n.front->blocks[x][y][0].getType()==AIR);
    }
    return (blocks[x][y][z].getType() == AIR);
 
}

void Chunk::uploadMesh() {
    vao.Bind();
  //  std::cout << "before vbo set data" << std::endl;
    vbo.setData(vertices.data(), static_cast<unsigned int>(vertices.size()) * sizeof(Vertex));
    //vertices.data(), (unsigned int)vertices.size() * sizeof(Vertex)
   // std::cout << "after vbo set data" << std::endl;
    VertexBufferLayout layout;
    layout.Push<float>(3); // pos
    layout.Push<float>(2); // uv
    vao.addBuffer(vbo, layout);

    ibo.setData(indices.data(), static_cast<unsigned int>(indices.size()));
    //indices.data(), (unsigned int)indices.size()
}

void Chunk::bake(Neighbors neighbors) { // take in left, right, front, back
    
    vertices.clear(); indices.clear();
    //std::cout << "current chunk x: " << getCoords().x << ", y: " << getCoords().y << std::endl;
  //  if (neighbors.left) std::cout << "neighbor left: " << neighbors.left->getCoords().x << ", " << neighbors.left->getCoords().y << std::endl;
  //  if (neighbors.right)std::cout << "neighbor right: " << neighbors.right->getCoords().x << ", " << neighbors.right->getCoords().y << std::endl;
 //   if (neighbors.front)std::cout << "neighbor front: " << neighbors.front->getCoords().x << ", " << neighbors.front->getCoords().y << std::endl;
 //   if (neighbors.back)std::cout << "neighbor back: " << neighbors.back->getCoords().x << ", " << neighbors.back->getCoords().y << std::endl;
    for (int x = 0; x < CHUNK_X; x++) {
        for (int y = 0; y < CHUNK_Y; y++) {
            for (int z = 0; z < CHUNK_Z; z++) {
                if (blocks[x][y][z].getType() != AIR) {
                    
                    bool t = isAir(x + dirs[4].x, y + dirs[4].y, z + dirs[4].z, neighbors);
                    
                    for (int f = 0; f < 6; f++) {
                        
                        glm::ivec3 n = {x, y, z};
                        n += dirs[f];
                        
                        if (isAir(n.x, n.y, n.z, neighbors)) {
                            addFace(blocks[x][y][z], (Face)f, t);
                        }
                        
                        
                    }
                }
                else {
                    //std::cout << "air face added at: " << x << ", " << y << ", " << z << std::endl;
                }
            }
        }
    }
//   std::cout << "before upload mesh" << std::endl;
    uploadMesh();
   // std::cout << "after upload mesh" << std::endl;
}


