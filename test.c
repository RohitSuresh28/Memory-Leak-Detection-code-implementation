#include<malloc.h>
#include"findLeak.h"

int main()
{
     int *p1 = (int *)malloc(10);
     int *p2 = (int *)calloc(10, sizeof(int));
     char *p3 = (char *) calloc(15, sizeof(float));
     float *p4 = (float*) malloc(16);
     free(p2);
     WriteMemLeak();
     return 0;
 }
