#include "../Include/gapbuffer.h"
#include <string.h>
#include <stdio.h>
#define  GAP_MAX 10

void buffer_grow(gap_buffer* gb){
    char* new_buffer = realloc(gb->buff,gb->gap_size);
    for (size_t i=0;i<=gb->gap_start;i++) {
        new_buffer[i] = gb->buff[i];
    }
    for (size_t i=gb->gap_start+gb->gap_size;i<=gb->length;i++) {
        new_buffer[i] = gb->buff[i];
    }
    free(gb->buff);
    gb->buff=new_buffer;
}

void buffer_insert(gap_buffer* gb,char* letter,size_t letter_size){
    char *new = realloc(gb->buff, gb->length+letter_size);
    if (new==NULL)perror("Couldn't realloc exiting"),exit(1);
    //strcat(new,letter);
    memcpy(&new[gb->length], letter, letter_size);
    gb->length+=letter_size;
    gb->buff=new;
}

void buffer_remove(gap_buffer* gb){
    if(gb->buff[gb->gap_start]){
        buffer_grow(gb);
    }
    gb->gap_size+=1;
    gb->gap_start-=1;
}
void buffer_left(gap_buffer* gb){
    gb->gap_start-=1;
}
void buffer_next_word(gap_buffer* gb){
    size_t  idx=gb->gap_start;
    while (gb->buff[idx]!=' ') {
        idx+=1;
    }
    gb->gap_start=idx+1;
}
void buffer_right(gap_buffer* gb){
    gb->gap_start+=1;
}
void buffer_free(gap_buffer* gb){
    char* new = realloc(gb->buff, 1);
    new[0]='\0';
    gb->length=1;
    gb->buff=new;

}
