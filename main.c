//
// Created by Hande Oktay on 9.10.2018.
//


#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <locale.h>
#include <malloc.h>
#include <conio.h>

struct string_node {                        //STRING STRUCT
    long letter;
    struct string_node *next;
};

typedef struct string_node *String;

struct sub_linked_list {                    //SUB LINKED LIST STRUCT - FOR MAIN LINKED LIST
    char *category_name;
    char *document_id;
    int count;
    int document_length;
    struct sub_linked_list *next;
};
typedef struct sub_linked_list *SLL;


struct main_linked_list {                   //MAIN LINKED LIST STRUCT
    String word;
    SLL sll;
    int file_count;
    struct main_linked_list *next;
};
typedef struct main_linked_list *MLL;


struct category_sub_linked_list {           //SUB LINKED LIST STRUCT - FOR CATEGORY LINKED LIST
    String word;
    int count;
    struct category_sub_linked_list *next;
};
typedef struct category_sub_linked_list *CSLL;


struct category_linked_list {               //CATEGORY LINKED LIST STRUCT
    char *category_name;
    CSLL csll;
    struct category_linked_list *next;
};
typedef struct category_linked_list *CLL;

struct word_linked_list {                   //WORD LINKED LIST STRUCT
    String word;
    int count;
    struct word_linked_list *next;
};
typedef struct word_linked_list *WLL;

//---------------------------------------
//Function prototypes
//---------------------------------------
void create_mll();

void create_cll();

void create_wll();

void mll_add(String word, char *categoryName, char *fileName);

SLL sll_add(SLL sub_linked_list, char *categoryName, char *fileName);

void cll_add(char *category_name);

CSLL csll_numerical_add(CSLL category_sub_linked_list, String word, int count);

void wll_numerical_add(String word, int count);

MLL mll_initialize(String word, char *categoryName, char *fileName);

SLL sll_initialize(char *categoryName, char *fileName);

CLL cll_initialize(char *category_name);

CSLL csll_initialize(String word, int count);

WLL wll_initialize(String word, int count);

CSLL csll_read(CSLL category_sub_linked_list, const char *category_name);

void mll_print(MLL mllPointer);

void sll_print(SLL sub_linked_list);

void cll_print(CLL cllHeadPtr);

void csll_print(CSLL category_sub_linked_list);

void wll_print(WLL wllHeadPtr);

void add_text_length(char *document_id, char *category_name, int text_length);

int sll_count(SLL sll);


void initialize_character_set();

String string_add(String root, long letter);

void string_print(String root);

String string_copy(String word);

int string_compare(String word_1, String word_2);

int value(long x);

//-----------------------------------------
//Variable Declarations
//---------------------------------------
MLL mllHeadPtr;
CLL cllHeadPtr;
WLL wllHeadPtr;
struct dirent *categoryPtr;          // Pointer to directory
struct dirent *filePtr;              // Pointer to file
char prevChar = NULL;
char c;
char *dataset_path;
char *file_path;
int length = 0;
int map_characters[500];            //Character set


int main() {
    mllHeadPtr = NULL;              //Pointer to Master Linked List(MLL)
    cllHeadPtr = NULL;              //Pointer to Category Linked List(CLL)
    setlocale(LC_ALL, "Turkish");   //Set locale to Turkish
    initialize_character_set();

    dataset_path = malloc(sizeof(char) * FILENAME_MAX);
    file_path = malloc(sizeof(char) * FILENAME_MAX);

    create_mll();                   //Handle MLL operations
    create_cll();                   //Handle Category Linked List(CLL) operations
    create_wll();
    wll_print(wllHeadPtr);
    getch();
    return 0;
}


//-----------CREATE METHODS-------------//

/***
 * This method read characters from all files and handle Masler Linked List (MLL) and Sub Linked List(SLL) operations.
 */
