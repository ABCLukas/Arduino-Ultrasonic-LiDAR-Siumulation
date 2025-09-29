//compile with:
//g++ inputHandler.cpp Renderer.cpp main.cpp -o main $(sdl2-config --cflags --libs)

#include<SDL2/SDL.h>
#include<iostream>
#include "Points.hpp"

//SDL window width and height
const int WIDTH = 640;
const int HEIGHT = 480;

//Declare window and renderer
SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

//Render scannedPoints
void renderPoint(){
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	for(const auto& pt : scannedPoints){
		std::cout <<" Point"<< pt.x << " " << pt.y << std::endl;
		SDL_RenderDrawPoint(renderer, pt.x+WIDTH/2,pt.y+HEIGHT/2);
	}
	SDL_RenderPresent(renderer);
}

//close the program to quit SDL properly
void quitSDL(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int startSDL (int argc, char *argv[]) {

	//initalize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		std::cout<< "Video inilization ERROR:" << SDL_GetError() << std::endl;
		return 1;
	}
	//initialize the Window
	window = SDL_CreateWindow("SDL_Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIDTH,HEIGHT,SDL_WINDOW_SHOWN); //other flats = ||
	
	if(window == nullptr){
		std::cout<< "Window creation ERROR: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//initialize the Renderer
	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	
	if(renderer == nullptr){
		std::cout<<"Renderer creation ERROR: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	//Draw Background Black
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	
	//setup run time variables
	bool running = true;
	SDL_Event event;

	renderPoint();//render Points that have been scanned before entering the Main Loop
	while(running){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				running = false;
				quitSDL();
			}
		}
	
		SDL_Delay(16);
	}

	quitSDL();
	return 0;
}
