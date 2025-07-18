//compile with:
//g++ inputHandler.cpp Renderer.cpp main.cpp -o main $(sdl2-config --cflags --libs)
//g++ sdl.cpp -lSDL2 -o sdl

#include<SDL2/SDL.h>
#include<iostream>
#include<vector>
#include<cmath>
#include "Points.hpp"

const int WIDTH = 640;
const int HEIGHT = 480;

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;

void renderPoint(){
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	for(const auto& pt : scannedPoints){
		SDL_RenderDrawPoint(renderer, pt.x,pt.y);
	}
	SDL_RenderPresent(renderer);
}

void quitSDL(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int startSDL (int argc, char *argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		std::cout<< "Video inilization ERROR:" << SDL_GetError() << std::endl;
		return 1;
	}

		window = SDL_CreateWindow("SDL_Test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, WIDTH,HEIGHT,SDL_WINDOW_SHOWN); //other flats = ||
	if(window == nullptr){
		std::cout<< "Window creation ERROR: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
	
	if(renderer == nullptr){
		std::cout<<"Renderer creation ERROR: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	bool running = true;
	SDL_Event event;

	while(running){
		while(SDL_PollEvent(&event)){
			renderPoint();
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
