#include <glad/glad.h>//should always be first
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>//OpenGL Mathematics
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <functional>

#include "../include/GL/Time.h"
#include "../include/GL/VBO.h"
#include "../include/GL/VAO.h"
#include "../include/GL/Shader.h"
#include "../include/Tetris.h"
#include "../include/TetrisRenderer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);
void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
std::function<void()> shader_viewpoint_callback;
void printFPS();

float SCR_WIDTH = 800;
float SCR_HEIGHT = 600;

class Mouse {
public:
    Mouse(const float x,const float y):pos({x,y}){}
    Mouse():Mouse(0,0){}
    glm::vec2 pos;
    bool leftPressed = false;
    bool rightPressed = false;
    bool wheelScrolled = false;
    bool firstInput = true;
};
Mouse mouse(SCR_WIDTH/2.0f,SCR_HEIGHT/2.0f);

Tetris tetrisGame;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//opengl versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//set to core profile

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL application", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window,key_callback);
    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSwapInterval(0);//VSync
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//capture mouse

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
	//glEnable(GL_LINE_SMOOTH);
    //glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

    Shader shader("resources/shaders/shader_vs.glsl","resources/shaders/shader_fs.glsl");

    //GLuint vbo;
    //VBO::generate(vbo,sizeof(vertices),vertices,GL_STATIC_DRAW);
    //VBO::bind(vbo);
    //GLuint vao;
    //VAO::generate(vao);
    //VAO::bind(vao);
    //VAO::addAttrib(vao,0,3,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)0);//coords
    //VAO::addAttrib(vao, 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));//colors
    //VAO::addAttrib(vao,2,2,GL_FLOAT,GL_FALSE,8 * sizeof(float),(void*)(6 * sizeof(float)));//textures

    glm::mat4 projection = glm::ortho(0.0f,SCR_WIDTH,SCR_HEIGHT,0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    shader_viewpoint_callback = [&]() {
	    projection = glm::ortho(0.0f,SCR_WIDTH,SCR_HEIGHT,0.0f);
    };

    TetrisRenderer tr;

    while (!glfwWindowShouldClose(window)) {
        Time::Update();
        //processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        tetrisGame.Update();

        shader.use();
        shader.setMat4("projection",projection);
        shader.setMat4("model",model);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //VBO::deleteIt(vbo);
    //VAO::deleteIt(vao);

    glfwTerminate();
    return 0;
}

void printFPS() {
    std::cout << 1/Time::deltaTime << std::endl;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window,true);
    }
    if((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && action == GLFW_PRESS) {
        tetrisGame.MoveCurrBlock(true);
    }
    if((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && action == GLFW_PRESS) {
        tetrisGame.MoveCurrBlock(false);
    }
    if(key == GLFW_KEY_Q && action == GLFW_PRESS) {
        tetrisGame.RotateCurrBlock(true);
    }
    if(key == GLFW_KEY_E && action == GLFW_PRESS) {
        tetrisGame.RotateCurrBlock(false);
    }
}

void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    float xposIn = static_cast<float>(xpos);
    float yposIn = static_cast<float>(ypos);

    if (mouse.firstInput) {
        mouse.pos.x = xposIn;
        mouse.pos.y = yposIn;
        mouse.firstInput = false;
    }


    mouse.pos.x = xposIn;
    mouse.pos.y = yposIn;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouse.leftPressed = true;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouse.leftPressed = false;
    }
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        mouse.rightPressed = true;
    }
    if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        mouse.rightPressed = false;
    }
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

}


void framebuffer_size_callback(GLFWwindow* window,int width,int height) {
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    glViewport(0,0,width,height); //0,0 - left bottom
    shader_viewpoint_callback();
}