void create_mll() {

    DIR *dataset = opendir(".\\dataset");
    if (dataset == NULL) {          //check if directory exist
        printf("Could not open directory");
        return;
    }
    FILE *fp;

    //Iterate through ech sub-directory in "dataset" directory
    while ((categoryPtr = readdir(dataset)) != NULL) {

        //Ignore "." and ".."
        if (strcmp(categoryPtr->d_name, ".") != 0 && strcmp(categoryPtr->d_name, "..") != 0) {

            //Generate path for category directories and open them
            snprintf(dataset_path, sizeof(char) * 100, ".\\dataset\\%s", categoryPtr->d_name);
            DIR *dr = opendir(dataset_path);

            //Executing text files in each category directories. Iterates through each file in corresponding directory
            while ((filePtr = readdir(dr)) != NULL) {

                //Ignore "." and ".."
                if (strcmp(filePtr->d_name, ".") != 0 && strcmp(filePtr->d_name, "..") != 0) {

                    //Generate path for each text file
                    snprintf(file_path, sizeof(char) * 100, "%s\\%s", dataset_path, filePtr->d_name);

                    //Open a file and read characters
                    fp = fopen(file_path, "r");
                    c = (char) fgetc(fp);
                    String word = NULL;
                    length = 0;

                    while (c != EOF) {
                        while (c == ' ' || c == '\n') {          //Loop for skipping whitespace characters
                            prevChar = c;           //Holds the space character
                            c = (char) fgetc(fp);   //Holds the character after space for next iteration
                            length++;
                        }
                        if (prevChar ==
                            ' ' || prevChar ==
                                   '\n') {      //Make sure previous character is whitespace, prevent adding each char as a different MLL node
                            mll_add(word, categoryPtr->d_name, filePtr->d_name);
                            word = NULL;            //Set String word (pointer) to null for new word
                        }
                        word = string_add(word, c); //Add character to word
                        prevChar = NULL;
                        c = (char) fgetc(fp);
                        length++;
                    }
                    add_text_length(filePtr->d_name, categoryPtr->d_name, length);
                    fclose(fp);
                }
            }
            closedir(dr);
        }
    }
    closedir(dataset);

    MLL iterator = mllHeadPtr;
    while (iterator != NULL) {
        iterator->file_count = sll_count(iterator->sll);
        iterator = iterator->next;
    }

    mll_print(mllHeadPtr);                          //Print the MLL and each node's sub linked list(SLL).
}


/***
 * Counts all the sub linked list nodes from one MLL node
 * @param sll
 * @return (int)count of the sub linked list nodes
 */
int sll_count(SLL sll) {
    int count = 0;
    while (sll != NULL) {
        count++;
        sll = sll->next;
    }
    return count;
}

/**
 * Read category names and handle creating Category Linked List(CLL) and Category Sub Linked List(CSLL) operations.
 * CLL is used for detecting discriminating words.
 */
void create_cll() {

    DIR *dataset = opendir(".\\dataset");
    if (dataset == NULL) {    //check if directory exist
        printf("Could not open directory");
        return;
    }

    //ITERATES THROUGH EACH SUB DIRECTORY IN 'DATASET' DIRECTORY
    while ((categoryPtr = readdir(dataset)) != NULL) {

        //IGNORE "." AND ".."
        if (strcmp(categoryPtr->d_name, ".") != 0 && strcmp(categoryPtr->d_name, "..") != 0) {
            cll_add(categoryPtr->d_name);
        }
    }
    cll_print(cllHeadPtr);
    closedir(dataset);
}


/***
 * Read through MLL and its sub linked lists for finding stop words
 */
void create_wll() {
    MLL mll_iterator = mllHeadPtr;

    //iterate through MLL
    while (mll_iterator != NULL) {
        int count = 0;

        //iterate through SLL in each node of MLL
        SLL sll_iterator = mll_iterator->sll;
        while (sll_iterator != NULL) {
            count += sll_iterator->count;
            sll_iterator = sll_iterator->next;
        }
        wll_numerical_add(mll_iterator->word, count);

        mll_iterator = mll_iterator->next;
    }

}



//-----------ADD METHODS-------------//

