#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/param.h>

struct parser{
    int input_fd;
    char* buf; // Buffer for reading in input
    size_t buf_cap; // Size of the buffer
    ssize_t buf_l; //Amount read into the buffer
    size_t pos; // Position in the buffer
};

void
create_parser(int fd, char *buffer, int buffer_len, struct parser *parser) {
    parser->input_fd = fd;
    parser->buf = buffer;
    parser->buf_cap = buffer_len;
    parser->pos = 0;
    parser->buf_l = 0;
}

char
parser_next_char(struct parser *parser) {
    if (parser->pos >= parser->buf_l) {
        parser->buf_l = read(parser->input_fd, parser-> buf, parser->buf_cap);
        if (parser->buf_l <= 0) {
            if (parser->buf_l == 0) return '\0';
            // TODO handle this error
            return '\0';
        }
        parser->pos = 0;
    // } else parser->pos++;
    }

    return parser->buf[parser->pos++];
}

struct state{
    struct parser parser;
    char parser_buf[1024];
    bool done;
    size_t pos;
    size_t arena_size;
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
 * Steps forward in the state of the program. It will return it's current possition each step.
 */
size_t
state_step(struct state *state) {
    char inst = parser_next_char(&state->parser);
    switch (inst) {
    case '\0' :
        state->done = true;
        break;
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