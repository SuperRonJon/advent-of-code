#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 128

size_t count_lines(FILE *f)
{
	char linebuff[LINESIZE];
	size_t count = 0;
	while (fgets(linebuff, LINESIZE, f) != NULL)
	{
		count++;
	}
	clearerr(f);
	rewind(f);
	return count;
}

struct tupl
{
	int first;
	int second;
};

struct tupl split_nums(char *s)
{
	int found_first = 0;
	size_t slen = strlen(s);
	char *curr = malloc(slen);
	size_t curri = 0;
	int first = 0;
	int second = 0;
	for (size_t i = 0; i < slen; i++)
	{
		if (s[i] != ' ' && s[i] != '\n')
		{
			curr[curri++] = s[i];
		}
		else if (found_first == 0)
		{
			curr[curri] = '\0';
			first = strtol(curr, NULL, 10);
			curri = 0;
			found_first = 1;
		}
	}
	curr[curri] = '\0';
	second = strtol(curr, NULL, 10);
	struct tupl t = {first, second};
	free(curr);
	return t;
}

int cmp_ints(const void *left, const void *right)
{
	const int first = *(int *)left;
	const int second = *(int *)right;
	if (first == second)
		return 0;
	if (first < second)
		return -1;
	return 1;
}

int get_total_distance(const int *l1, const int *l2, const size_t len)
{
	int total = 0;
	for (size_t i = 0; i < len; i++)
	{
		int dist = l1[i] - l2[i];
		if (dist < 0)
			dist *= -1;
		total += dist;
	}
	return total;
}

int occurrences_in_list(const int *list, const size_t len, const int value)
{
	int count = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (list[i] == value)
			count++;
	}
	return count;
}

int get_total_similarity(const int *l1, const int *l2, const size_t len)
{
	int total = 0;
	for (size_t i = 0; i < len; i++)
	{
		const int occurances = occurrences_in_list(l2, len, l1[i]);
		total += l1[i] * occurances;
	}
	return total;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		fputs("No input file given\n", stderr);
		return 1;
	}

	FILE *infile = fopen(argv[1], "r");
	size_t line_count = count_lines(infile);
	int *list1 = malloc(line_count * sizeof(int));
	int *list2 = malloc(line_count * sizeof(int));
	size_t i = 0;
	char linebuff[LINESIZE];
	while (fgets(linebuff, LINESIZE, infile) != NULL)
	{
		struct tupl t = split_nums(linebuff);
		list1[i] = t.first;
		list2[i] = t.second;
		i++;
	}
	qsort(list1, line_count, sizeof(int), cmp_ints);
	qsort(list2, line_count, sizeof(int), cmp_ints);
	int dist = get_total_distance(list1, list2, line_count);
	int similarity = get_total_similarity(list1, list2, line_count);
	printf("Total distance (part1): %d\n", dist);
	printf("Total similarity (part2): %d\n", similarity);
	free(list1);
	free(list2);
	return 0;
}
