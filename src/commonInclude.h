#pragma once

#include "glad.h"
#include <iostream>
#include <csignal>
#include <string>
#include "vendor/glm/glm.hpp"

#define SIGTRAP 5 // redefined for use of windows compilation
#define ASSERT(x) if(!(x)) raise(SIGTRAP);
#define GLCALL(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();

bool GLLogCall(const std::string function, const std::string file, int line);

struct keyInputs { // just to bundle keyboard stuff together
    
    // OTHER
    
    bool firstMouse = true; // weird variable for setup of mouse input
    
    double mousePos[2] = {0.0, 0.0};
    double yaw = -89.99;
    double pitch = 0.0;
    
    // KEYBOARD
    
    bool W_pressed;
    bool A_pressed;
    bool S_pressed;
    bool D_pressed;
    bool K_pressed = false;
    bool Escape_pressed = false;
    bool Control_pressed;
    bool Space_pressed;
   
    // MOUSE
    
    bool leftClick = false;
    bool rightClick = false;
    
};

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};
