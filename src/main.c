#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/param.h>

#include "buf.h"

/**
 * parser reads the internal buffer char by char. It refils by copying in the
 * entire input buffer.
 */
struct parser{
	// Input buffer
	struct feng_buffer *input;
	// Internal buffer
    struct feng_buffer buf;
	// Position in our buffer
	size_t pos;
};

void
create_parser(struct parser *parser, struct feng_buffer *input) {
	parser->input = input;
}

/**
 * gets the next char from the buffer. Copies in the input buf as needed.
 * If there are no remaining chars then char will be null. False is returned
 * if there is an issue.
 */
bool
parser_next(struct parser *parser, char *c) {
	*c = '\0';
	if (parser->buf.len == parser->pos) {
		parser->pos = parser->buf.len = 0;
		if (parser->input->len == 0) return true;

		if (!feng_buffer_append(&parser->buf, parser->input->data, parser->input->len)) return false;
		parser->input->len = 0;
	}

	*c = parser->buf.data[parser->pos++];
	return true;
}

struct state{
	struct parser parser;
	// Position in the arena
    size_t pos;
	// Size of the trailing arena
    size_t arena_size;
	// Arena array of n size
    unsigned char arena[];
};

struct state *
state_initialize(struct feng_buffer *input, size_t arena_size) {
    struct state *state;
    if ((state = calloc(sizeof(state) + arena_size, 1)) == NULL) return NULL;

    create_parser(&state->parser, input);
    state->arena_size = arena_size;

    return state;
}

void
state_deinit(struct state *state) {
    free(state);
}

/**
 * Steps forward in the state of the program. It will return 1 if it steps
 * forward, 0 if no steps are made, and < 0 if there is an error.
 */
int
state_step(struct state *state) {
    char inst;

	// TODO this should be an error
	if (!parser_next(&state->parser, &inst)) return -1;

    switch (inst) {
    case '\0':
        return 0;
    case '>' :
        if (state->pos < state->arena_size-1) state->pos++;
        else exit(1); // TODO error handling
        break;
    case '<' :
        if (state->pos > 0) state->pos--;
        else exit(1); // TODO error handling
        break;
    case '+' :
        state->arena[state->pos]++;
        break;
    case '-' :
        state->arena[state->pos]--;
        break;
	}

	return 1;
}

int
execute_buffer(struct state *state) {
    int buf_len = state->parser.buf.len;
    int executed_steps = buf_len - state->parser.pos;
    while (state_step(state) > 0);
    return executed_steps;
}

int
excecute_n(struct state *state, int n) {
    int buf_pos = state->parser.pos;
    int executed_steps = MIN(n, state->parser.buf.len - buf_pos);
    for (int step = buf_pos; step < buf_pos+executed_steps; step++)
        state_step(state);
    return executed_steps;
}

void
state_print(struct state *state) {

    // TODO use an input or const variable to replace the 5
    if (state->pos > 5) fprintf(stderr, "... ");
	for (int idx = MAX((ssize_t)state->pos - 5, 0); idx < state->pos; idx++)
			fprintf(stderr, "%d ", state->arena[idx]);

	fprintf(stderr, "[%d]", state->arena[state->pos]);

    for (int idx = state->pos+1; idx <= MIN(state->arena_size-1, state->pos+5); idx++)
			fprintf(stdout, " %d", state->arena[idx]);

	fprintf(stdout, "\n");
}

int
main(int argc, char** argv) {
	struct feng_buffer input = {0};
	feng_buffer_ensure_cap(&input, BUFSIZ);
    struct state *state = state_initialize(&input, 1024);

	ssize_t n;
	size_t buf_size = BUFSIZ;

    while((n = getline(&input.data, &input.cap, stdin))) {
		// This is pretty hacky
		input.len = n;

		execute_buffer(state);
		state_print(state);
	}
}