/*** This function iterates through MLL and check if the word which will be added already exists in MLL.
 *   It uses string_compare() function to compare words in each node.
 *   Adds the new node according to alphabetical order.
 *   @param String word, categoryName, filename
 */
void mll_add(String word, char *categoryName, char *fileName) {

    if (mllHeadPtr == NULL) {
        mllHeadPtr = mll_initialize(word, categoryName, fileName);
    } else if (string_compare(mllHeadPtr->word, word) == 1) {           //If new node should be the first node
        MLL newNode = mll_initialize(word, categoryName, fileName);
        newNode->next = mllHeadPtr;
        mllHeadPtr = newNode;
    } else {
        MLL iterator = mllHeadPtr;

        while (iterator->next != NULL && string_compare(iterator->next->word, word) == -1) {
            iterator = iterator->next;
        }

        if (iterator->next != NULL && string_compare(iterator->next->word, word) == 0) {
            iterator->next->sll = sll_add(iterator->next->sll, categoryName, fileName);
            return;
        } else {
            MLL newNode = mll_initialize(word, categoryName, fileName);
            newNode->next = iterator->next;
            iterator->next = newNode;
            return;
        }
    }
}

/***
 * Creates SSL for each MLL node.
 * Checks the document name and file if for preventing duplicate entries.
 * @param sub_linked_list - corresponding MLL node's sub linked list pointer
 * @param categoryName
 * @param fileName
 * @return sub_linked_list
 */
SLL sll_add(SLL sub_linked_list, char *categoryName, char *fileName) {

    if (sub_linked_list == NULL) {
        sub_linked_list = sll_initialize(categoryName, fileName);
    } else {
        SLL iterator = sub_linked_list;
        while (iterator != NULL) {
            if ((strcmp(iterator->category_name, categoryName) == 0)) {
                if ((strcmp(iterator->document_id, fileName) == 0)) {
                    iterator->count++;
                    return sub_linked_list;
                }
            }

            if (iterator->next == NULL) {
                iterator->next = sll_initialize(categoryName, fileName);
                return sub_linked_list;
            }

            iterator = iterator->next;
        }
    }
    return sub_linked_list;
}

/***
 * Creates Category Sub Linked List
 * This linked list used for detecting discriminating words.
 * @param category_name - takes current directory name during reading files.
 */
void cll_add(char *category_name) {

    if (cllHeadPtr == NULL) {
        cllHeadPtr = cll_initialize(category_name);
    } else {
        CLL iterator = cllHeadPtr;
        while (iterator->next != NULL) iterator = iterator->next;
        iterator->next = cll_initialize(category_name);
    }
}

/***
 *
 * @param category_sub_linked_list
 * @param word
 * @param count
 * @return
 */
CSLL csll_numerical_add(CSLL category_sub_linked_list, String word, int count) {

    if (category_sub_linked_list == NULL) {
        return csll_initialize(word, count);

    } else if (category_sub_linked_list->count < count) {
        CSLL newNode = csll_initialize(word, count);
        newNode->next = category_sub_linked_list;
        return newNode;
    } else {
        CSLL iterator = category_sub_linked_list;

        while (iterator->next != NULL && iterator->next->count > count) {
            iterator = iterator->next;
        }

        CSLL newNode = csll_initialize(word, count);
        newNode->next = iterator->next;
        iterator->next = newNode;

        return category_sub_linked_list;
    }
}

CSLL csll_alphabetical_add(CSLL category_sub_linked_list, String word, int count) {

    if (category_sub_linked_list == NULL) {
        return csll_initialize(word, count);

    } else if (string_compare(category_sub_linked_list->word, word) == 1) {
        CSLL newNode = csll_initialize(word, count);
        newNode->next = category_sub_linked_list;
        return newNode;
    } else {
        CSLL iterator = category_sub_linked_list;

        while (iterator->next != NULL && string_compare(iterator->next->word, word) == -1) {
            iterator = iterator->next;
        }

        CSLL newNode = csll_initialize(word, count);
        newNode->next = iterator->next;
        iterator->next = newNode;

        return category_sub_linked_list;
    }
}


