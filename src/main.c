#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

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
    char arena[];
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
    }

    return state->pos;
}

int
main(int argc, char** argv) {
    struct state *state = state_initialize(0, 1024);
    while(!state->done) {
        state_step(state);
        printf("Pos: %d, Val: %d\n", (int)state->pos, state->arena[state->pos]);
    }
}