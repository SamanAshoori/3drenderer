#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"

// Global variables
bool is_running = false;
int previous_frame_time = 0;

//////////////////////
//	Declare an array of vectors/points
///////////////////////////
#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS]; //9X9X9 cube;
vec2_t projected_points[N_POINTS];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};
//3 degrees of freedom

float fov_factor = 640;

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


//Functions that recieves 3d vector and retruns a 2d point
vec2_t project(vec3_t point){
	vec2_t projected_point = {
		.x =(fov_factor * point.x) / point.z,
		.y =(fov_factor * point.y) / point.z
	};
	return projected_point;
}

void update(void)
{
	previous_frame_time = SDL_GetTicks();
	uint64_t time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);

	//Add delta time
	if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
		SDL_Delay(time_to_wait);
	}
	
	

	cube_rotation.x += 0.01;
	cube_rotation.y += 0.01;
	cube_rotation.z += 0.01;
	for (int i = 0;i< N_POINTS;i++){
		vec3_t point = cube_points[i];

		//rotation here
		vec3_t transformed_point = vec3_rotate_x(point,cube_rotation.x);
		transformed_point = vec3_rotate_y(transformed_point,cube_rotation.y);
		//transformed_point = vec3_rotate_z(transformed_point,cube_rotation.z);

		//Translating the point away from the camera
		transformed_point.z -= camera_position.z;

		//project the current point
		vec2_t projected_point = project(transformed_point);

		//Save the projected 2d vector in the array of vector points
		projected_points[i] = projected_point;
	}
}

void render(void)
{
	//draw_grid(50);

	//Loop all projected points
	for (int i = 0;i < N_POINTS;i++){
		vec2_t projected_point = projected_points[i];
		draw_rect(
			projected_point.x + (window_width / 2),
			projected_point.y + (window_height / 2),
			4,
			4,
			0xFFFFFF00
		);

	}


	render_colour_buffer();
	clear_color_buffer(0xFF000000);


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
