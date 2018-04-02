#include "tree.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <limits.h>
#include "tree.h"


/******************(to enable MODE_DEBUG flag should be passed)*******************************/
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
/****************** End debug region *********************************************/


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
	int row; //debug
	int col; //debug
	char buff; // upcoming char
	bool has_next; // EOF reached or not
	char line_buff[LINE_BUF_SIZE]; // used to read the full line
	Command *com_buff; // used to parse the store the current command
	regex_t matcher; // regex matcher
} Parser;

/**
 * Reads the next char in stdin
 */
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

/**
 * Constructor
 */
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

/**
 * Checks for syntax errors in the line
 */
bool Parser_is_valid_line(Parser *parser) {
	int status = regexec(&parser->matcher, parser->line_buff, (size_t) 0, NULL, 0);
	return status == 0;
}

/**
 * Destructor
 */
void Parser_free(Parser *self) {
	free(self->com_buff);
	free(self);
	regfree(&self->matcher);
}

/**
 * Used for skipping empty lines, comments, and long invalid lines
 */
void Parser_skip_line(Parser *self) {
	while (self->has_next && self->buff != '\n')
		Parser_read(self);
	Parser_read(self); //Read the first char of the next line
}

/**
 * Reads up to the end of th eline and stores in self->line_buff
 */
void Parser_read_line(Parser *self) {
	int i = 0;
	while (i <= LINE_BUF_SIZE - 1 && self->buff != '\n' && self->has_next) {
		self->line_buff[i++] = self->buff;
		Parser_read(self);
	}
	self->line_buff[i] = '\0';
	if (self->buff != '\n') //the line is too long and redundant
		Parser_skip_line(self);
}

/**
 * Used for breaking down self->line_buff into separate words with delimeter ' '
 */
void Parser_read_word(Parser *self, char *target, int *source_i) {
	int i = 0;
	while (self->line_buff[*source_i] != '\0' && self->line_buff[*source_i] != ' ') {
		target[i++] = self->line_buff[(*source_i)++];
	}
	target[i] = '\0';
}

/**
 * @return
 * true if line was valid
 */
bool Parser_read_and_validate(Parser *self) {
	Parser_read_line(self);
	if (!Parser_is_valid_line(self))
		return false;
	int line_index = 0;
	Parser_read_word(self, self->com_buff->comm_name, &line_index);
	++line_index;
	Parser_read_word(self, self->com_buff->arg1, &line_index);
	++line_index;
	if (!Command_is_single_arg(self->com_buff->comm_name))
		Parser_read_word(self, self->com_buff->arg2, &line_index);
	return true;
}

void Parser_print_status(bool ok) {
	if (ok)
		printf("OK\n");
	else
		fprintf(stderr, "ERROR\n");
}

void Parser_print_query_result(int res[], int limit) {
	if (res[0] == EMPTY_ITEM) {
		printf("NONE\n");
		return;
	}
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
		// At least one elemnt is needed to denote empty result set
		int *ans = malloc(sizeof(int) * ((limit > 0) ? limit : 1));
		ans[0] = EMPTY_ITEM;
		if (!Tree_extract_max(t, atoi(com->arg1), limit, ans)) {
			Parser_print_status(false);
		} else {
			Parser_print_query_result(ans, limit);
		}
		free(ans);
	} else { //Should no happen
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


int main() {
	struct Tree *t = Tree_new();
	Parser *p = Parser_new();
	while (p->has_next) {
		Parser_handle_next_line(p, t);
	}
	Parser_free(p);
	Tree_free(t);
}
