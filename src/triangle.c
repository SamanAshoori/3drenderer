//todo
#include "triangle.h"
#include "display.h"

void int_swap(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;

}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1,int x2,int y2, uint32_t colour){
    //find the two slopes from two triangle legs
    float inv_slope_1 = (float)(x1-x0) / (y1-y0);
    float inv_slope_2 = (float)(x2-x0) / (y2-y0);

    //x_start and x_end from the top vertex (x0,y0)
    float x_start = x0;
    float x_end = x0;

    //loop all the scan lines from top to bottom
    for (int y = 0; y <= y2;y++){
        draw_line(x_start,y,x_end,y,colour);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }


}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1,int x2,int y2,uint32_t colour){

}

void draw_filled_triangle(int x0,int y0,int x1,int y1,int x2, int y2,uint32_t colour){
    //sort the vertices by y-cord ascending (y0 < y1< y2) - very poor swap
    if (y0 > y1){
        int_swap(&y0,&y1);
        int_swap(&x0,&x1);
    }
    if (y1 > y2)
    {
        int_swap(&y1,&y2);
        int_swap(&x1,&x2);
    }
    if (y0 > y1){
        int_swap(&y0,&y1);
        int_swap(&x0,&x1);
    }

    //calculate new vertex (mX,mY) using triangle similarity
    int My = y1;
    int Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2-y0)) + x0;

    
    //draw flat-bottom 
    fill_flat_bottom_triangle(x0,y0,x1,y1,Mx,My,colour);

    //draw flat top 
    fill_flat_top_triangle(x1,y1,Mx,My,x2,y2,colour);

}