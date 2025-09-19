//
//  player.cpp
//  New Test Project
//
//  Created by Calvin Richards on 8/30/25.
//

#include "player.h"

Player::Player()
: direction(0.0, 0.0, -1.0), position(0.0, CHUNK_Y, 0.0)
{
    aabb = {
        position + glm::vec3(-0.3f,0.0f,-0.3f),
        position + glm::vec3(0.3f, 2.0f, 0.3f)
    };
}

void Player::setDirection(float pitch, float yaw) {
    
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction = glm::normalize(direction);
}

void Player::move(keyInputs keys, float delta) {
    
    glm::vec3 moveDir(direction.x, 0.0f, direction.z);
    moveDir = glm::normalize(moveDir);
    
    if (keys.W_pressed) {
        position.x += moveDir.x * speed.x * delta;
        position.z += moveDir.z * speed.z * delta;
    }
    if (keys.A_pressed) {
        position -= glm::normalize(glm::cross(direction, upwards)) * speed * delta;
    }
    if (keys.S_pressed) {
        position.x -= moveDir.x * speed.x * delta;
        position.z -= moveDir.z * speed.z * delta;
    }
    if (keys.D_pressed) {
        position += glm::normalize(glm::cross(direction, upwards)) * speed * delta;
    }
    if (keys.Control_pressed) {
        position.y -= speed.y * delta;
    }
    if (keys.Space_pressed) {
        position.y += speed.y * delta;
    }
    
    aabb = {
        position + glm::vec3(-0.3f,0.0f,-0.3f),
        position + glm::vec3(0.3f, 2.0f, 0.3f)
    };
    
    isLeftClick = keys.leftClick;
    isRightClick = keys.rightClick;
        
}
