//
// Created by Hande Oktay on 11.10.2018.
//
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>
#include "incudes/String.h"

void ascii_map(char c);

int main() {
    setlocale(LC_ALL, "tr_TR.UTF-8"); //set language to Turkish


    initialize_character_set();

    String test1 = NULL;
    test1 = string_add(test1, 'b');
    test1 = string_add(test1, 'ğ');
    test1 = string_add(test1, 'z');


    char *data = "I hope it works";
    String test2 = NULL;
    test2 = string_add(test2, 'B');
    test2 = string_add(test2, 'ğ');
    test2 = string_add(test2, 'z');


      printf("%d", string_compare(test1, test2));
    // test1 = string_parse(data);
    //test1 = string_copy(test2);
   // string_print(test1);


}


void ascii_map(char c) {

    int map[100];
    map[97] = 1000;

    int x = map[c];
    // printf("%d", x);
}