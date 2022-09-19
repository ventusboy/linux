#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readFile();
int interperet();
int readLine();

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

int interperet(char *words[])
{
    int processcount = 0;
    if(words[0] == "processcount"){
        processcount = atoi(words[1]);
    }

    return 0;
}

int readLine(char *str)
{
    
    printf("%s", str);

    char *words[50];
    int i = 0;
    int wordIndex = 0;
    int buffindex = 0;
    char *buffer = calloc(255, sizeof(char));

    while (str[i] != '\0')
    {
        if (str[i] == ' '){// || str[i + 1] == '\0')
        {

            buffer[buffindex] = '\0';
            words[wordIndex] = calloc(strlen(buffer) + 1, sizeof(char));

            strcpy(words[wordIndex], buffer);

            printf("%s\n", buffer);

            i++;
            wordIndex++;
            buffindex = 0;

            continue;
        }
        if (str[i + 1] == '\0' || str[i] == '#')
        {
            break;
        }

        buffer[buffindex] = str[i];
        i++;
        buffindex++;
    }

    printf("the last word is is %s", words[wordIndex]);
    if(wordIndex > 0){
        interperet(words);
    }

    free(buffer);
    for (int i = 0; i < wordIndex; i++)
    {
        free(words[i]);
    }
    
    return 0;
}