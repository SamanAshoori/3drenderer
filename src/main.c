#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

// Global variables
bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL; 

// Declare a pointer to an array of unsigned int 32 elements for colour buffer
uint32_t *colour_buffer = NULL;
SDL_Texture* colour_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

//Set colour buffertexture


bool initalize_window(void)
{
	// Check if SDL was initialized successfully
	// C has no exceptions, so we have to use return values to check for errors
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Failed to initialize SDL\n");
		return false;
	}

	//USE SDL to get maximum width and height of the display we are rendering with
	SDL_DisplayMode displaymode;
	//passes the address of display mode to modify the data in it
	SDL_GetCurrentDisplayMode(0,&displaymode);
	
	window_width = displaymode.w;
	window_height = displaymode.h;

	// Create SDL Window in the centre of the screen with dimensions 800x600 and no border
	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		window_width,
		window_height,
		SDL_WINDOW_BORDERLESS);
	// Check if the window was created successfully
	if (!window)
	{
		fprintf(stderr, "Failed to create SDL Window\n");
		return false;
	}

	// Create SDL Renderer -1 means gets the first default rendering driver and 0 means no flags
	renderer = SDL_CreateRenderer(window, -1, 0);
	// Check if the renderer was created successfully
	if (!renderer)
	{
		fprintf(stderr, "Failed to create SDL Renderer\n");
		return false;
	}
	SDL_SetWindowFullscreen(window,SDL_WINDOW_FULLSCREEN);

	// Passed all checks, return true
	
	
	return true;
}

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

void update(void)
{

}

void render_colour_buffer(void){
	SDL_UpdateTexture(
		colour_buffer_texture,
		NULL,
		colour_buffer,
		(int)(window_width * sizeof(uint32_t))
		
	);
	SDL_RenderCopy(
		renderer,
		colour_buffer_texture,
		NULL,
		NULL
	);
}

void clear_color_buffer(uint32_t colour){
	for (int y = 0; y < window_height;y++){
		for (int x = 0; x < window_width;x++){
			colour_buffer[(window_width * y) + x] = colour;
		}
	}
}



void destroy_window(void)
{
	// Free the colour buffer memory
	free(colour_buffer);
	colour_buffer = NULL;

	// Destroy the renderer and window by calling the appropriate SDL functions and passing in the pointers to the renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	// note how we destroy in reverse order of creation, first the renderer then the window, then quit SDL
}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	

	clear_color_buffer(0xFFFFFF00);
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
