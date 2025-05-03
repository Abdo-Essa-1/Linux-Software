#include <stdio.h>
#include <unistd.h>          /* for write func */
#include <stdlib.h>          /* for exit func */

int i = 0;

int main(int argc, char* argv[])
{
    for(i = 1; i < argc-1; ++i)
    {
        printf("%s ", argv[i]);
    }

    printf("%s\n", argv[i]);

    return 0;
}