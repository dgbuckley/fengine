#include <string.h>
#include <stdlib.h>
#include "buf.h"


bool
feng_buffer_append(struct feng_buffer *dest, char *data, size_t len) {
	if (!feng_buffer_ensure_cap(dest, len + dest->len)) return false;

    memcpy(dest->data + dest->len, data, len);
	dest->len += len;
    return true;
}


bool
feng_buffer_ensure_cap(struct feng_buffer *buf, size_t size) {
	if (buf->cap >= size) return true;

    size_t cap = buf->cap > 1 ? buf->cap : 1;
    for (; cap < size; (cap = 2 * cap));

    char *data = realloc(buf->data, cap);
    if (data == NULL) return false;

    buf->cap = cap;
    buf->data = data;
    return true;
}

bool
feng_buffer_append_char(struct feng_buffer *buf, char c) {
	if (!feng_buffer_ensure_cap(buf, buf->len+1)) return false;

	buf->data[buf->len] = c;
	buf->len++;
	return true;
}


void
feng_buffer_reset(struct feng_buffer *buf) {
    if (!buf->data) free(buf->data);
    buf->cap = buf->len = 0;
}


char*
feng_buffer_to_owned(struct feng_buffer *buf) {
    char *data = buf->data;
	buf->data = NULL;
	feng_buffer_reset(buf);
	return data;
}
