#ifndef MESH_H
#define MESH_H

#include "vector.h"

#define N_MESH_VERTICES 8
vec3_t mesh_vertices[N_MESH_VERTICES] = {
	{.x = -1, .y = -1, .z = -1}, //1 
	{.x = -1, .y = 1, .z = -1}, //2
	{.x = 1, .y = 1, .z = -1}, //3
	{.x = 1, .y = -1, .z = -1}, //4
	{.x = 1, .y = 1, .z = 1}, //5
	{.x = 1, .y = -1, .z = 1},//6
	{.x = -1, .y = 1, .z = 1},//7
	{.x = -1, .y = -1, .z = 1},//8
};  

#endif