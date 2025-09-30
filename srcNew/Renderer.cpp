#include "Renderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <iostream>
#include <sys/types.h>
#include <vector>

//initializes SDL and Creates Window and Renderer
Renderer::Renderer(u_int16_t width, u_int16_t height)
: width_(width), height_(height), running_(true){
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cerr<<"Video initialisation error: " << SDL_GetError() <<std::endl;
        running_ = false;
    }
    //initialitze Window and Renderer
    window_ = SDL_CreateWindow("SIM Rendering", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_, SDL_WINDOW_SHOWN);
    renderer_ = SDL_CreateRenderer(window_, -1,SDL_RENDERER_ACCELERATED); 
    //Check if Renderer and Window initilized right
    if(!window_ ||!renderer_){    
        std::cerr<<"SDL creation Error: " <<SDL_GetError() <<std::endl;
        running_ = false;
    }
}

//Closes SDL Renderer and Window Properly
Renderer::~Renderer(){
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);
    SDL_Quit();
}

//Draws Background Grid
void Renderer::drawBackground(){
    SDL_SetRenderDrawColor(renderer_,99,99,99,255);
    SDL_RenderClear(renderer_);

    //Draw Gridlines
    for(u_int16_t i = 0;i>=width_/100;i++){
        SDL_SetRenderDrawColor(renderer_,0,0,0, 255);
        SDL_RenderDrawLine(renderer_,i,i,width_,height_);
    }
    //Renders The Drawn Lines
    SDL_RenderPresent(renderer_);
}

//Draws The Scanned Points to the Renderer
void Renderer::renderPoints(PointCloud &cloud){
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);

    //Loops Thrugh the Point Cloud and Draws the containing Points to the Renderer
    std::vector<Point> c = cloud.getPoints();
    for(auto &pt: c){
       SDL_RenderDrawPoint(renderer_, (pt.x+width_/2),(pt.y+height_/2)); 
    }
    SDL_RenderPresent(renderer_);
}

//Presents The Render
void Renderer::presentFrame(){
    SDL_RenderPresent(renderer_);
}

//Clears The Renderer
void Renderer::clear(){
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
}

//returns the running variable
bool Renderer::isRunning(){
    return running_;
}

//Handels poll Events from SDL
void Renderer::pollEvents(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT){
            running_ = false;
        }
    }
}