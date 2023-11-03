#include "window.h"

bool Window::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Error initializing SDL2: %s\n", SDL_GetError());
		return false;
	}
	printf("DEBUG: SDL Successfully Initialized!\n");


	this->win = SDL_CreateWindow(
		this->windowTitle,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		this->width,
		this->height,
		0
	);


	if (!this->win) {
		printf("Error initializing Window: %s\n", SDL_GetError());
		return false;
	}
	printf("DEBUG: Window Successfully Initialized!\n");


	this->renderer = SDL_CreateRenderer(
		this->win,
		-1,
		SDL_RENDERER_ACCELERATED
	);


	if (!this->renderer) {
		printf("Error initializing Renderer: %s\n", SDL_GetError());
		return false;
	}
	printf("DEBUG: Renderer Successfully Initialized!\n");
	return true;
}


int Window::Run() {
	if (!this->Init()) return -1;
}