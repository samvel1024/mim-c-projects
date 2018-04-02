#include "tree.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include "tree.h"

//#define MODE_DEBUG
#ifdef MODE_DEBUG
#define log printf
#define DECORATE_STDOUT true
#else
#define log fake_printf
#define DECORATE_STDOUT false
#endif
#define UNUSED(x) (void)(x)


void fake_printf(const char *format, ...) {
	UNUSED(format);
}

#define LINE_BUF_SIZE 100
#define COMM_ADD_NODE "addUser"
#define COMM_DEL_USER "delUser"
#define COMM_ADD_ITEM "addMovie"
#define COMM_DEL_ITEM "delMovie"
#define COMM_QUERY "marathon"
#define REGEX "^((addUser|delMovie|addMovie|marathon) ([1-9][0-9]*|0) ([1-9][0-9]*|0))|((delUser) ([1-9][0-9]*|0))$"

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

bool Command_is_single_arg(char *name) {
	return strcmp(name, COMM_DEL_USER) == 0;
}


typedef struct parser_t {
	int row;
	int col;
	char buff;
	bool has_next;
	char line_buff[LINE_BUF_SIZE];
	Command *com_buff;
	regex_t matcher;
} Parser;

bool Parser_read(Parser *self) {
	if (!self->has_next) return false;
	if (self->buff == '\n')
		self->row++;
	char c = (char) getchar();
	self->col++;
	self->buff = c;
	self->has_next = c != EOF;
	return self->has_next;
}


Parser *Parser_new() {
	Parser *p = malloc(sizeof(Parser));
	p->has_next = true;
	p->line_buff[0] = '\0';
	p->com_buff = Command_new();
	p->buff = '\n';
	p->row = 0;
	p->col = 0;
	regcomp(&p->matcher, REGEX, REG_EXTENDED);
	Parser_read(p);
	return p;
}

bool Parser_is_valid_line(Parser *parser, const char *string) {
	int status = regexec(&parser->matcher, string, (size_t) 0, NULL, 0);
	return status == 0;
}


void Parser_free(Parser *self) {
	free(self->com_buff);
	free(self);
	regfree(&self->matcher);
}


void Parser_skip_line(Parser *self) {
	while (self->has_next && self->buff != '\n')
		Parser_read(self);
	Parser_read(self); //Read the first char of the next line
}

void Parser_read_line(Parser *self, char *buff) {
	int i = 0;
	while (i <= LINE_BUF_SIZE - 1 && self->buff != '\n' && self->has_next) {
		buff[i++] = self->buff;
		Parser_read(self);
	}
	buff[i] = '\0';
	if (self->buff != '\n')
		Parser_skip_line(self);
}

void Parser_read_word(const char *source, char *target, int *source_i) {
	int i = 0;
	while (source[*source_i] != '\0' && source[*source_i] != ' ') {
		target[i++] = source[(*source_i)++];
	}
	target[i] = '\0';
}

bool Parser_read_and_validate(Parser *self) {
	Parser_read_line(self, self->line_buff);
	if (!Parser_is_valid_line(self, self->line_buff))
		return false;
	int line_index = 0;
	Parser_read_word(self->line_buff, self->com_buff->comm_name, &line_index);
	++line_index;
	Parser_read_word(self->line_buff, self->com_buff->arg1, &line_index);
	++line_index;
	if (!Command_is_single_arg(self->com_buff->comm_name))
		Parser_read_word(self->line_buff, self->com_buff->arg2, &line_index);
	return true;
}

void Parser_print_status(bool ok) {
	if (ok)
		printf("OK\n");
	else
		fprintf(stderr, "ERROR\n");
}

void Parser_print_query_result(int res[], int limit) {
	for (int i = 0; i < limit && res[i] != -1; ++i) {
		printf(i == 0 ? "%d" : " %d", res[i]);
	}
	printf("\n");
}

void Parser_execute_command(Command *com, struct Tree *t) {
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
		int *ans = malloc(sizeof(int) * ((limit > 0) ? limit : 1));
		ans[0] = EMPTY_ITEM;
		if (!Tree_extract_max(t, atoi(com->arg1), limit, ans)) {
			Parser_print_status(false);
		} else {
			if (ans[0] == EMPTY_ITEM) {
				printf("NONE\n");
			} else {
				Parser_print_query_result(ans, limit);
			}
		}
		free(ans);
	} else {
		printf("Unhandled command name %s\n", com->comm_name);
		exit(1);
	}
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

	long arg1 = strtol(com->arg1, 0, 10);
	long arg2 = Command_is_single_arg(com->comm_name) ? 0 : strtol(com->arg2, 0, 10);

	if (arg1 > INT_MAX || arg2 > INT_MAX) {
		Parser_print_status(false);
		return;
	}

	log(Command_is_single_arg(com->comm_name) ? "Parsed command: %s %s\n" : "Parsed command: %s %s %s\n", com->comm_name,
	    com->arg1,
	    com->arg2);

	Parser_execute_command(com, t);

}


void test() {
	int ans[100];
	struct Tree *t = Tree_new();
	Tree_extract_max(t, 0, 2, ans);
	Tree_add_item(t, 0, 1337);
	Tree_extract_max(t, 0, 2, ans);
	Tree_add_item(t, 0, 1410);
	Tree_add_node(t, 0, 1);
	Tree_add_node(t, 1, 2);
	Tree_add_item(t, 1, 1815);
	Tree_add_item(t, 2, 1683);
	Tree_add_item(t, 2, 1525);
	Tree_extract_max(t, 0, 2, ans);

	Tree_remove_node(t, 1);
	Tree_remove_item(t, 2, 1525);
	Tree_extract_max(t, 0, 2, ans);
	Tree_add_node(t, 2, 1);
	Tree_add_item(t, 1, 2018);
	Tree_extract_max(t, 0, 3, ans);
	Tree_free(t);
}

void test_free() {
	int ans[100];
	struct Tree *t = Tree_new();
	Tree_add_node(t, 0, 1);
	Tree_add_node(t, 0, 2);
	Tree_add_node(t, 0, 3);
	Tree_free(t);
}


void test_stdin() {
	struct Tree *t = Tree_new();
	Parser *p = Parser_new();
	while (p->has_next) {
		Parser_handle_next_line(p, t);
	}
	Parser_free(p);
	Tree_free(t);
}

int main() {

	bool TEST = false;
	if (TEST) test_free();
	else test_stdin();
//	printf("%d", Command_is_valid_line("addMovie 141 14901"));

}

