//
// Created by Hande Oktay on 9.10.2018.
//
#ifndef HANDE_OKTAY_STRING_H
#define HANDE_OKTAY_STRING_H

#include <stdio.h>

int map_characters[500];

struct string_node {
    long letter;
    struct string_node *next;
};

typedef struct string_node *String;

void initialize_character_set();

String string_add(String root, long letter);

String string_parse(char *data);

void string_print(String root);

String string_copy(String word);

int string_equals(String word_1, String word_2);

int string_compare(String word_1, String word_2);

int value(long x);


#endif //HANDE_OKTAY_STRING_H


