#include <stdio.h>
#include "vector.h"
#include "triangle.h"
#include "array.h" // was missing

FILE *fptr;
vec3_t *vertices = NULL; // renamed from v for clarity
face_t *faces = NULL;    // was 'f', mismatched with array_push call

int main_test()
{
    fptr = fopen("./assets/cube.obj", "r");
    char myfile[1000];

    while (fgets(myfile, 100, fptr))
    {
        if (myfile[0] == 'v' && myfile[1] != 'n' && myfile[1] != 't')
        {
            vec3_t vertex;                                                 // local struct to parse into
            sscanf(myfile, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z); // was outside loop
            array_push(vertices, vertex);
        }
        if (myfile[0] == 'f')
        {
            face_t face;
            sscanf(myfile, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &face.a, &face.b, &face.c); // was outside loop
            array_push(faces, face);
        }
    }
    // print vertices
    for (int i = 0; i < array_length(vertices); i++)
    {
        printf("vertex %d: x=%.2f y=%.2f z=%.2f\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
    }

    // print faces
    for (int i = 0; i < array_length(faces); i++)
    {
        printf("face %d: a=%d b=%d c=%d\n", i, faces[i].a, faces[i].b, faces[i].c);
    }
}