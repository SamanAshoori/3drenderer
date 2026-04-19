#include "display.h"

// Global variables
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

void draw_grid(int grid_size){
	uint32_t colour;
	colour = 0xFFFFFFFF;

	for (int y = 0; y < window_height;y++){
		for (int x = 0; x < window_width;x++){
			if(x % grid_size == 0 || y % grid_size == 0){
				colour_buffer[(window_width * y) + x] = colour;
			}
		}
	}
	
}

void draw_pixel(int x,int y, uint32_t colour){
    if (x >= 0 && x < window_width && y < window_height){
        colour_buffer[(window_width * y) + x] = colour;
        //because once the width is over you need a new row which is why its y before x
    }
    
}

void draw_rect(int x,int y, int height, int width, uint32_t colour){


	for (int i = 0; i < width;i++){
		for (int j = 0; j < height;j++){
				int current_x = x + i;
				int current_y = y + j;
				draw_pixel(current_x,current_y,colour);
				//colour_buffer[(window_width * current_y) + current_x] = colour;
		}
	}

}

void draw_rectangle(int x,int y, int height, int width, uint32_t colour){
	for (int i = 0; i <width;i++){
		for(int j = 0; j < height; j++){
			int current_x = x + i;
			int current_y = y + j;
			draw_pixel(i,j,0xff0000ff);
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


