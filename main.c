#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readFile();
struct queue * interperet();
int readLine();

struct process
{
    int timeRemaining;
    int id;
};

struct queue
{
    int timeBlock;
    int processCount;
    struct process *processes;
};





int main()
{
    printf("\nloading instructions\n\n");
    readFile();
    return 0;
}

int readFile()
{
    FILE *fp;

    char buff[255];
    int count = 0;
    int lineIndex = 0;

    char *lines[100];
    fp = fopen("set1_process.in", "r");

    while (fgets(buff, 255, fp))
    {
        lines[lineIndex] = calloc(strlen(buff) + 1, sizeof(char));
        strcpy(lines[lineIndex], buff);
        lineIndex++;
    }

    for (int i = 0; i < 1; i++)
    {
        readLine(lines[i]);
    }

    int fclose(FILE * fp);
    return 0;
}

struct queue *interperet(char *words[], struct queue *queue)
{
    int processcount = 0;
    if(!strcmp(words[0], "processcount")){
        processcount = atoi(words[1]);
        queue->processCount = processcount;
        queue->processes = calloc(processcount, sizeof(struct process));
    }

    return queue;
}

int readLine(char *str)
{
    
    printf("%s", str);
    int max = strlen(str) + 1;

    char *words[50];
    int i = 0;
    int wordIndex = 0;
    int buffIndex = 0;
    char *buffer = calloc(255, sizeof(char));

     while (i < max) {
        if (str[i] == ' ' || str[i] == '#' || str[i] =='\0')
        {
            if (str[i] == '#' && buffIndex == 0){
                break;
            }

            if(str[i] == ' ' && buffIndex == 0){
                i++;
                continue;
            }
            
            //break off
            buffer[buffIndex] = '\0';
            words[wordIndex] = calloc(strlen(buffer) + 1, sizeof(char));
            strcpy(words[wordIndex++], buffer);

            printf("%s\n", buffer);
            buffIndex = 0;

            continue;
        }

        buffer[buffIndex++] = str[i++];
    }


    printf("the last word is %s \n", words[wordIndex - 1]);
    struct queue *q;
    q = calloc(1, sizeof(struct queue));
    q->timeBlock = 1;
    if(wordIndex > 0){
        interperet(words, q);
    }
    printf("the process count is %d \n", q->processCount);

    free(buffer);
    for (int i = 0; i < wordIndex; i++)
    {
        free(words[i]);
    }
    
    return 0;
}