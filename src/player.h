//
//  player.h
//  New Test Project
//
//  Created by Calvin Richards on 8/30/25.
//
#pragma once
#include "vendor/glm/glm.hpp"
#include "commonInclude.h"
#include "block.h"

class Player {
private:
    
    bool flying;
    
    glm::vec3 direction;
    
    glm::vec3 position;
    
    glm::vec3 upwards = glm::vec3(0.0, 1.0, 0.0);
    
    bool isLeftClick = false;
    bool isRightClick = false;
    bool leftClickPrev = false;
    bool rightClickPrev = false;
    
    AABB aabb;
public:
    
    const glm::vec3 speed = glm::vec3(10); // player speed
    
    glm::vec3 getPosition() const {
        return position;
    }
    glm::vec3 getDirection() const {
        return direction;
    }
    
    AABB getAABB() {
        return aabb;
    }
    
    Player();
    ~Player() = default;
    void setDirection(float pitch, float yaw);
    void move(keyInputs keys, float delta);
    
    bool leftClick() {
        bool pressed = isLeftClick;
        bool triggered = pressed && !leftClickPrev; // true only on first frame of click
        leftClickPrev = pressed;
        return triggered;
    }
    bool rightClick() {
        
        bool pressed = isRightClick;
        bool triggered = pressed && !rightClickPrev;
        rightClickPrev = pressed;
        return triggered;
    }
};
