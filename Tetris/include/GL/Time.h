#pragma once

#include <GLFW/glfw3.h>

//Wrapper class for GLFW time (time in seconds)
class Time
{
private:
    inline static double lastTime = 0;
public:
    Time() = delete;
    inline static double time = 0;
    inline static double deltaTime = 0;
    static void Update() {
        time = glfwGetTime();
        deltaTime = time - lastTime;
        lastTime = time;
    }
};