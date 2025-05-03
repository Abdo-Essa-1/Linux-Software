#include <stdio.h>
#include <string.h>

#define MAX_WORDS 2000
#define MAX_WORD_LENGTH 20000
#define MAX_INPUT_LENGTH 20000

char input[MAX_INPUT_LENGTH];
char prompt[MAX_WORDS][MAX_WORD_LENGTH];
int wordCount = 0;
char* token = NULL;
int i = 0;
int status = 0;

/* femtoshell_main */
int main(int argc, char *argv[])
{
    for(;;)
    {
        wordCount = 0; /* Reset word counter */

        for (i = 0; i < MAX_WORDS; i++)
        {
            prompt[i][0] = '\0'; /* Clear old words */
        }

        /* Enter the prompt */
        printf("Shopik_Lopik >> ");
        fflush(stdout);
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        /* Remove the newline character if present */
        input[strcspn(input, "\n")] = '\0';

        /* Split the sentence into words */
        token = strtok(input, " ");
        while (token != NULL && wordCount < MAX_WORDS)
        {
            /* Copy each word into the buffer */
            strncpy(prompt[wordCount], token, MAX_WORD_LENGTH - 1);
            prompt[wordCount][MAX_WORD_LENGTH - 1] = '\0'; /* Ensure null-termination */
            wordCount++;
            token = strtok(NULL, " ");
        }

        /* Handle Commands */
        if (wordCount == 0)
        {
            continue; /* No input, repeat prompt */
        }

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
        else
        {
            printf("Invalid command\n");
            status = -1;
        }
    }

    /* Return the last status */
    return status;
}