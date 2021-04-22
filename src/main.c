#include <stdio.h>
#include <dirent.h>

struct parser{
    int input_fd;
    char* buf; // Buffer for reading in input
    int buf_l; //Amount read into the buffer
    int pos; // Position in the buffer
};

void
create_parser(int fd, char *buffer, struct parser *parser) {
    parser->input_fd = fd;
    parser->buf = buffer;
}

int
main(int argc, char** argv) {
    // TODO
}