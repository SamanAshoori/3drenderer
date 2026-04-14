#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

// Global variables
bool is_running = false;

//////////////////////
//	Declare an array of vectors/points
///////////////////////////
#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS]; //9X9X9 cube;

void setup(void)
{
	// Allocate the required bytes in memory for colour buffer
	// Should be width * height * bytes per pixel (4 for RGBA)
	// malloc returns a pointer to the allocated memory so we have to cast it to the correct type (uint32_t*)
	// malloc needs number of bytes to allocate the memory so we to multyiple the size of a uint32_t by number of pixels (width * height)
	// the first uint32_t is the type of the pointer, casting the void* returned by malloc to uint32_t* so we can use it as an array of uint32_t
	colour_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
	// In real production code you should check if malloc succeeded and returned a non NULL pointer
	if (!colour_buffer)
	{
		fprintf(stderr, "Failed to allocate memory for colour buffer\n");
		exit(1); // Exit with an error code
	}
	else
	{
		//SDL_PIxelFormat means that the pixels are ALPHA,RGB Colours and each pixel has 8 bits which is why its called AGBR8888
		//streaming means we will updating the texture frame by frame
		colour_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
	}

	int point_count = 0;

	//Start loading my array of vectors
	//from -1 to 1 (in this 9x9x9 cube)
	for(float x = -1; x<= 1; x += 0.25){
		for(float y = -1; y<= 1; y += 0.25 ){
			for(float z = -1; z<= 1; z += 0.25){
				//create a new vec3 for each run of the loop
				vec3_t new_point = {.x = x,.y = y,.z = z};
				cube_points[point_count++] = new_point;
			}
		}
	}
}

void process_input(void)
{
	SDL_Event event;
	// Non blocking function, so render loop never stalls waiting for input.
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		break;
	}
}

void update(void)
{

}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	clear_color_buffer(0xFF000000); // red background
	draw_grid(100);
	draw_rect(200,200,100,200,0xFF0000FF);
	draw_pixel(10,10,0xFFFFFF00);
	render_colour_buffer();
	
	
	// Present the back buffer to the screen
	SDL_RenderPresent(renderer);
}

int main(void)
{
	is_running = initalize_window();

	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	// Free the colour buffer memory
	destroy_window();
	return 0;
}
