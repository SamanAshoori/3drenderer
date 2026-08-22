#include "mesh.h"
#include "string.h"
#include "array.h"
#include <stdio.h>
// TODO Make implementations

mesh_t mesh = {
	.vertices = NULL,
	.faces = NULL,
	.rotation = {0, 0, 0}};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{.x = -1, .y = -1, .z = -1}, // 1
	{.x = -1, .y = 1, .z = -1},	 // 2
	{.x = 1, .y = 1, .z = -1},	 // 3
	{.x = 1, .y = -1, .z = -1},	 // 4
	{.x = 1, .y = 1, .z = 1},	 // 5
	{.x = 1, .y = -1, .z = 1},	 // 6
	{.x = -1, .y = 1, .z = 1},	 // 7
	{.x = -1, .y = -1, .z = 1}	 // 8
};

face_t cube_faces[N_CUBE_FACES] = {
	// front (red face)
	{.a = 1, .b = 2, .c = 3, .colour = 0xFFFF0000},
	{.a = 1, .b = 3, .c = 4, .colour = 0xFFFF0000},
	// right (green)
	{.a = 4, .b = 3, .c = 5, .colour = 0xFF00FF00},
	{.a = 4, .b = 5, .c = 6, .colour = 0xFF00FF00},
	// back (blue)
	{.a = 6, .b = 5, .c = 7, .colour = 0xFF0000FF},
	{.a = 6, .b = 7, .c = 8, .colour = 0xFF0000FF},
	// left (purple)
	{.a = 8, .b = 7, .c = 2, .colour = 0xFFFF00FF},
	{.a = 8, .b = 2, .c = 1, .colour = 0xFFFF00FF},
	// top (teal)
	{.a = 2, .b = 7, .c = 5, .colour = 0xFF00FFFF},
	{.a = 2, .b = 5, .c = 3, .colour = 0xFF00FFFF},
	// bottom (yellow)
	{.a = 6, .b = 8, .c = 1, .colour = 0xFFFFFF00},
	{.a = 6, .b = 1, .c = 4, .colour = 0xFFFFFF00},
};

void load_cube_mesh_data(void)
{
	for (int i = 0; i < N_CUBE_VERTICES; i++)
	{
		vec3_t cube_vertex = cube_vertices[i];
		array_push(mesh.vertices, cube_vertex);
	}
	for (int i = 0; i < N_CUBE_FACES; i++)
	{
		face_t cube_face = cube_faces[i];
		array_push(mesh.faces, cube_face);
	}
}

void saman_load_obj_file_data(char *filename)
{
	FILE *fptr = fopen(filename, "r");
	char line[1000];

	while (fgets(line, 1000, fptr))
	{
		if (line[0] == 'v' && line[1] != 'n' && line[1] != 't')
		{
			vec3_t vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			array_push(mesh.vertices, vertex);
		}
		if (line[0] == 'f')
		{
			face_t face;
			sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &face.a, &face.b, &face.c);
			array_push(mesh.faces, face);
		}
	}
	fclose(fptr);
}

void load_obj_file_data(char *filename)
{
	FILE *file;
	file = fopen(filename, "r");

	char line[1024];

	while (fgets(line, 1024, file))
	{
		// vertex information
		if (strncmp(line, "v ", 2) == 0)
		{
			vec3_t vertex;
			sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
			array_push(mesh.vertices, vertex)
		}
		// face information
		if (strncmp(line, "f ", 2) == 0)
		{
			int vertex_indices[3];
			int texture_indices[3];
			int normal_indices[3];
			sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
				   &vertex_indices[0], &texture_indices[0], &normal_indices[0],
				   &vertex_indices[1], &texture_indices[1], &normal_indices[1],
				   &vertex_indices[2], &texture_indices[2], &normal_indices[2]);

			face_t face = {
				.a = vertex_indices[0],
				.b = vertex_indices[1],
				.c = vertex_indices[2]
			};
			array_push(mesh.faces,face)
		}
	}
}