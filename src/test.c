#include <stdio.h>

FILE *fptr;

int main(){
    fptr = fopen("./assets/cube.obj","r");
    char myfile[1000];
    printf("file open");
    
   while (fgets(myfile, 100, fptr)) {
    if (myfile[0] == 'v' && myfile[1] != 'n' && myfile[1] != 't') {
        printf("%s", myfile);
    }
    if (myfile[0] == 'f') {
        printf("%s", myfile);
    }
}
}




