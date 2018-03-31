#include "tree.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include "tree.h"


#define LINE_BUF_SIZE 500
#define COM_BUF_SIZE 100
#define COMM_ADD_NODE  "addUser"
#define COMM_DEL_USER  "delUser"
#define COMM_ADD_ITEM  "addMovie"
#define COMM_DEL_ITEM  "delMovie"
#define COMM_QUERY  "marathon"
#define COMM_SIZE 5
const char *COMMAND_NAMES[] = {COMM_ADD_ITEM, COMM_ADD_NODE, COMM_DEL_USER, COMM_DEL_ITEM, COMM_QUERY};

typedef struct command_t {
	char comm_name[COM_BUF_SIZE];
	char arg1[COM_BUF_SIZE];
	char arg2[COM_BUF_SIZE];
} Command;

Command *Command_new() {
	Command *c = malloc(sizeof(Command));
	c->arg1[0] = '\0';
	c->arg2[0] = '\0';
	c->comm_name[0] = '\0';
	return c;
}

bool Command_is_valid_name(char *str) {
	for (int i = 0; i < COMM_SIZE; ++i) {
		if (strcmp(COMMAND_NAMES[i], str) == 0)
			return true;
	}
	return false;
}

bool Command_is_valid_arg(char *str) {
	int num = atoi(str);
	if (num == 0 && !strcmp(str, "0") && !strcmp(str, "+0") && !strcmp(str, "-0")) {
		return false;
	}
	return true;
}

bool Command_is_single_arg(char *name) {
	return strcmp(name, COMM_QUERY) == 0;
}


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
	p->has_next = true;
	Parser_read(p);
	return p;
}

void Parser_read_line(Parser *self, char *buff) {
	int i = 0;
	while (self->buff != '\n' && self->has_next) {
		buff[i++] = self->buff;
		Parser_read(self);
	}
	buff[i] = '\0';
}

bool Parser_read_word(const char *source, char *target, int *start) {
	int i = 0;
	while (source[*start] != '\0' && source[*start] != ' ') {
		target[i++] = source[*start++];
	}
	return source[*start] != '\0';
}

bool Parser_is_endl(Parser *self) {
	return self->buff == '\n';
}


void Parser_skip_line(Parser *self) {
	while (self->has_next && self->buff != '\n')
		Parser_read(self);
	Parser_read(self); //Read the first char of the next line
}

void Parser_print_status(bool ok) {
	printf(ok ? "OK\n" : "ERROR\n");
}

bool Parser_read_and_validate(Parser *self, Command *com, char *buf) {
	Parser_read_line(self, buf);
//
//	int line_index = 0;
//	if (!Parser_read_word(buf, com->comm_name, &line_index) || !){
//		Parser_print_status(false);
//	}


}



void Parser_handle_next_line(Parser *self, struct Tree *t, Command *com) {

	if (self->buff == '#' || self->buff == '\n') { //Ignored lines
		Parser_skip_line(self);
		return;
	}

//	if (!Parser_read_and_validate(self, com)) {
//		printf("ERROR\n");
//		return;
//	}

	if (strcmp(com->comm_name, COMM_ADD_NODE) == 0) {
		Parser_print_status(Tree_add_node(t, atoi(com->arg1), atoi(com->arg2)));
	} else if (strcmp(com->comm_name, COMM_ADD_ITEM) == 0) {
		Parser_print_status(Tree_add_item(t, atoi(com->arg1), atoi(com->arg2)));
	} else if (strcmp(com->comm_name, COMM_DEL_ITEM) == 0) {
		Parser_print_status(Tree_remove_item(t, atoi(com->arg1), atoi(com->arg2)));
	} else if (strcmp(com->comm_name, COMM_DEL_USER) == 0) {
		Parser_print_status(Tree_remove_node(t, atoi(com->arg1)));
	} else if (strcmp(com->comm_name, COMM_QUERY) == 0) {

	} else {
		printf("Unhandled command name %s", com->comm_name);
		exit(1);
	}

}

//
//int main() {
//	struct Tree *t = Tree_new();
//	Command *buf_command = Command_new();
//	Parser *p = Parser_new();
//	while (p->has_next) {
//		Parser_handle_next_line(p, t, buf_command);
//	}
//	return 0;
//}

int main () {
	regex_t regex;
	int reti;
	char msgbuf[100];

/* Compile regular expression */
	reti = regcomp(&regex, "^a[[:alnum:]]", 0);
	if (reti) {
		fprintf(stderr, "Could not compile regex\n");
		exit(1);
	}

/* Execute regular expression */
	reti = regexec(&regex, "abc", 0, NULL, 0);
	if (!reti) {
		puts("Match");
	}
	else if (reti == REG_NOMATCH) {
		puts("No match");
	}
	else {
		regerror(reti, &regex, msgbuf, sizeof(msgbuf));
		fprintf(stderr, "Regex match failed: %s\n", msgbuf);
		exit(1);
	}

/* Free memory allocated to the pattern buffer by regcomp() */
	regfree(&regex);

	return(0);
}