#include "glad.h" // graphics api
#include "indexBuffer.h"
#include "renderer.h"
#include "shader.h"
#include "texture.h"
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include <GLFW/glfw3.h> // graphics api
#include <chrono>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector> // C with vectors (and classes)

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "interact.h"
#include "player.h"
#include "world.h"

#define MOVE_SPEED 5
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define COORDINATE 3
#define TEXTURE 2

/*
Right now I just want to be able to show one block on the screen and move the
camera around it

Weak perspective projection

each vertex will be put into the projection matrix
vec3(x,y,z) X projMat = projected coordinates

I could be making my functions static, but nothing is
using main right now, so I don't see a point


Create a camera class
*/

using namespace std; // get rid of this at some point,
// I just like saying cout and string

enum Keys {
    ESCAPE = 1 << 0,
    W_DOWN = 1 << 1,
    A_DOWN = 1 << 2,
    S_DOWN = 1 << 3,
    D_DOWN = 1 << 4,
    UP_DOWN = 1 << 5,
    LEFT_DOWN = 1 << 6,
    RIGHT_DOWN = 1 << 7,
    DOWN_DOWN = 1 << 8,
    FORWARD_DOWN = 1 << 9,
    BACKWARD_DOWN = 1 << 10,
    FIVE_DOWN = 1 << 11
};

// glGetError(int errorCode); can be used for older openGL versions
// gldebugmessagecallback is a new, better error function

// IMPORTANT VECTORS AND ANGLES
static float angleX =
    glm::radians(0.0f); // Rotation angle for the blocks (can get rid of at some
                        // point, kind of just fun)
static float angleY = glm::radians(0.0f);

static const glm::vec3 up(0.0, 1.0, 0.0);
static const glm::vec3 x_axis(0.0f, 1.0, 0.0);
static const glm::vec3 y_axis(1.0f, 0.0f, 0.0f);

