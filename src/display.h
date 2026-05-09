#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#define FPS 120
//each frame should take the time of a millisecond divided by ideal fps to get frame target time
#define FRAME_TARGET_TIME (1000/ FPS )


extern SDL_Window *window;
extern SDL_Renderer *renderer;
// Declare a pointer to an array of unsigned int 32 elements for colour buffer
extern uint32_t *colour_buffer;
extern SDL_Texture* colour_buffer_texture;

extern int window_width;
extern int window_height;

bool initalize_window(void);
void draw_grid(int grid_size);
void draw_pixel(int x,int y,uint32_t colour);
void draw_rect(int start_x,int start_y, int height, int width, uint32_t colour);
void render_colour_buffer(void);
void clear_color_buffer(uint32_t colour);
void destroy_window(void);
void draw_line(int x0,int y0, int x1, int y1, uint32_t colour);


#endif