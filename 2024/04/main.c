#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 248

enum horiz_dir {
    RIGHT,
    LEFT,
    NONE
};

enum vert_dir {
    UP,
    DOWN,
    NONEV
};

bool matches_direction(
    char **grid, 
    const size_t len, 
    const size_t row, 
    const size_t col, 
    enum horiz_dir h_dir, 
    enum vert_dir v_dir,
    const char *target
)
{
    const size_t target_len = strlen(target);
    int row_mod = 0;
    int col_mod = 0;
    switch (v_dir)
    {
        case UP:
            row_mod = -1;
            if (row < target_len - 1)
                return false;
            break;
        case DOWN:
            row_mod = 1;
            if (row > len - target_len)
                return false;
            break;
	default:
	    break;
    }
    switch (h_dir)
    {
        case LEFT:
            col_mod = -1;
            if (col < target_len - 1)
                return false;
            break;
        case RIGHT:
            col_mod = 1;
            if (col > len - target_len)
                return false;
            break;
	default:
	    break;
    }

    bool match = true;
    for (int i = 0; i < (int)target_len; i++)
    {
        const size_t row_i = row + (i * row_mod);
        const size_t col_i = col + (i * col_mod);
        if (grid[row_i][col_i] != target[i])
        {
            match = false;
            break;
        }
    }
    return match;
}

// Part 1
int count_xmas(char **grid, const size_t len)
{
    int count = 0;
    static const char *target = "XMAS";

    for (size_t row = 0; row < len; row++)
    {
        for (size_t col = 0; col < len; col++)
        {
            if (grid[row][col] != 'X')
                continue;
            if (matches_direction(grid, len, row, col, RIGHT, NONEV, target))
                count++;
            if (matches_direction(grid, len, row, col, LEFT, NONEV, target))
                count++;
            if (matches_direction(grid, len, row, col, NONE, UP, target))
                count++;
            if (matches_direction(grid, len, row, col, NONE, DOWN, target))
                count++;
            if (matches_direction(grid, len, row, col, RIGHT, UP, target))
                count++;
            if (matches_direction(grid, len, row, col, RIGHT, DOWN, target))
                count++;
            if (matches_direction(grid, len, row, col, LEFT, UP, target))
                count++;
            if (matches_direction(grid, len, row, col, LEFT, DOWN, target))
                count++;
        }
    }
    return count;
}

// Part 2
int count_mas_x(char **grid, const size_t len)
{
    int count = 0;
    static const char *target = "MAS";
    const size_t half_len = (size_t) strlen(target) / 2;

    for (size_t row = 0; row < len; row++)
    {
        for (size_t col = 0; col < len; col++)
        {
            if (
                (grid[row][col] != 'A') ||
                (row < half_len || row > (len - 1 - half_len)) ||
                (col < half_len || col > (len - 1 - half_len))
            )
                continue;
            
            bool down_right = matches_direction(grid, len, row - 1, col - 1, RIGHT, DOWN, target);
            bool down_left = matches_direction(grid, len, row - 1, col + 1, LEFT, DOWN, target);
            bool up_right = matches_direction(grid, len, row + 1, col - 1, RIGHT, UP, target);
            bool up_left = matches_direction(grid, len, row + 1, col + 1, LEFT, UP, target);

            if (
                (down_right && up_right) || 
                (down_right && down_left) || 
                (up_left && up_right) || 
                (up_left && down_left)
            )
                count++;
        }
    }
    return count;
}

size_t get_grid_size(FILE *f)
{
    if (f == NULL)
        return 0;
    char linebuff[LINESIZE];
    if (fgets(linebuff, LINESIZE, f) == NULL)
        return 0;
    
    size_t count = 0;
    for (size_t i = 0; i < LINESIZE; i++)
    {
        if(linebuff[i] == '\n')
            break;
        else
        {
            count++;
        }
    }
    rewind(f);
    return count;
}

char **create_grid(const size_t size)
{
    char **grid = malloc(size * sizeof(char *));
    for (size_t i = 0; i < size; i++)
    {
        grid[i] = malloc(size * sizeof(char));
    }
    return grid;
}

void free_grid(char **grid, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        free(grid[i]);
    }
    free(grid);
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
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return 1;
    }

    const size_t grid_len = get_grid_size(infile);
    const size_t linesize = grid_len + 2;
    char **grid = create_grid(grid_len);
    char *linebuff = malloc((linesize) * sizeof(char));
    size_t row = 0;
    while (fgets(linebuff, linesize, infile) != NULL)
    {
        for (size_t i = 0; i < grid_len; i++)
        {
            grid[row][i] = linebuff[i];
        }
        row++;
    }
    fclose(infile);
    free(linebuff);
    int xmas_count = count_xmas(grid, grid_len);
    int mas_x_count = count_mas_x(grid, grid_len);
    free_grid(grid, grid_len);
    printf("XMAS count (Part 1): %d\n", xmas_count);
    printf("MAS-X count (Part 2): %d\n", mas_x_count);
    return 0;
}
