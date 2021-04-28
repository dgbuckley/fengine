#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/param.h>

#include "buff.h"

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
state_initialize(int input_fd, size_t arena_size) {
    struct state *state;
    if ((state = calloc(sizeof(state) + arena_size, 1)) == NULL) return NULL;

    create_parser(input_fd, state->parser_buf, 1024, &state->parser);
    state->arena_size = arena_size;
    state->done = false;

    return state;
}

void
state_deinit(struct state *state) {
    free(state);
}

/**
 * Steps forward in the state of the program. It will return it's current position each step.
 */
size_t
state_step(struct state *state) {
    char inst

	// TODO this should be an error
	if  (parser_next(&state->parser, &inst)) return 0;

    switch (inst) {
    case '>' :
        if (state->pos < state->arena_size-1) state->pos++;
        else exit(1); // TODO error handling
        break;
    case '<' :
        if (state->pos < state->arena_size-1) state->pos--;
        else exit(1); // TODO error handling
        break;
    case '+' :
        state->arena[state->pos]++;
        break;
    case '-' :
        state->arena[state->pos]--;
        break;
	}

	return state->pos;
}

int
execute_buffer(struct state *state) {
    int buf_len = state->parser.buf.len;
    int executed_steps = buf_len - state->parser.pos;
    for (int step = state->parser.pos; step < buf_len; step++)
        state_step(state);
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
			fprintf(stderr, " %d", state->arena[idx]);

	fprintf(stderr, "\n");
}

int
main(int argc, char** argv) {
    struct state *state = state_initialize(0, 1024);
    while(!state->done) {
        state_step(state);
		state_print(state);
    }
}