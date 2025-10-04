#pragma once
#include <SDL2/SDL.h>
#include "PointCloud.hpp"
#include <sys/types.h>

class Renderer{
    public:
        //Constructor
        Renderer(u_int16_t width,u_int16_t height);
        //Deconstructor
        ~Renderer();
        //Draws Background
        void drawBackground();
        //Renders Points from PointCloud into the Frame
        void renderPoints(PointCloud& cloud);
        //Displays Frame on Window
        void presentFrame();
        //Clears the Shown Frame on the Window
        void clear();
        //Manages Poll Events from SDL
        void pollEvents();
        //checks if the Program is still running
        bool isRunning();
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        bool running_;
        u_int16_t width_, height_;
};
