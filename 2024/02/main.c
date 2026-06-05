#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 64

size_t get_number_count(char *report_string)
{
    size_t cnt = 0;
    size_t len = strlen(report_string);
    for (size_t i = 0; i < len; i++)
    {
        if (report_string[i] == ' ' || report_string[i] == '\n')
            cnt++;
    }
    return cnt;
}

void fill_report(int *report, char *report_string)
{
    size_t len = strlen(report_string);
    size_t rep_itr = 0;

    char val_str[16];
    size_t val_itr = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (report_string[i] == ' ' || report_string[i] == '\n')
        {
            val_str[val_itr] = '\0';
            report[rep_itr++] = strtol(val_str, NULL, 10);
            val_itr = 0;
        }
        else
        {
            val_str[val_itr++] = report_string[i];
        }
    }
}

bool report_is_safe(int *report, size_t len)
{
    // 1 is increasing -1 is decreasing
    const int direction = report[1] > report[0] ? 1 : -1;
    
    for (size_t i = 1; i < len; i++)
    {
        int curr = report[i];
        int prev = report[i - 1];
        if (curr == prev)
            return false;
        if (direction == 1 && (curr < prev || curr > prev + 3))
            return false;
        if (direction == -1 && (curr > prev || curr < prev - 3))
            return false;
    }
    return true;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fputs("No input file given\n", stderr);
        return 1;
    }

    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Error opening file: %s\n", argv[1]);
        return 1;
    }

    char linebuff[LINESIZE];
    int safe_count = 0;
    while(fgets(linebuff, LINESIZE, infile) != NULL)
    {
        size_t num_count = get_number_count(linebuff);
        int *report = malloc(num_count * sizeof(int));
        fill_report(report, linebuff);
        if (report_is_safe(report, num_count))
            safe_count++;
    }
    printf("Safe report count: %d\n", safe_count);
    return 0;
}
