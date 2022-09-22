#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readFile();
//struct queue * interperet();
void interperet();
int readLine();
void run();

struct process
{
    int timeRemaining;
    char id[255];
    int burst;
    int arrival;
};

struct queue
{
    int runtime;
    char method[10];
    int currentProcess;
    int processCount;
    int processCountIndex;
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

    struct queue *q;
    q = calloc(1, sizeof(struct queue));
    for (int i = 0; i < lineIndex; i++)
    {
        readLine(lines[i], q);
    }
    //printf("the last word is %s \n", words[wordIndex - 1]);

    printf("the process count is %d \n", q->processCount);
    printf("the total runtime is: %d\n", q -> runtime);
    printf("the method is: %s\n", q -> method);
    for (int i = 0; i < q -> processCount; i++)
    {
        char name[255];
        strcpy(name, q -> processes[i].id);

        printf("process id is: %s\n", name);
        printf("the %s's process arrivial is: %d\n", name, q -> processes[i].arrival);
        printf("%s's process burst is: %d\n", name,  q -> processes[i].burst);
    }
    run(q);
    
    //printf("the first process burst is: %d\n", q -> processes[0].burst);


    int fclose(FILE * fp);
    return 0;
}

void run(struct queue *q){
    if(!strcmp(q->method, "fcfs")){
        for (int i = 0; i < q->runtime + 1; i++)
        {
            int p = 0;

            while(p < q->processCount)
            {
                if(q->processes[p].arrival < i && q->processes[p].burst > 0 && (q -> currentProcess == p || q -> currentProcess == -1)){
                    q->processes[p].burst--;
                    q->currentProcess = p;
                    
                    if(q->processes[p].burst == 0){
                        printf("process %s is complete at time %d\n", q->processes[p].id, i);
                        q->currentProcess = -1;
                        //i--;
                        //p=0;
                        break;
                    }
                }
                p++;
            };
        }
        
    }
    return;
}

void interperet(char *words[], struct queue *queue)
{
    int processcount = 0;
    if(!strcmp(words[0], "processcount")){
        processcount = atoi(words[1]);
        queue->processCount = processcount;
        queue->processCountIndex = 0;
        queue->currentProcess = -1;
        queue->processes = calloc(processcount, sizeof(struct process));
    }

    if(!strcmp(words[0], "runfor")){
        queue -> runtime = atoi(words[1]);
    }

    if(!strcmp(words[0], "use")){
        strcpy(queue -> method, words[1]);
    }

    if(!strcmp(words[0], "process")){
        printf("processes are attempted");
        queue -> processes[queue -> processCountIndex].arrival = atoi(words[4]);
        strcpy(queue -> processes[queue -> processCountIndex].id, words[2]);
        queue -> processes[queue -> processCountIndex++].burst = atoi(words[6]);
    }

    if(!strcmp(words[0], "end")){
        //strcpy(queue -> method, words[1]);
        //do nothing
    }

    return;
}

int readLine(char *str, struct queue *q)
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

            if(str[i] == '\0'){
                break;
            }
            continue;
        }

        buffer[buffIndex++] = str[i++];
    }

    if(wordIndex > 0){
        interperet(words, q);
    }

    free(buffer);
    for (int i = 0; i < wordIndex; i++)
    {
        free(words[i]);
    }
    
    return 0;
}