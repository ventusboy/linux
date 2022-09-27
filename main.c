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
    int quantum;
    char method[255];
    int currentProcess;
    int processCount;
    int processCountIndex;
    struct process *processes;
};

char *methodNames[3] = {
    "First Come First Serve",
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
    // fp = fopen("./asn1-sampleio/set3_process.in", "r");
    // fp = fopen("./asn1-sampleio/set1_process.in", "r");
    fp = fopen("./asn1-sampleio/set4_process.in", "r");

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

    run(q);

    int fclose(FILE * fp);
    return 0;
}

void run(struct queue *q)
{
    printf("%d processes\n", q->processCount);
    printf("Using %s\n", q->method);
    if (!strcmp(q->method, methodNames[2]) && q->quantum)
    {
        printf("Quantum %d\n", q->quantum);
    }
    printf("\n");

    int completed = 0;
    int quantumCount = 0;
    q->currentProcess = -1;

    for (int i = 0; i < q->runtime + 1; i++)
    {
        int prevSelectedProcess = q->currentProcess;
        int quantumCheck = 0;

        // do operations first

        for (int whoArrived = 0; whoArrived < q->processCount; whoArrived++)
        {
            struct process *currentProcess = &(q->processes[whoArrived]);
            if (currentProcess->arrival == i)
            {
                printf("Time %d: %s arrived\n", i, currentProcess->id);
                /*if (!strcmp(q->method, methodNames[1]) && q->currentProcess > -1 && currentProcess->burst < q->processes[q->currentProcess].burst)
                {
                    q->currentProcess = -1;
                }*/
            }
        }

        // bursts
        if (q->currentProcess > -1 && q->processes[q->currentProcess].burst > 0)
        {
            q->processes[q->currentProcess].burst--;
            quantumCount++;
        }

        // finish
        for (int whoFinished = 0; whoFinished < q->processCount; whoFinished++)
        {
            struct process *currentProcess = &(q->processes[whoFinished]);
            if (q->currentProcess == whoFinished && currentProcess->burst == 0)
            {
                // if finished, echo completed
                printf("Time %d: %s finished\n", i, currentProcess->id);
                completed++;
                q->currentProcess = -1;
                currentProcess->turnAround = i - currentProcess->arrival;
            }
        }

        // rr code
        if (q->quantum && quantumCount%q->quantum == 0 || q->currentProcess == -1)
        {
            int count = 1;
            while (count < q->processCount + 1)
            {
                int processToCheck = (q->currentProcess + count) % q->processCount;
                if (q->processes[processToCheck].arrival <= i && q->processes[processToCheck].burst > 0)
                {
                    q->currentProcess = processToCheck;
                    prevSelectedProcess = -1;
                    break;
                }
                count++;
            }
        }

        // sjf code
        if (!strcmp(q->method, methodNames[1]))
        {
            int shortest = 0;

            for (int k = 0; k < q->processCount; k++)
            {
                if (q->processes[k].arrival > i || q->processes[k].burst < 1) {
                    continue;
                }

                if (q->processCount - 1 == completed && q->processes[k].burst > 0)
                {
                    q->currentProcess = k;
                    break;
                } 
                
                if (q->processes[k].burst > 0 && (q->processes[k].burst < shortest || shortest == 0))
                {
                    shortest = q->processes[k].burst;
                    q->currentProcess = k;
                }
            }
            //q->currentProcess = shortest;
        }

        // fcfs code
        if (!strcmp(q->method, methodNames[0]) && q->currentProcess == -1)
        {
            int count = 0;
            while (count < q->processCount)
            {
                if (q->processes[count].arrival <= i && q->processes[count].burst > 0)
                {
                    q->currentProcess = count;
                    break;
                }
                count++;
            }
        }

        for (int t = 0; t < q->processCount; t++)
        {
            if (q->processes[t].burst > 0 && i >= q->processes[t].arrival && q->currentProcess != t)
            {
                // its waiting
                q->processes[t].waitTime++;
            }
        }

        if (prevSelectedProcess != q->currentProcess && q->currentProcess != -1)
        {
            quantumCount = 0;
            printf("Time %d: %s selected (burst %d)\n", i, q->processes[q->currentProcess].id, q->processes[q->currentProcess].burst);
        }

        if (i == q->runtime)
        {
            printf("Finished at time %d\n\n", i);
            break;
        }

        if (q->currentProcess == -1 && i < q->runtime)
        {
            printf("Time %d: IDLE\n", i);
        }
    }

    for (int i = 0; i < q->processCount; i++)
    {
        printf("%s wait %d turnaround %d\n", q->processes[i].id, q->processes[i].waitTime, q->processes[i].turnAround);
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

    if (!strcmp(words[0], "quantum"))
    {
        queue->quantum = atoi(words[1]);
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