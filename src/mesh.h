#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"


#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per faces
extern face_t cube_faces[N_CUBE_FACES];


//an mesh is an array of faces and vertices - we need to use a dynamic array because we dont know mesh sizes
typedef struct {
    vec3_t* vertices; //dynamic array of vertices
    face_t* faces; //dynamic array of faces
    vec3_t rotation; //rotation with x,y and z angles (euler angles)

} mesh_t;

extern mesh_t mesh; //global mesh

void load_cube_mesh_data(void);

#endif