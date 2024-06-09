#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Include/str_util.h"

int u8len(char*s){
    int len = 0;
    while(*s){
        /*
        * INFO: By utf-8 standars all bytes in multibyte charcode start with 10xxxxxx excluding the first code point
        * by and ing them with 0xc0 1100000 we can check that current byte is the first code point or not
        * eg the codepoint for char ا (alef) is 0xD8 0xA7 where 0xD8 translate to 11010111  and second byte to  10100111 respectivly
        * by anding the first byte we get 1100000,0xc0 wich is not equal to 10000000
        * but for seconf byte we get 0x80 wich is exactly matches to binary  we don't want  to calculate
        */
        if((*s & 0xc0)!=0x80)len+=1;
        s++;
    }
    return len;
}
// INFO: get the cuorsor position of the given string  get length and strating coursor position
char* u8pos(char* s,int x, int y){
    int l = u8len(s);


    return"\0x1b[";
}

char* u8strncpy(char*dest,const char*src,size_t n){
    int k=n-1;// INFO: Should consider the last empty byte for \0 where the last bytecode is length 4
    size_t i;
    if((int)n>u8len(dest)){
        n = u8len(dest);
    };
    if(k){
        dest[k]=0;
        strncpy(dest, src, n);
        if(dest[k]&0x80){//last byte has been overwriten
            //increament the i till it reaches last valid utf-8 char byte 10xxxxxx
            for (i=k; (i>0) && ((k-i) < 3) && ((dest[i] & 0xC0) == 0x80); i--) ;
            switch (k-i) {
                case 0:                       dest[i]='\0';break;
                case 1:if((src[i]&0xe0)!=0xc0)dest[i]='\0';break;//0xe0 11100000 ; 0xc0 11000000 code point length is 2 bytes
                case 2:if((src[i]&0xf0)!=0xe0)dest[i]='\0';break;//0xf0 11110000 ; 0xe0 11100000 code point length is 3 bytes
                case 3:if((src[i]&0xf8)!=0xf0)dest[i]='\0';break;//0xf8 11111000 ; 0xe0 11110000 code point length is 4 bytes
                dest[i]=src[i];
            }
        }
    }
    return dest;
}

void *join(char** string_list,int length){
    char *s = malloc(length*20);//asuming every word is at most 20 bytes
    for (int i=length-1;i>=0; i--) { // what to do if it reaches a latina word
        strcat(s, string_list[i]);
        free(string_list[i]);
    }
    s[strlen(s)] = '\0';
    return s;
}

char * reverse_string(char *s,char delimiter){
    char *word= malloc(20);//guessed the maximum sized for a word
    int letter_index = 0;
    char **words =  malloc(strlen(s));
    int words_index=0 ;
    for (size_t i=0;i<strlen(s); i+=1) {
            if (s[i]==delimiter){
                words[words_index]=reverse_word(word);
                words_index+=1;                       // INFO: Found a  Word
                word=malloc(20);
                letter_index=0;
            }
            else{
                word[letter_index] = s[i];
                letter_index+=1;
            }
        }
    words[words_index] = reverse_word(word);
    return join(words,words_index+1);
}

char* reverse_word(const char* s){
    if((*s&0x80)==0x80){
        char * res = malloc(strlen(s)+2);
        size_t length=  strlen(s);
        for (size_t i=0;i<length;i+=2) {
            res[i] = s[length-i-2];
            res[i+1] = s[length-i-1];
        }
        res[length]=' ';
        res[length+1]='\0';
        return res;
    }
    else{
        char* res = malloc(strlen(s)+1);
        strcat(res, s);
        strcat(res, " ");
        return res;
    }
}
