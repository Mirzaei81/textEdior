#include <stdio.h>
#include <stdlib.h>
typedef struct gap_buffer{
    char* buff;
    size_t length;  //buffer length
    size_t gap_size; //gap
    size_t gap_start;
}gap_buffer;

void buffer_grow(gap_buffer* gap_buff);
void buffer_insert(gap_buffer* gap_buff,char* letter,size_t letter_size);
void buffer_left(gap_buffer* gap_buff);
void buffer_right(gap_buffer* gap_buff);
void buffer_next_word(gap_buffer* gap_buff);
void buffer_free(gap_buffer* gap_buffer);
