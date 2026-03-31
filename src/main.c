#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

//Global variables
bool is_running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool initalize_window(void){
	//Check if SDL was initialized successfully
	//C has no exceptions, so we have to use return values to check for errors
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Failed to initialize SDL\n");
		return false;
	}
	//Create SDL Window in the centre of the screen with dimensions 800x600 and no border
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_BORDERLESS
	);
	//Check if the window was created successfully
	if (!window){
		fprintf(stderr, "Failed to create SDL Window\n");
		return false;
	}

	//Create SDL Renderer -1 means gets the first default rendering driver and 0 means no flags
	renderer = SDL_CreateRenderer(window, -1, 0);
	//Check if the renderer was created successfully
	if (!renderer){
		fprintf(stderr, "Failed to create SDL Renderer\n");
		return false;
	}
	
	//Passed all checks, return true
	return true;
}

void setup(void){

}

void process_input(void){
	SDL_Event event;
	//Non blocking function, so render loop never stalls waiting for input.
	SDL_PollEvent(&event);

	switch(event.type){
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				is_running = false;
			break;
			
	}
}

void update(void){

}

void render(void){
	SDL_SetRenderDrawColor(renderer,0,255,0,255);
	SDL_RenderClear(renderer);

	//Present the back buffer to the screen
	SDL_RenderPresent(renderer);

}

int main(void){
	is_running = initalize_window();

	setup();

	while(is_running){
		process_input();
		update();
		render();
	}
	return 0;
}
