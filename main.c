#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readFile(char *filename);
void interperet();
int readLine();
void run();
int test();
int push();
int pushToFront();
int shift();

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
    "First Come First Served",
    "Shortest Job First (Pre)",
    "Round-Robin"};

int push(int *array, int toPush, int size)
{
    int count = 0;
    while (count < size)
    {
        if (array[count] == -1)
        {
            array[count] = toPush;
            break;
        }
        count++;
    }
}

int pushToFront(int *array, int toPush, int size)
{
    int count = size;
    while (count > 0)
    {
        array[count] = array[count - 1];
        count--;
    }
    array[0] = toPush;
}

int shift(int *array, int size)
{
    int toReturn = array[0];
    int count = 0;
    while (count < size)
    {
        array[count] = array[count + 1];
        count++;
    }
    return toReturn;
}

int main()
{
    char *filename = calloc(255, sizeof(char));

    sprintf(filename, "process.in");
    readFile(filename);
    free(filename);
    test();
    return 0;
}

int test()
{
    for (int i = 1; i < 5; i++)
    {
        char *filename = calloc(255, sizeof(char));
        sprintf(filename, "./asn1-sampleio/set%d_process.in", i);

        // sprintf(filename, "process.in");
        readFile(filename);
        free(filename);

        char temp[255];
        char buff[255];
        char buff2[255];
        sprintf(temp, "./asn1-sampleio/set%d_processes.out", i);

        FILE *fp1;
        FILE *fp2;
        fp1 = fopen(temp, "r");
        fp2 = fopen("processes.out", "r");
        while (fp1 && fgets(buff, 255, fp1))
        {
            fgets(buff2, 255, fp2);
            int check = strcmp(buff2, buff);
            if (check != 0)
            {
                printf("error check file %d\n", i);
                break;
            }
            // printf("%s %s", buff, buff2);
        }
    }

    return 0;
}

int readFile(char *filename)
{
    FILE *fp;
    char buff[255];

    fp = fopen(filename, "r");

    struct queue *q;
    q = calloc(1, sizeof(struct queue));

    while (fgets(buff, 255, fp))
    {
        readLine(buff, q);
    }

    run(q);

    fclose(fp);
    return 0;
}

void run(struct queue *q)
{
    FILE *wfp;
    char buff[255];

    wfp = fopen("processes.out", "w");

    fprintf(wfp, "%d processes\n", q->processCount);
    // printf("%d processes\n", q->processCount);
    fprintf(wfp, "Using %s\n", q->method);
    if (!strcmp(q->method, methodNames[2]) && q->quantum)
    {
        fprintf(wfp, "Quantum %d\n", q->quantum);
    }
    fprintf(wfp, "\n");

    int completed = 0;
    int quantumCount = 0;
    q->currentProcess = -1;

    int readyQueue[q->processCount + 1];
    int readyIndex = 0;
    for (int i = 0; i < q->processCount + 1; i++)
    {
        readyQueue[i] = -1;
    }

    for (int i = 0; i < q->runtime + 1; i++)
    {
        int prevSelectedProcess = q->currentProcess;
        int quantumCheck = 0;

        // do arrivals first
        for (int whoArrived = 0; whoArrived < q->processCount; whoArrived++)
        {
            struct process *currentProcess = &(q->processes[whoArrived]);
            if (currentProcess->arrival == i)
            {
                fprintf(wfp, "Time %d: %s arrived\n", i, currentProcess->id);
                int count = readyIndex++;
                push(&readyQueue[0], whoArrived, readyIndex);
            }
        }

        // bursts
        if (q->currentProcess > -1 && q->processes[q->currentProcess].burst > 0)
        {
            q->processes[q->currentProcess].burst--;
            quantumCount++;
        }

        // finishes
        for (int whoFinished = 0; whoFinished < q->processCount; whoFinished++)
        {
            struct process *currentProcess = &(q->processes[whoFinished]);
            if (q->currentProcess == whoFinished && currentProcess->burst == 0)
            {
                // if finished, echo completed
                fprintf(wfp, "Time %d: %s finished\n", i, currentProcess->id);
                completed++;
                q->currentProcess = -1;
                currentProcess->turnAround = i - currentProcess->arrival;
            }
        }

        // rr code
        if (q->quantum && quantumCount % q->quantum == 0 || q->currentProcess == -1)
        {
            if(q->currentProcess > -1){
                push(&readyQueue[0], q->currentProcess, readyIndex);
            }
            q->currentProcess = shift(&readyQueue[0], readyIndex);

            int count = 0;
            //printf("%d is selected rr\n", q->currentProcess);
            //printf("%d is prev rr\n", prevSelectedProcess);
            prevSelectedProcess = -1;
        }
        //printf("time %d, current process %d, last process: %d\n", i, q->currentProcess, prevSelectedProcess);
        /*for (int i = 0; i < q->processCount; i++)
        {
            if(readyQueue[i] > -1){
                printf("%d[%s]\n", i, q->processes[readyQueue[i]].id);
            } else {
                printf("%d[%s]\n", i, "free");
            }
        }*/

        // sjf code
        if (!strcmp(q->method, methodNames[1]))
        {
            int shortest = 0;

            for (int k = 0; k < q->processCount; k++)
            {
                if (q->processes[k].arrival > i || q->processes[k].burst < 1)
                {
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
                q->processes[t].waitTime++;
            }
        }

        if (prevSelectedProcess != q->currentProcess && q->currentProcess != -1)
        {
            quantumCount = 0;
            fprintf(wfp, "Time %d: %s selected (burst %d)\n", i, q->processes[q->currentProcess].id, q->processes[q->currentProcess].burst);
        }

        if (i == q->runtime)
        {
            fprintf(wfp, "Finished at time %d\n\n", i);
            break;
        }

        if (q->currentProcess == -1 && i < q->runtime)
        {
            fprintf(wfp, "Time %d: IDLE\n", i);
        }
    }

    for (int i = 0; i < q->processCount; i++)
    {
        fprintf(wfp, "%s wait %d turnaround %d\n", q->processes[i].id, q->processes[i].waitTime, q->processes[i].turnAround);
    }

    for (int i = 0; i < q->processCount; i++)
    {
        if (q->processes[i].burst > 0)
        {
            fprintf(wfp, "%s wait %d did not complete\n", q->processes[i].id, q->runtime - q->processes[i].arrival);
        }
        if (q->runtime <= q->processes[i].arrival)
        {
            fprintf(wfp, "%s could not be scheduled\n", q->processes[i].id);
        }
    }
    fclose(wfp);
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
        // do nothing
    }

    return;
}

int readLine(char *str, struct queue *q)
{

    int max = strlen(str) + 1;

    char *words[255];
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