void mouse(GLFWwindow *window, float delta, keyInputs &keys) {
    // MOUSE MANAGEMENT

    if (GLFW_CURSOR_DISABLED == glfwGetInputMode(window, GLFW_CURSOR)) {

        double x, y;

        glfwGetCursorPos(window, &x, &y);

        if (keys.firstMouse) {
            keys.mousePos[0] = x;
            keys.mousePos[1] = y;
            keys.firstMouse = false;
        }

        float xoff = x - keys.mousePos[0];
        float yoff = keys.mousePos[1] - y;

        keys.mousePos[0] = x;
        keys.mousePos[1] = y;

        xoff *= 0.1; // sensitivity
        yoff *= 0.1;

        keys.yaw += xoff;
        keys.pitch += yoff;

        if (keys.pitch > 89.99) {
            keys.pitch = 89.99;
        }
        if (keys.pitch < -89.99) {
            keys.pitch = -89.99;
        }
        // end finding mouse position

        // now check for clicks and stuff
        bool Left_current =
            (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
        if (Left_current && !keys.leftClick) {

            keys.leftClick =
                (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS);
        }
        keys.leftClick = Left_current;

        bool Right_current =
            (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS);
        if (Right_current && !keys.rightClick) {
            keys.rightClick =
                (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS);
        }
        keys.rightClick = Right_current;
    }
}

void keyboard(GLFWwindow *window, keyInputs &keys, float delta) {

    // CONTINUOUS INPUTS

    keys.W_pressed = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    keys.A_pressed = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    keys.S_pressed = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    keys.D_pressed = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
    keys.Space_pressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    keys.Control_pressed =
        (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);

    // IGNORING THIS PART FOR NOW
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        angleY -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        angleY += 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        angleX -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        angleX += 0.05;
    }
    // this part keeps it's logic because it isn't directly tied to gameplay

    // SINGULAR INPUTS

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS &&
        !keys.Escape_pressed) {
        glfwSetWindowShouldClose(
            window, GLFW_TRUE); // later on this will prompt another screen to
                                // ask if you really want to quit
    }
    keys.Escape_pressed = (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS);

    bool K_current = (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS);
    if (K_current && !keys.K_pressed) {
        // Toggle cursor
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {
            cout << "Cursor enabled" << endl;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        } else {
            cout << "Cursor disabled" << endl;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    keys.K_pressed = K_current;
}

void frameBufferSize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void fruityRotate() {
} // vestigial function that I'm not deleting for sentimental purposes

int main(int, char **) {
    // cout << "Hello World" << endl;

    GLFWwindow *window;
    if (!glfwInit()) {
        cout << "GLFW init failed" << endl;
        glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                              "Calvin's Minecraft Clone", NULL,
                              NULL); // ex. Fruity Engine, Charli Engine
    if (!window) {
        cout << "window init failed" << endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // caps the fps for some reason

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Could not load window" << endl;
        glfwTerminate();
    }

    // glfwSetFramebufferSizeCallback(window, frameBufferSize);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);

    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    cout << "GLFW: " << glfwGetVersionString() << endl;
    {
        // BLENDING SETTINGS
        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCALL(glBlendEquation(GL_FUNC_ADD));

        // *********** ADDING BLOCKS
        Player player;
        //   cout << "got this far" << endl;
        World world = World(1, 1);
        // cout << "got this far 2" << endl;

        /*
        std::vector<Vertex> allVertices; // get rid of
        std::vector<unsigned int> allIndices; // get rid of

        allVertices.reserve(world.chunks.size() * CHUNK_SIZE * 24); // 24
        vertices per block allIndices.reserve(world.chunks.size() * CHUNK_SIZE *
        36);  // 36 indices per block

        unsigned int indexOffset = 0; // get rid of

        for (auto& c : world.chunks) {
            Chunk& chunk = c.second;

            // Add vertices
            allVertices.insert(allVertices.end(),
                               chunk.vertices.begin(),
                               chunk.vertices.end());

            // Add indices with offset
            for (auto idx : chunk.indices) {
                allIndices.push_back(idx + indexOffset);
            }

            // Update offset for next chunk
            indexOffset += static_cast<unsigned int>(chunk.vertices.size());
        }

        VertexArray va;
        VertexBuffer vb(allVertices.data(), static_cast<unsigned
        int>(allVertices.size()) * sizeof(Vertex));

        VertexBufferLayout layout;
        layout.Push<float>(3); // position
        layout.Push<float>(2); // texcoord
        va.addBuffer(vb, layout);

        IndexBuffer ib(allIndices.data(), static_cast<unsigned
        int>(allIndices.size()));
        */
        Renderer renderer;
        // Shader shader("res/shaders/basic.glsl");
        renderer.shader.Bind();
        // shader.setUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture texture(
            "res/textures/blocks.png"); // blocks.png is a block atlas
        texture.Bind();                 // implicitly using slot 0
        renderer.shader.setUniform1i("u_Texture",
                                     0); // binding texture to slot 0

        // unbinding
        // va.Unbind();
        // vb.Unbind();
        // ib.Unbind();
        renderer.shader.Unbind();
        // end unbinding

        // *********** END ADDING BLOCKS

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui::StyleColorsDark();
        ImGui_ImplOpenGL3_Init((char *)glGetString(330));

        // bool show_demo_window = true;
        // bool show_another_window = false;

        // ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        glm::mat4 p =
            glm::perspective(glm::radians(45.0f), aspect, 0.1f, 1000.0f);
        glm::vec3 t(0.0f, 0.0f, 0.0f); // transform for the WORLD
        // *** this -34.0 may be the cause for a lot of these issues ***

        keyInputs keyIn;
        auto last = std::chrono::high_resolution_clock::now();

        glm::vec3 block_highlight = glm::vec3(0);

        float timeAccumulated = 0.0f;
        int frameCount = 0;

        while (!glfwWindowShouldClose(window)) {

            // ** UPDATE STAGE **
            // *****************************************

            auto current = std::chrono::high_resolution_clock::now();
            std::chrono::duration<float> delta =
                current - last; // delta time variable

            timeAccumulated += delta.count();
            frameCount++;
            if (timeAccumulated >= 1.0f) {
                std::cout << "FPS: "
                          << (int)(frameCount / (timeAccumulated / 1.0f))
                          << std::endl;
                frameCount = 0;
                timeAccumulated = 0.0f;
            }

            glfwPollEvents();

            // cout << delta.count() << endl;

            mouse(window, delta.count(), keyIn);    // updates mouse coordinates
            keyboard(window, keyIn, delta.count()); // processes keyboard inputs

            player.setDirection(keyIn.pitch,
                                keyIn.yaw); // as the name would imply, takes in
                                            // mouse coordinates and updates the
                                            // camera direction vector
            //
            //player.move(keyIn,delta.count()); // uses current keyboard information to // translate camera position
            //
            block_highlight = worldInteract(player, world, keyIn, delta.count()); // worldInteract() processes raycasting + block
                                // breaking/placing it is also going to handle
                                // collision (that is my plan)

            glm::mat4 v =                         // view matrix
                glm::lookAt(player.getPosition(), // where you are
                            player.getPosition() +
                                player.getDirection(), // where you're looking
                            up);

            // updateLocations(&t, &angleX, &angleY);
            glm::mat4 m =                           // model matrix
                glm::translate(glm::mat4(1.0f), t); // model translation
            // m = glm::rotate(m, angleX, x_axis);     // model rotate on x
            // m = glm::rotate(m, angleY, y_axis);     // model rotate on y

            // m = glm::translate(m, t);
            //  add a scaling to keep the image smaller
            glm::mat4 mvp = p * v * m;
            // std::cout << "test1 " << std::endl;
            for (auto &ch : world.chunks) {
                auto &chunk = ch.second;
                if (chunk.getReloadStatus()) {
                    Chunk::Neighbors n = world.makeNeighbors(chunk.getCoords());
                    chunk.bake(n);
                    chunk.setReloadStatus(false);
                }
            }

            renderer.clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            renderer.shader.Bind();
            // shader.setUniform4f("u_Color", r, 0.2f, 0.8f, 1.0f);
            renderer.shader.setUniformMat4f("u_MVP", mvp);

            // std::cout << "before rendering" << std::endl;

            for (auto &chunk : world.chunks) {
                auto &ch = chunk.second;
                // std::cout << "index count: " << ch.getIb().getCount() <<
                // std::endl;
                renderer.draw(ch.getVa(), ch.getIb(), renderer.shader);
            }
            // std::cout << "after rendering" << std::endl;

            glm::mat4 vp = p * v;
            renderer.drawBlockHighlight(block_highlight, vp);
            renderer.drawCrosshair();

            {
                ImGui::SliderFloat("translation x", &t.x, -126.0f, 126.0f);
                ImGui::SliderFloat("translation y", &t.y, -60.0f, 60.0f);
                ImGui::Text("Mouse X position: %.3f, Mouse Y position: %.3f",
                            player.getDirection().x, player.getDirection().y);

                ImGui::Text("Camera X pos: %.3f, Camera Y pos: %.3f",
                            player.getPosition().x, player.getPosition().y);
                ImGui::Text("Camera Z depth: %.3f", player.getPosition().z);
                // ImGui::SliderFloat("angle", &angle, 0.0f, 2*M_PI);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                            1000.0f / ImGui::GetIO().Framerate,
                            ImGui::GetIO().Framerate);
            }

            // std::cout << "Vertices: " << allVertices.size()
            //         << " Indices: " << allIndices.size() << std::endl;

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            last = current;
            // *********************************************
            // ** END RENDER STAGE **
        }
        // va.Unbind();
        // vb.Unbind();
        // ib.Unbind();
        renderer.shader.Unbind();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // glfw terminate will get mad at you
    // delete ib and vb

    glfwTerminate();

    return 0;
}
