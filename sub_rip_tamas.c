#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Time
{
    unsigned int h;
    unsigned int m;
    unsigned int s;
    unsigned int ms;
};

struct Subtitle
{
    unsigned int numeric_counter;
    struct Time start_time, end_time;
    char text[256];
};


void change_time(struct Time *time, int add_time)
{
    int total_seconds = 3600 * time->h + 60 * time->m + time->s + add_time; // turn everything in seconds

    // turn everything back
    time->h = total_seconds / 3600;
    total_seconds %= 3600;
    time->m = total_seconds / 60;
    total_seconds %= 60;
    time->s = total_seconds;

}

void remove_tag(char text[256])
{
    char buffer[256];
    strcpy(buffer, text + 3); // remove the first tag
    strcpy(text, buffer);
    text[strlen(text) - 6] = '\0'; // remove the last tag

}

void read_print_sub(FILE *input_file, FILE *output_file, int secs)
{
    struct Subtitle sub1;

    while(fscanf(input_file, "%d", &sub1.numeric_counter) == 1) // read from the file while we have a numeric counter for each subtitle
    {
        // read the times
        fscanf(input_file, "%d:%d:%d,%d --> %d:%d:%d,%d\n", &sub1.start_time.h, &sub1.start_time.m, &sub1.start_time.s,&sub1.start_time.ms, &sub1.end_time.h, &sub1.end_time.m, &sub1.end_time.s, &sub1.end_time.ms);

        char buffer[256];

        fgets(buffer, sizeof(buffer), input_file);
        strcpy(sub1.text, buffer);

        while(buffer[0] != '\n')
        {
            fgets(buffer, sizeof(buffer), input_file);
            strcat(sub1.text, buffer);
        }
        //change start time and end time
        change_time(&sub1.start_time, secs);
        change_time(&sub1.end_time, secs);

        remove_tag(sub1.text); // remove the tagg from the text

        fprintf(output_file, "%d\n", sub1.numeric_counter);
        fprintf(output_file,"%02d:%02d:%02d,%d --> %02d:%02d:%02d,%d\n", sub1.start_time.h, sub1.start_time.m, sub1.start_time.s,sub1.start_time.ms, sub1.end_time.h, sub1.end_time.m, sub1.end_time.s, sub1.end_time.ms);
        fprintf(output_file,"%s\n\n", sub1.text);
    }
}

int main(int argc, char **argv)
{  

    if( argc != 3)
    {
        printf("Not enough arguments.\n");
        exit(EXIT_FAILURE);
    }

    FILE *test_in = fopen(argv[1], "r");
    FILE *test_out = fopen("test_out.txt", "w");

    int shift = atoi(argv[2]);

    if(!test_in)
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    if(!test_out)
    {
        perror("fopen()");
        exit(EXIT_FAILURE);
    }

    read_print_sub(test_in,test_out, shift);

    fclose(test_in);
    fclose(test_out);
    return 0;
}