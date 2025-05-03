#include <stdio.h>
#include <fcntl.h>           /* for open func */
#include <unistd.h>          /* for read, write, close funcs */
#include <stdlib.h>          /* for exit func */

#define COUNT 100

int main(int argc, char *argv[])
{
	/* Making buffer */
	char buf[COUNT] = {'\0'};

	/* Checking if there is only 3 argument */
	if(argc != 3)
	{
		printf("Usage: %s source_path dest_path\n", argv[0]);
		exit(-1);
	}

    /* Making file descriptor for the source file */
	int src = open(argv[1], O_RDONLY);

    /* Checking if Source is opened */
	if(src < 0)
	{
		printf("Could not open the source\n");
		exit(-2);
	}

	/* Making file descriptor for the destination file */
	int dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

	/* Checking if Destination is opened */
	if(dest < 0)
	{
		printf("Could not open a new file\n");
		exit(-3);
	}

	/* Reading data from the source and write in
     * the the destination till end of file
     */
	int num_read = 0;
	while((num_read = read(src, buf, COUNT)) > 0)
	{
		if(write(dest, buf, num_read) < 0)
		{
			printf("Write failed\n");
			exit(-4);
		}
	}

    /* Closing the files */
	close(src);
    close(dest);

    /* Deleting the source file */
    if (unlink(argv[1]) != 0)
    {
        printf("Error deleting file\n");
        exit(-5);
    }

	return 0;
}