/**
 * Creates Word Linked List and add nodes according
 * @param word
 * @param count
 */
void wll_numerical_add(String word, int count) {

    if (wllHeadPtr == NULL) {
        wllHeadPtr = wll_initialize(word, count);
    } else if (wllHeadPtr->count < count) {
        WLL newNode = wll_initialize(word, count);
        newNode->next = wllHeadPtr;
        wllHeadPtr = newNode;
    } else {
        WLL iterator = wllHeadPtr;
        while (iterator->next != NULL && iterator->next->count > count) {
            iterator = iterator->next;
        }
        WLL newNode = wll_initialize(word, count);
        newNode->next = iterator->next;
        iterator->next = newNode;
    }
}

WLL wll_alphabetical_add(WLL wllHeadPtr, String word, int count) {

    if (wllHeadPtr == NULL) {
        wllHeadPtr = wll_initialize(word, count);
    } else if (string_compare(wllHeadPtr->word, word) == 1) {
        WLL newNode = wll_initialize(word, count);
        newNode->next = wllHeadPtr;
        wllHeadPtr = newNode;
    } else {
        WLL iterator = wllHeadPtr;

        while (iterator->next != NULL && string_compare(iterator->next->word, word) == -1) {
            iterator = iterator->next;
        }

        WLL newNode = wll_initialize(word, count);
        newNode->next = iterator->next;
        iterator->next = newNode;
    }
    return wllHeadPtr;
}





//-----------INITIALIZE NODES FOR LINKED LISTS METHODS-------------//

/***
 *  Allocate memory for new node in Master Linked List
 *  Adds given character in parameter to new node
 * @param wordPtr, document_id
 * @return the new node
 */
MLL mll_initialize(String word, char *categoryName, char *fileName) {
    MLL node = (MLL) malloc(sizeof(struct main_linked_list));
    node->next = NULL;
    node->file_count = 1;
    node->word = word;
    node->sll = NULL;
    node->sll = sll_add(node->sll, categoryName, fileName);
    return node;
}

SLL sll_initialize(char *categoryName, char *fileName) {
    SLL node = (SLL) malloc(sizeof(struct sub_linked_list));
    node->document_id = malloc(sizeof(char) * FILENAME_MAX);
    node->category_name = malloc(sizeof(char) * FILENAME_MAX);

    node->next = NULL;
    node->count = 1;
    node->document_length = 0;

    strcpy(node->category_name, categoryName);
    strcpy(node->document_id, fileName);
    return node;
}

CLL cll_initialize(char *category_name) {
    CLL node = malloc(sizeof(struct category_linked_list));
    node->category_name = malloc(sizeof(char) * FILENAME_MAX);

    strcpy(node->category_name, category_name);
    node->next = NULL;
    node->csll = NULL;
    node->csll = csll_read(node->csll, category_name);
    return node;
}

CSLL csll_read(CSLL category_sub_linked_list, const char *category_name) {

    MLL mll_iterator = mllHeadPtr;

    //iterate through MLL
    while (mll_iterator != NULL) {
        int count = 0;

        //iterate through SLL in each node of MLL
        SLL sll_iterator = mll_iterator->sll;
        while (sll_iterator != NULL) {

            if (strcmp(sll_iterator->category_name, category_name) == 0) {
                count += sll_iterator->count;
                sll_iterator = sll_iterator->next;
            } else {
                count = 0;
                goto test;
            }
        }
        test:
        {
            printf("");
        };
        if (count != 0) {
            category_sub_linked_list = csll_numerical_add(category_sub_linked_list, mll_iterator->word, count);
        }
        mll_iterator = mll_iterator->next;
    }

    return category_sub_linked_list;
}


CSLL csll_initialize(String word, int count) {
    CSLL node = malloc(sizeof(struct category_sub_linked_list));
    node->word = string_copy(word);
    node->next = NULL;
    node->count = count;
    return node;
}

