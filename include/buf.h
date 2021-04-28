#ifndef FENGINE_BUFFER_H
#define FENGINE_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

struct feng_buffer {
    char *data;

    size_t len;
    size_t cap;
};

/**
 * Appends a string to the buffer. Returns true if successful.
 */
bool feng_buffer_append(struct feng_buffer *dest, char *data, size_t len);

/**
 * Adds char to the end of the array. Returns true if successful.
 */
bool feng_buffer_append_char(struct feng_buffer *dest, char c);

/**
 * Returns the buffers data and resets the buffer to 0.
 */
char *feng_buffer_to_owned(struct feng_buffer *buf);

/**
 * Ensures the capacity is as big as "size" or larger. Returns true if successful.
 */
bool feng_buffer_ensure_cap(struct feng_buffer *buf, size_t size);

/**
 * Resets buffer and frees any stored data.
 */
void feng_buffer_reset(struct feng_buffer *buf);

#endif
