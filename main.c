#include "tree.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include "tree.h"


#define LINE_BUF_SIZE 200
#define COMM_ADD_NODE  "addUser"
#define COMM_DEL_USER  "delUser"
#define COMM_ADD_ITEM  "addMovie"
#define COMM_DEL_ITEM  "delMovie"
#define COMM_QUERY  "marathon"
#define DOUBLE_ARG_REGEX "^(addUser|delMovie|addMovie|marathon) [1-9][0-9]* [1-9][0-9]*$"
#define SINGLE_ARG_REGEX "^(delUser) [1-9][0-9]*$"

typedef struct command_t {
	char comm_name[LINE_BUF_SIZE];
	char arg1[LINE_BUF_SIZE];
	char arg2[LINE_BUF_SIZE];
} Command;

Command *Command_new() {
	Command *c = malloc(sizeof(Command));
	c->arg1[0] = '\0';
	c->arg2[0] = '\0';
	c->comm_name[0] = '\0';
	return c;
}

bool Command_matches_pattern(const char *string, char *pattern) {
	int status;
	regex_t re;
	if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
		return false;
	}
	status = regexec(&re, string, (size_t) 0, NULL, 0);
	regfree(&re);
	if (status != 0) {
		return false;
	}
	return true;
}

bool Command_is_valid_line(const char *line) {
	return Command_matches_pattern(line, SINGLE_ARG_REGEX) || Command_matches_pattern(line, DOUBLE_ARG_REGEX);
}

bool Command_is_single_arg(char *name) {
	return strcmp(name, COMM_DEL_USER) == 0;
}


typedef struct parser_t {
	char buff;
	bool has_next;
	char line_buff[LINE_BUF_SIZE];
	Command *com_buff;
} Parser;

bool Parser_read(Parser *self) {
	if (!self->has_next) return false;
	char c = (char) getchar();
	self->buff = c;
	self->has_next = c != EOF;
	return self->has_next;
}


Parser *Parser_new() {
	Parser *p = malloc(sizeof(Parser));
	p->has_next = true;
	p->line_buff[0] = '\0';
	p->com_buff = Command_new();
	Parser_read(p);
	return p;
}


void Parser_skip_line(Parser *self) {
	while (self->has_next && self->buff != '\n')
		Parser_read(self);
	Parser_read(self); //Read the first char of the next line
}

void Parser_read_line(Parser *self, char *buff) {
	int i = 0;
	while (i < LINE_BUF_SIZE - 1 && self->buff != '\n' && self->has_next) {
		buff[i++] = self->buff;
		Parser_read(self);
	}
	buff[i] = '\0';
	Parser_skip_line(self);
}

void Parser_read_word(const char *source, char *target, int *source_i) {
	int i = 0;
	while (source[*source_i] != '\0' && source[*source_i] != ' ') {
		target[i++] = source[*source_i++];
	}
	target[i] = '\0';
}

bool Parser_read_and_validate(Parser *self) {
	Parser_read_line(self, self->line_buff);
	if (!Command_is_valid_line(self->line_buff))
		return false;
	int line_index = 0;
	Parser_read_word(self->line_buff, self->com_buff->comm_name, &line_index);
	++line_index;
	Parser_read_word(self->line_buff, self->com_buff->arg1, &line_index);
	++line_index;
	if (!Command_is_single_arg(self->com_buff->comm_name))
		Parser_read_word(self->line_buff, self->com_buff->arg2, &line_index);
}

void Parser_print_status(bool ok) {
	printf(ok ? "OK\n" : "ERROR\n");
}

void Parser_print_query_result(int res[], int limit) {
	for (int i = 0; i < limit && res[i] != -1; ++i) {
		printf(i == 0 ? "%d" : " %d", res[i]);
	}
	printf("\n");
}


void Parser_handle_next_line(Parser *self, struct Tree *t) {

	if (self->buff == '#' || self->buff == '\n') { //Ignored lines
		Parser_skip_line(self);
		return;
	}

	if (!Parser_read_and_validate(self)) {
		Parser_print_status(false);
		return;
	}

	Command *com = self->com_buff;
	if (strcmp(com->comm_name, COMM_ADD_NODE) == 0) {
		Parser_print_status(Tree_add_node(t, atoi(com->arg1), atoi(com->arg2)));
	} else if (strcmp(com->comm_name, COMM_ADD_ITEM) == 0) {
		Parser_print_status(Tree_add_item(t, atoi(com->arg1), atoi(com->arg2)));
	} else if (strcmp(com->comm_name, COMM_DEL_ITEM) == 0) {
		Parser_print_status(Tree_remove_item(t, atoi(com->arg1), atoi(com->arg2)));
	} else if (strcmp(com->comm_name, COMM_DEL_USER) == 0) {
		Parser_print_status(Tree_remove_node(t, atoi(com->arg1)));
	} else if (strcmp(com->comm_name, COMM_QUERY) == 0) {
		int limit = atoi(com->arg2);
		int *ans = malloc(sizeof(int) * limit);
		if (!Tree_extract_max(t, atoi(com->arg1), limit, ans)) {
			Parser_print_status(false);
		} else {
			if (ans[0] == -1) {
				printf("NONE\n");
			} else {
				Parser_print_query_result(ans, limit);
			}
		}
	} else {
		printf("Unhandled command name %s", com->comm_name);
		exit(1);
	}

}


int main() {
	struct Tree *t = Tree_new();
	Parser *p = Parser_new();
	while (p->has_next) {
		Parser_handle_next_line(p, t);
	}
	return 0;
}

