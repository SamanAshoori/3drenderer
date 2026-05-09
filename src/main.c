#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"

triangle_t triangles_to_render[N_MESH_FACES];

// Global variables
bool is_running = false;
int previous_frame_time = 0;


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

	//loop all the triangle faces of our mesh
	for(int i = 0; i < N_MESH_FACES;i++)
	{
		face_t mesh_face = mesh_faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh_vertices[mesh_face.a - 1];
		face_vertices[1] = mesh_vertices[mesh_face.b - 1];
		face_vertices[2] = mesh_vertices[mesh_face.c - 1];

		triangle_t projected_triangle;

		//loop all 3 vertices of current face
		for (int j = 0; j < 3;j++){
			vec3_t transformed_vertex = face_vertices[j];
			//rotation here
			transformed_vertex = vec3_rotate_x(transformed_vertex,cube_rotation.x);
			transformed_vertex = vec3_rotate_y(transformed_vertex,cube_rotation.y);
			transformed_vertex = vec3_rotate_z(transformed_vertex,cube_rotation.z);

			//translate vertex away from camera
			transformed_vertex.z -= camera_position.z;


			//project current vertex
			vec2_t projected_point = project(transformed_vertex);

			//scale and translate the projected point to midddle of screen
			projected_point.x += (window_width/2);
			projected_point.y += (window_height/2);
			projected_triangle.points[j] = projected_point;

		}
		//save projected triangle in array of triangles ot render
		triangles_to_render[i] = projected_triangle;

	}

	/*for (int i = 0;i< N_POINTS;i++){
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
	}*/
}

void render(void)
{
	draw_grid(50);

	//Loop all projected triangle
	
	for (int i = 0;i < N_MESH_FACES;i++){
		triangle_t triangle = triangles_to_render[i];
		draw_rect(triangle.points[0].x,triangle.points[0].y,3,3,0xFF00FF00);
		draw_rect(triangle.points[1].x,triangle.points[1].y,3,3,0xFF00FF00);
		draw_rect(triangle.points[2].x,triangle.points[2].y,3,3,0xFF00FF00);
	}
	


	render_colour_buffer();
	clear_color_buffer(0xFF000000);


	// Present the back buffer to the screen
	SDL_RenderPresent(renderer);
}

void draw_line(int x0,int y0, int x1, int y1){
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);
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
