#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readFile();
// struct queue * interperet();
void interperet();
int readLine();
void run();

struct process
{
    int waitTime;
    int turnAround;
    char id[255];
    int burst;
    int arrival;
};

struct queue
{
    int runtime;
    char method[255];
    int currentProcess;
    int processCount;
    int processCountIndex;
    struct process *processes;
};

char *methodNames[3] = {
    "First-Come First-Serve",
    "Shortest Job First (Pre)",
    "Round-Robin"};

/*enum methods {
    fcfs,
    sjf,
    rr
};*/

int main()
{
    // printf("\nloading instructions\n\n");
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
    // printf("the last word is %s \n", words[wordIndex - 1]);

    // printf("the total runtime is: %d\n", q -> runtime);
    /*for (int i = 0; i < 0 -> processCount; i++)
    {
        char name[255];
        strcpy(name, q -> processes[i].id);

        printf("process id is: %s\n", name);
        printf("the %s's process arrivial is: %d\n", name, q -> processes[i].arrival);
        printf("%s's process burst is: %d\n", name,  q -> processes[i].burst);
    }*/
    run(q);

    // printf("the first process burst is: %d\n", q -> processes[0].burst);

    int fclose(FILE * fp);
    return 0;
}

void run(struct queue *q)
{
    printf("%d processes\n", q->processCount);
    // enum methods m = q -> method;
    printf("Using %s\n\n", q->method);

    int completed = 0;

    if (!strcmp(q->method, methodNames[0]))
    {
        for (int i = 0; i < q->runtime + 1; i++)
        {
            int selectedProcess = 0;

            while (selectedProcess < q->processCount)
            {
                struct process* currentProcess = &(q->processes[selectedProcess]);
                if (currentProcess->arrival == i)
                {
                    //if arrived, echo arrived
                    printf("Time %d: %s arrived\n", i, currentProcess->id);
                }

                if (q->currentProcess == selectedProcess && currentProcess->burst > 0)
                {
                    // if selected, do work
                    currentProcess->burst--;
                }

                if (q->currentProcess == selectedProcess && currentProcess->burst == 0)
                {
                    // if finished, echo completed
                    printf("Time %d: %s finished\n", i, currentProcess->id);
                    completed++;
                    q->currentProcess = -1;
                }

                if (currentProcess->arrival <= i && currentProcess->burst > 0 && (q->currentProcess == -1))
                {
                    // if available, select it
                    printf("Time %d: %s selected (burst %d)\n", i, currentProcess->id, currentProcess->burst);
                    q->currentProcess = selectedProcess;
                    currentProcess->waitTime = i - currentProcess->arrival;
                    currentProcess->turnAround = currentProcess->waitTime + currentProcess->burst;
                }

                selectedProcess++;
            };
            if (completed == q->processCount)
            {
                printf("Finished at time %d\n\n", i);
                break;
            }
        }
        /*    P1 wait 5 turnaround 10
    P2 wait 5 turnaround 14*/
        for (int i = 0; i < q->processCount; i++)
        {
            printf("%s wait %d turnaround %d\n", q->processes[i].id,  q->processes[i].waitTime, q->processes[i].turnAround);
        }

        for (int i = 0; i < q->processCount; i++)
        {
            if (q->processes[i].burst > 0)
            {
                printf("%s wait %d did not complete\n", q->processes[i].id, q->runtime - q->processes[i].arrival);
            }
            if (q->runtime <= q->processes[i].arrival)
            {
                printf("%s could not be scheduled\n", q->processes[i].id);
            }
        }
    }
    return;
}

void interperet(char *words[], struct queue *queue)
{
    int processcount = 0;
    if (!strcmp(words[0], "processcount"))
    {
        processcount = atoi(words[1]);
        queue->processCount = processcount;
        queue->processCountIndex = 0;
        queue->currentProcess = -1;
        queue->processes = calloc(processcount, sizeof(struct process));
    }

    if (!strcmp(words[0], "runfor"))
    {
        queue->runtime = atoi(words[1]);
    }

    if (!strcmp(words[0], "use"))
    {
        if (!strcmp(words[1], "fcfs"))
        {
            strcpy(queue->method, methodNames[0]);
            return;
        }
        if (!strcmp(words[1], "sjf"))
        {
            strcpy(queue->method, methodNames[1]);
            return;
        }
        if (!strcmp(words[1], "rr"))
        {
            strcpy(queue->method, methodNames[2]);
            return;
        }
        strcpy(queue->method, "DNE");
    }

    if (!strcmp(words[0], "process"))
    {
        // printf("processes are attempted");
        queue->processes[queue->processCountIndex].arrival = atoi(words[4]);
        strcpy(queue->processes[queue->processCountIndex].id, words[2]);
        queue->processes[queue->processCountIndex++].burst = atoi(words[6]);
    }

    if (!strcmp(words[0], "end"))
    {
        // strcpy(queue -> method, words[1]);
        // do nothing
    }

    return;
}

int readLine(char *str, struct queue *q)
{

    // printf("%s", str);
    int max = strlen(str) + 1;

    char *words[50];
    int i = 0;
    int wordIndex = 0;
    int buffIndex = 0;
    char *buffer = calloc(255, sizeof(char));

    while (i < max)
    {
        if (str[i] == ' ' || str[i] == '#' || str[i] == '\0')
        {
            if (str[i] == '#' && buffIndex == 0)
            {
                break;
            }

            if (str[i] == ' ' && buffIndex == 0)
            {
                i++;
                continue;
            }

            // break off
            buffer[buffIndex] = '\0';
            words[wordIndex] = calloc(strlen(buffer) + 1, sizeof(char));
            strcpy(words[wordIndex++], buffer);

            // printf("%s\n", buffer);
            buffIndex = 0;

            if (str[i] == '\0')
            {
                break;
            }
            continue;
        }

        buffer[buffIndex++] = str[i++];
    }

    if (wordIndex > 0)
    {
        interperet(words, q);
    }

    free(buffer);
    for (int i = 0; i < wordIndex; i++)
    {
        free(words[i]);
    }

    return 0;
}