#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LINESIZE
    #define LINESIZE 64
#endif

size_t get_number_count(const char *const report_string)
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

void fill_report(int *report, const char *const report_string)
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

bool report_is_safe(const int *const report, const size_t len)
{
    // 1 is increasing -1 is decreasing
    const int direction = report[1] > report[0] ? 1 : -1;
    
    for (size_t i = 1; i < len; i++)
    {
        int curr = report[i];
        int prev = report[i - 1];
        if (
            (curr == prev) ||
            (direction == 1 && (curr < prev || curr > prev + 3)) ||
            (direction == -1 && (curr > prev || curr < prev - 3))
        )
            return false;
    }
    return true;
}

// returns whether the array is safe without index exclude
bool is_safe_without(const int *const report, const size_t len, const size_t exclude)
{
    size_t new_len = len - 1;
    int *new_report = malloc(new_len * sizeof(int));
    
    size_t j = 0;
    for (size_t i = 0; i < new_len; i++)
    {
        if (i == exclude)
        {
            j++;
        }
        new_report[i] = report[j];
        j++;
    }
    bool res = report_is_safe(new_report, new_len);
    free(new_report);
    return res;
}

// Returns true if the report is safe or
// if it is safe with one number removed
bool report_can_be_safe(const int *const report, const size_t len)
{
    if (!report_is_safe(report, len))
    {
        for (size_t i = 0; i < len; i++)
        {
            if (is_safe_without(report, len, i))
                return true;
        }
        return false;
    }
    else
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
    int dampened_count = 0;
    while(fgets(linebuff, LINESIZE, infile) != NULL)
    {
        size_t num_count = get_number_count(linebuff);
        int *report = malloc(num_count * sizeof(int));
        fill_report(report, linebuff);
        if (report_is_safe(report, num_count))
        {
            safe_count++;
            dampened_count++;
        }
        else if (report_can_be_safe(report, num_count))
        {
            dampened_count++;
        }
        free(report);
    }
    printf("Safe report count: %d\n", safe_count);
    printf("Dampened safe report count: %d\n", dampened_count);
    return 0;
}
