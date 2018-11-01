//
// Created by Hande Oktay on 9.10.2018.
//

#include <malloc.h>
#include <mem.h>
#include <locale.h>
#include "String.h"


String string_initialize(long letter) {
    String node = (String) malloc(sizeof(struct string_node));
    node->letter = letter;
    node->next = NULL;
    return node;
}

String string_add(String root, long letter) {
    if (root == NULL) {
        root = string_initialize(letter);
    } else {
        String iterator = root;
        while (iterator->next != NULL) {
            iterator = iterator->next;
        }
        iterator->next = string_initialize(letter);
    }
    return root;
}

String string_parse(char *data) {

    String str = NULL;
    for (int i = 0; i < strlen(data); i++) {
        str = string_add(str, data[i]);
    }
    return str;
}

String string_copy(String word) {
    String iterator = word;
    String str = NULL;
    while (iterator != NULL) {
        str = string_add(str, iterator->letter);
        iterator = iterator->next;
    }
    return str;
}


void string_print(String root) {
    String iterator = root;
    while (iterator != NULL) {
        printf("%c", (unsigned char) iterator->letter);
        iterator = iterator->next;
    }
}

//return 1 if equal, 0 if not
int string_equals(String word_1, String word_2) {
    String iterator_1 = word_1;
    String iterator_2 = word_2;

    while (iterator_1 != NULL && iterator_2 != NULL) {
        if (map_characters[value(iterator_1->letter)] == map_characters[value(iterator_2->letter)]) {
            iterator_1 = iterator_1->next;
            iterator_2 = iterator_2->next;
        } else {
            return 0;
        }
    }
    if (iterator_1 == NULL && iterator_2 == NULL) {
        return 1;
    }

    return 0;
}

//this function returns -1 if the first word is before, 1 if the first word is after and 0 if they are equal
int string_compare(String word_1, String word_2) {
    if (word_1 == NULL || word_2 == NULL) return -2;
    String iterator_1 = word_1;
    String iterator_2 = word_2;

    while (iterator_1 != NULL && iterator_2 != NULL) {

        if ((map_characters[value(iterator_1->letter)] - map_characters[value(iterator_2->letter)] < 0)) {
            return -1;
        } else if ((map_characters[value(iterator_1->letter)] - map_characters[value(iterator_2->letter)] > 0)) {
            return 1;
        } else if ((map_characters[value(iterator_1->letter)] - map_characters[value(iterator_2->letter)]) == 0) {
            iterator_1 = iterator_1->next;
            iterator_2 = iterator_2->next;
        }
    }
    if (iterator_1 != NULL) {
        return 1;
    } else if (iterator_2 != NULL) {
        return -1;
    } else {
        return 0;
    }

}

int value(long x) {
    switch (x) {
        case ('ç'):
        case -25:
            return 128;
        case ('Ç'):
            return 129;
        case ('ğ'):
        case -16:
            return 130;
        case ('Ğ'):
            return 131;
        case ('ı'):
        case -3:
            return 132;
        case ('İ'):
        case -35:
            return 133;
        case ('ö'):
        case -10:
            return 134;
        case ('Ö'):
            return 135;
        case ('ş'):
        case -2:
            return 136;
        case ('Ş'):
            return 137;
        case ('ü'):
        case -4:
            return 138;
        case ('Ü'):
            return 139;
        default:
            return x;
    }
}

void initialize_character_set() {

    map_characters[value('0')] = 0;
    map_characters[value('1')] = 1;
    map_characters[value('2')] = 2;
    map_characters[value('3')] = 3;
    map_characters[value('4')] = 4;
    map_characters[value('5')] = 5;
    map_characters[value('6')] = 6;
    map_characters[value('7')] = 7;
    map_characters[value('8')] = 9;
    map_characters[value('9')] = 10;

    map_characters[value('a')] = 11;
    map_characters[value('A')] = 11;

    map_characters[value('b')] = 12;
    map_characters[value('B')] = 12;

    map_characters[value('c')] = 13;
    map_characters[value('C')] = 13;

    map_characters[value('ç')] = 14;
    map_characters[value('Ç')] = 14;

    map_characters[value('d')] = 15;
    map_characters[value('D')] = 15;

    map_characters[value('e')] = 16;
    map_characters[value('E')] = 16;

    map_characters[value('f')] = 17;
    map_characters[value('F')] = 17;

    map_characters[value('g')] = 18;
    map_characters[value('G')] = 18;

    map_characters[value('ğ')] = 19;
    map_characters[value('Ğ')] = 19;

    map_characters[value('h')] = 20;
    map_characters[value('H')] = 20;

    map_characters[value('ı')] = 21;
    map_characters[value('I')] = 21;

    map_characters[value('i')] = 22;
    map_characters[value('İ')] = 22;

    map_characters[value('j')] = 23;
    map_characters[value('J')] = 23;

    map_characters[value('k')] = 24;
    map_characters[value('K')] = 24;

    map_characters[value('l')] = 25;
    map_characters[value('L')] = 25;

    map_characters[value('m')] = 26;
    map_characters[value('M')] = 26;

    map_characters[value('n')] = 27;
    map_characters[value('N')] = 27;

    map_characters[value('o')] = 28;
    map_characters[value('O')] = 28;

    map_characters[value('ö')] = 29;
    map_characters[value('Ö')] = 29;

    map_characters[value('p')] = 30;
    map_characters[value('P')] = 30;

    map_characters[value('r')] = 31;
    map_characters[value('R')] = 31;

    map_characters[value('s')] = 32;
    map_characters[value('S')] = 32;

    map_characters[value('ş')] = 33;
    map_characters[value('Ş')] = 33;

    map_characters[value('t')] = 34;
    map_characters[value('T')] = 34;

    map_characters[value('u')] = 35;
    map_characters[value('U')] = 35;

    map_characters[value('ü')] = 36;
    map_characters[value('Ü')] = 36;

    map_characters[value('v')] = 37;
    map_characters[value('V')] = 37;

    map_characters[value('y')] = 38;
    map_characters[value('Y')] = 38;

    map_characters[value('z')] = 39;
    map_characters[value('Z')] = 39;
}