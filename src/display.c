#include "display.h"

// Global variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
// Declare a pointer to an array of unsigned int 32 elements for colour buffer
uint32_t *colour_buffer = NULL;
SDL_Texture* colour_buffer_texture = NULL;
int window_width = 800;
int window_height = 600;

// single definition for the extern globals declared in display.h
enum cull_method cull_method;
enum render_method render_method;

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
			// offset each pixel by the rectangle's origin instead of drawing at (i,j)
			int current_x = x + i;
			int current_y = y + j;
			draw_pixel(current_x,current_y,colour);
		}
	}

}

void draw_line(int x0,int y0, int x1, int y1, uint32_t colour){
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	//what is the rise (side length) - sometimes delta y is larger
	int longest_side_length = (abs(delta_x) >= abs(delta_y)) ? abs(delta_x) : abs(delta_y);

	//find out how much to increment in both x and y in each step
	float x_inc = delta_x / (float)(longest_side_length);
	float y_inc = delta_y / (float)(longest_side_length);

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= longest_side_length;i++)
	{
		draw_pixel(round(current_x),round(current_y),colour);
		current_x += x_inc;
		current_y += y_inc;
	}


}

void draw_triangle(int x0, int y0,int x1, int y1,int x2, int y2,uint32_t colour){
	draw_line(x0,y0,x1,y1,colour);
	draw_line(x1,y1,x2,y2,colour);
	draw_line(x2,y2,x0,y0,colour);

}
	

void destroy_window(void)
{
	// Free the colour buffer memory
	colour_buffer = NULL;

	// Destroy the renderer and window by calling the appropriate SDL functions and passing in the pointers to the renderer and window
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	// note how we destroy in reverse order of creation, first the renderer then the window, then quit SDL
}