WLL wll_initialize(String word, int count) {
    WLL node = malloc(sizeof(struct word_linked_list));
    node->word = string_copy(word);
    node->count = count;
    node->next = NULL;
    return node;
}


void add_text_length(char *document_id, char *category_name, int text_length) {

    MLL mll_iterator = mllHeadPtr;

    while (mll_iterator != NULL) {
        SLL sll_iterator = mll_iterator->sll;
        while (sll_iterator != NULL) {
            if (strcmp(sll_iterator->category_name, category_name) == 0 &&
                strcmp(sll_iterator->document_id, document_id) == 0) {
                sll_iterator->document_length = text_length;
            }
            sll_iterator = sll_iterator->next;
        }
        mll_iterator = mll_iterator->next;
    }


}




//-----------PRINT METHODS-------------//

/***
 * Iterates through each node and prints the Master Linked List
 * @param MLL head pointer
 */
void mll_print(MLL mllPointer) {
    MLL iterator = mllPointer;

    printf("MASTER LINKED LIST: ");
    while (iterator != NULL) {
        printf("\n\n\t=> ");
        string_print(iterator->word);
        printf("\n\t   Detected at %d %s!\n\t------------------------------------------------", iterator->file_count,
               (iterator->file_count == 1 ? "file" : "files"));
        sll_print(iterator->sll);
        printf("\n");
        iterator = iterator->next;
    }
}

void sll_print(SLL sub_linked_list) {
    SLL iterator = sub_linked_list;
    int count = 1;
    while (iterator != NULL) {
        printf("\n\t    %d-) Category: %s\n", count++, iterator->category_name);
        printf("\t        id:%s", iterator->document_id);
        printf("\tlength:%d", iterator->document_length);
        printf("\tcount:%d\n", iterator->count);
        iterator = iterator->next;
    }
}

void cll_print(CLL cllHeadPtr) {
    CLL iterator = cllHeadPtr;
    printf("\n\n\nDISCRIMINATING WORDS:\n");

    while (iterator != NULL) {
        printf("\n\t=> Category Name: ");
        printf(iterator->category_name);
        csll_print(iterator->csll);
        printf("\n");
        iterator = iterator->next;
    }
}

void csll_print(CSLL category_sub_linked_list) {
    CSLL iterator = category_sub_linked_list;
    int count = 0;
    CSLL alphabeticalSorted = NULL;
    while (iterator != NULL && count < 5) {
        alphabeticalSorted = csll_alphabetical_add(alphabeticalSorted, iterator->word, iterator->count);
        iterator = iterator->next;
        count++;
    }

    while (alphabeticalSorted != NULL) {
        printf("\n\t   word:  ");
        string_print(alphabeticalSorted->word);
        printf("\n\t   count:  %d\n", alphabeticalSorted->count);
        alphabeticalSorted = alphabeticalSorted->next;
    }
}

void wll_print(WLL wllHeadPtr) {
    WLL iterator = wllHeadPtr;
    int count = 0;
    printf("\n\n\nSTOP WORDS:\n");

    WLL alphabeticalSorted = NULL;
    while (iterator != NULL && count < 5) {
        alphabeticalSorted = wll_alphabetical_add(alphabeticalSorted, iterator->word, iterator->count);
        iterator = iterator->next;
        count++;
    }
    while (alphabeticalSorted != NULL) {
        printf("\n\t   word:  ");
        string_print(alphabeticalSorted->word);
        printf("\n\t   count:  %d\n", alphabeticalSorted->count);
        alphabeticalSorted = alphabeticalSorted->next;
    }
}






//-----------STRING METHODS-------------//

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



//-----------FOR CHARACTER SET-------------//

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

    map_characters[value('W')] = 38;
    map_characters[value('w')] = 38;

    map_characters[value('X')] = 39;
    map_characters[value('x')] = 39;

    map_characters[value('y')] = 40;
    map_characters[value('Y')] = 40;

    map_characters[value('z')] = 41;
    map_characters[value('Z')] = 41;
}