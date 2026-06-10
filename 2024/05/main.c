#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LINESIZE
#define LINESIZE 256
#endif

struct counts
{
	size_t rules;
	size_t updates;
};

struct rule
{
	int first;
	int second;
};

struct update
{
	int *pages;
	size_t len;
};

struct rule create_rule(const char *const s)
{
	struct rule res;
	sscanf(s, "%d|%d", &res.first, &res.second);
	return res;
}

struct update create_update(const char *const s)
{
	size_t page_count = 0;
	const size_t len = strlen(s);
	for (size_t i = 0; i < len; i++)
	{
		if (s[i] == ',' || s[i] == '\n')
			page_count++;
	}

	struct update res;
	res.pages = malloc(page_count * sizeof(int));
	res.len = page_count;

	char pagebuff[16];
	size_t buff_i = 0;
	size_t page_i = 0;
	for (size_t i = 0; i < len; i++)
	{
		if (s[i] == ',' || s[i] == '\n')
		{
			pagebuff[buff_i] = '\0';
			buff_i = 0;
			res.pages[page_i++] = strtol(pagebuff, NULL, 10);
		}
		else if (s[i] >= '0' && s[i] <= '9')
		{
			pagebuff[buff_i++] = s[i];
		}
	}
	return res;
}

struct counts get_counts(FILE *infile)
{
	size_t rule_count = 0;
	size_t update_count = 0;
	bool get_rules = true;
	char linebuff[LINESIZE];
	
	while (fgets(linebuff, sizeof(linebuff), infile) != NULL)
	{
		if (linebuff[0] == '\n')
		{
			get_rules = false;
			continue;
		}
		if (get_rules)
			rule_count++;
		else
			update_count++;
	}
	rewind(infile);
	struct counts result = {rule_count, update_count};
	return result;
}

void free_updates(struct update *updates, const size_t len)
{
	for (size_t i = 0; i < len; i++)
	{
		free(updates[i].pages);
	}
	free(updates);
}

bool rule_is_followed(const struct update *const updates, const struct rule *const r)
{
	size_t first_index = 0;
	size_t second_index = SIZE_MAX;

	for (size_t i = 0; i < updates->len; i++)
	{
		if (updates->pages[i] == r->first)
			first_index = i;
		if (updates->pages[i] == r->second)
			second_index = i;
	}
	return first_index <= second_index;
}

bool update_is_valid(const struct update *const updates, const struct rule *const rules, const size_t rules_len)
{
	for (size_t i = 0; i < rules_len; i++)
	{
		if (!rule_is_followed(updates, &rules[i]))
			return false;
	}
	return true;
}

void fix_update_rule(struct update *updates, const struct rule *const r)
{
	if (updates->len == 0)
		return;
	size_t first_index = 0;
	size_t second_index = updates->len - 1;
	for (size_t i = 0; i < updates->len; i++)
	{
		if (updates->pages[i] == r->first)
			first_index = i;
		if (updates->pages[i] == r->second)
			second_index = i;
	}
	int tmp = updates->pages[first_index];
	updates->pages[first_index] = updates->pages[second_index];
	updates->pages[second_index] = tmp;
}

void fix_update(struct update *updates, const struct rule *const rules, const size_t rules_len)
{
	bool finished = false;
	while (!finished)
	{
		finished = true;
		for (size_t i = 0; i < rules_len; i++)
		{
			if (!rule_is_followed(updates, &rules[i]))
			{
				fix_update_rule(updates, &rules[i]);
				finished = false;
				break;
			}
		}
	}
}

int center_page(const struct update *const updates)
{
	size_t half_i = updates->len / 2;
	return updates->pages[half_i];
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
		perror("Error opening input file");
		return 1;
	}

	// Parse rules and update sets from input file
	const struct counts c = get_counts(infile);
	const size_t rules_len = c.rules;
	const size_t updates_len = c.updates;

	struct rule *rules = malloc(rules_len * sizeof(struct rule));
	size_t rules_i = 0;
	struct update *updates = malloc(updates_len * sizeof(struct update));
	size_t updates_i = 0;

	bool get_rules = true;
	char linebuff[LINESIZE];

	while (fgets(linebuff, sizeof(linebuff), infile) != NULL)
	{
		if (linebuff[0] == '\n')
		{
			get_rules = false;
			continue;
		}
		if (get_rules && rules_i < rules_len)
		{
			rules[rules_i++] = create_rule(linebuff);
		}
		else if (!get_rules && updates_i < updates_len)
		{
			updates[updates_i++] = create_update(linebuff);
		}
	}
	fclose(infile);
	
	// Check update sets against rules
	int sum = 0;
	int invalid_sum = 0;
	for (size_t i = 0; i < updates_len; i++)
	{
		// Check update follows all rules
		if (update_is_valid(&updates[i], rules, rules_len))
		{
			// If valid, add center page value to sum
			sum += center_page(&updates[i]);
		}
		else
		{
			fix_update(&updates[i], rules, rules_len);
			invalid_sum += center_page(&updates[i]);
		}
	}
	printf("Sum of center page of valid updates (part 1): %d\n", sum);
	printf("Sum of center page of fixed invalid updates (part 2): %d\n", invalid_sum);
	free(rules);
	free_updates(updates, updates_len);
	return 0;
}
