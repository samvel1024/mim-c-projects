#include "tree.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define BUF_SIZE 100

typedef enum {
	ADD_NODE,
	REMOVE_NODE,
	ADD_ITEM,
	REMOVE_ITEM,
	QUERY
} CommandType;

const char *COMM_ADD_USER = "addUser";
const char *COMM_DEL_USER = "delUser";
const char *COMM_ADD_ITEM = "addMovie";
const char *COMM_REMOVE_ITEM = "delMovie";
const char *COMM_QUERY = "marathon";


typedef struct command_t {
	char comm_name[BUF_SIZE];
	char arg1[BUF_SIZE];
	char arg2[BUF_SIZE];
} Command;


typedef struct parser_t {
	char buff;
	bool has_next;
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
	Parser_read(p);
	return p;
}

void Parser_read_word(Parser *self, char *buff, char terminator) {
	int i = 0;
	while (self->buff != terminator && self->has_next) {
		buff[i++] = self->buff;
		Parser_read(self);
	}
}


void Parser_skip_line(Parser *self) {
	while (self->has_next && self->buff != '/n')
		Parser_read(self);
}

bool Parser_handle_next_command(Parser *self, Command *com) {

	while (self->has_next && self->buff == '\n')
		Parser_read(self);

	if (self->buff == '#') {
		Parser_skip_line(self);
	} else {
		

	}

}


int main() {
	Parser *p = Parser_new();
	while (p->has_next) {

	}
}