#ifndef __SIMPLE_CRYPTO_H
#define __SIMPLE_CRYPTO_H

#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include"simple_crypto.c"

struct otp_response one_time_pad(char* text, char* pad);
char* caesar(char* text, int offset);
char* viginere(char* text, char* cipher);
char* random_string(int length); 

#endif
