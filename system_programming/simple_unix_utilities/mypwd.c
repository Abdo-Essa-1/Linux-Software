#include <stdio.h>           /* for printf func */
#include <unistd.h>          /* for getcwd func */

#define COUNT 1000

/* Making buffer */
char buf[COUNT];

int main()
{
    /* Get current working directory and print it */
	getcwd(buf, COUNT);
	printf("%s\n", buf);

    return 0;
}