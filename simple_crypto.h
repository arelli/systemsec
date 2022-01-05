#ifndef __SIMPLE_CRYPTO_H
#define __SIMPLE_CRYPTO_H

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"simple_crypto.c"

void print_npc(char* npc_string, int length);
char* random_string(int length);
char* random_string_letters(char lowest, char highest,int length);
int is_letter(char input);
char* strip_special(char* input);
int find(char term,char* string);

char* one_time_pad(char* text, char* pad);
char* caesar_core(char* text, int shift, char* alphabet);
char* caesar(char* text, int shift);
char* vigenere(char* text, char* key);

#endif
