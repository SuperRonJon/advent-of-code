#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t get_next_target_start(const char *const str, size_t start, const char *const target)
{
	size_t target_len = strlen(target);
	size_t str_len = strlen(str);

	for (size_t i = start; i < str_len; i++)
	{
		bool match = true;
		for (size_t j = 0; j < target_len; j++)
		{
			if (str[i + j] != target[j])
			{
				match = false;
				break;
			}
		}
		if (match)
			return i + target_len;
	}
	return SIZE_MAX;
}

size_t get_next_mul_start(const char *const str, size_t start)
{
	static const char *target = "mul(";
	return get_next_target_start(str, start, target);
}

size_t get_next_do_start(const char *const str, size_t start)
{
	static const char *target = "do()";
	return get_next_target_start(str, start, target);
}

size_t get_next_dont_start(const char *const str, size_t start)
{
	static const char *target = "don't()";
	return get_next_target_start(str, start, target);
}

enum command_type
{
	MUL,
	DO,
	DONT
};

struct command
{
	enum command_type type;
	size_t index;
};

struct command get_next_command(const char *const str, size_t start, bool enabled)
{
	struct command result;
	size_t do_start = get_next_do_start(str, start);
	if (!enabled)
	{
		result.type = DO;
		result.index = do_start;
		return result;
	}
	size_t next_mul = get_next_mul_start(str, start);
	size_t dont_start = get_next_dont_start(str, start);

	if (next_mul <= do_start && next_mul <= dont_start)
	{
		result.type = MUL;
		result.index = next_mul;
	}
	else if (do_start <= next_mul && do_start <= dont_start)
	{
		result.type = DO;
		result.index = do_start;
	}
	else
	{
		result.type = DONT;
		result.index = dont_start;
	}
	return result;
}

int get_line_sum(const char *const str, bool *enabled, bool can_disable)
{
	int sum = 0;
	size_t len = strlen(str);
	struct command next_command = get_next_command(str, 0, *enabled);
	size_t cur = next_command.index;

	char curr_str[4];
	size_t str_i = 0;
	bool first = true;
	bool valid = true;

	int val1 = 0;
	int val2 = 0;

	while (cur < len)
	{
		while(cur < len && valid)
		{
			if (next_command.type == DO)
			{
				*enabled = true;
				valid = false;
				break;
			}
			else if (next_command.type == DONT && can_disable)
			{
				*enabled = false;
				valid = false;
				break;
			}
			else if (next_command.type == MUL && !*enabled)
			{
				valid = false;
				break;
			}
			if ('0' <= str[cur] && str[cur] <= '9')
			{
				curr_str[str_i++] = str[cur++];
			}
			else if (str[cur] == ',' && first)
			{
				first = false;
				curr_str[str_i] = '\0';
				str_i = 0;
				val1 = strtol(curr_str, NULL, 10);
				curr_str[0] = '\0';
				cur++;
			}
			else if (str[cur] == ')' && !first && strlen(curr_str) != 0)
			{
				curr_str[str_i] = '\0';
				str_i = 0;
				val2 = strtol(curr_str, NULL, 10);
				break;
			}
			else {
				valid = false;
				str_i = 0;
			}
		}
		if (valid)
		{
			sum += (val1 * val2);
		}
		valid = true;
		first = true;
		next_command = get_next_command(str, cur, *enabled);
		cur = next_command.index;
	}
	return sum;
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

	const size_t line_length = 4096;
	char *linebuff = malloc(line_length);
	int part1_sum = 0;
	int part2_sum = 0;
	bool enabled1 = true;
	bool enabled2 = true;
	while (fgets(linebuff, line_length, infile))
	{
		part1_sum += get_line_sum(linebuff, &enabled1, false);
		part2_sum += get_line_sum(linebuff, &enabled2, true);
	}
	fclose(infile);
	free (linebuff);
	printf("Multiplication result (part1): %d\n", part1_sum);
	printf("Multiplication result with conditionals (part 2): %d\n", part2_sum);
	return 0;
}
