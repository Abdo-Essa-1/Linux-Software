#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_WORDS 2000
#define MAX_WORD_LENGTH 20000
#define MAX_INPUT_LENGTH 20000

char input[MAX_INPUT_LENGTH];
char prompt[MAX_WORDS][MAX_WORD_LENGTH];
int wordCount = 0;
char* token = NULL;
int i = 0;
int status = 0;

/* picoshell_main */
int main(int argc, char *argv[])
{
    for(;;)
    {
        /* Reset */
        wordCount = 0;
        for (i = 0; i < MAX_WORDS; i++)
        {
            /* Clear old words */
            prompt[i][0] = '\0';
        }

        /* Enter the prompt */
        printf("SHOBIK LOBIK $ ");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        /* Remove the newline */
        input[strlen(input) - 1] = 0;

        /* If empty, Continue */
        if(strlen(input) == 0)
        {
            continue;
        }

        /* Split the sentence into words */
        token = strtok(input, " ");
        while (token != NULL && wordCount < MAX_WORDS)
        {
            /* Copy each word into the prompt */
            strncpy(prompt[wordCount], token, MAX_WORD_LENGTH - 1);
            /* Ensure null-termination */
            prompt[wordCount][MAX_WORD_LENGTH - 1] = '\0';
            wordCount++;
            token = strtok(NULL, " ");
        }

        /* Handle Commands */
        if(!strcmp(prompt[0], "echo"))
        {
            i = 1;
            while(i+1 < wordCount)
            {
                printf("%s ", prompt[i]);
                ++i;
            }
            printf("%s\n", prompt[i]);
            status = 0;
        }
        else if(!strcmp(prompt[0], "exit"))
        {
            printf("Good Bye\n");
            break;
        }
        else if(!strcmp(prompt[0], "pwd"))
        {
            /* Get current working directory and print it */
            getcwd(input, MAX_INPUT_LENGTH);
            printf("%s\n", input);
            status = 0;
        }
        else if(!strcmp(prompt[0], "cd"))
        {
            if (!chdir(prompt[1]))
            {
                /* Success */
                status = 0;
            }
            else
            {
                /* Print Error */
                printf("cd: %s: No such file or directory\n", prompt[1]);
                status = -1;
            }
        }
        else
        {
            /* create new process using fork */
            pid_t pid = fork();

            /* Checking whether i am the parent or i am the child */
            if(pid > 0)
            {
                /*  Wait for the child to terminate*/
                wait(&status);

                /* If Error, make sure that the status is Error */
                if(status > 0)
                {
                    status = -1;
                }
            }
            else if(pid == 0)
            {
                /* Making the program arguments */
                char* newargv[MAX_WORDS] = {NULL};
                for(i = 0; i < wordCount; ++i)
                {
                    newargv[i] = prompt[i];
                }

                /* Executing the program */
                execvp(prompt[0], newargv);

                /* If we arrive here then an error happened */
                printf("%s: command not found\n", prompt[0]);
                exit(-1);
            }
            else
            {
                /* Fork didn't success */
                status = -1;
            }
        }
    }

    /* Return the last status */
    return status;
}