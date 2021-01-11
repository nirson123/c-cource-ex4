#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_LETTERS ((int)26)

/**
 * trie node data structure
 */
typedef struct node {
    char letter;                            //the latter of the edge entering this node
    long unsigned int count;                //the number of times the word represented by this node was entered
    struct node* children[NUM_LETTERS];     //the node's children
} node;


/**
 * create a new node
 * @param letter
 * @return pointer to new node
 */
node* new_node(char letter){
    node* new = (node*)malloc(sizeof(node));    //allocate the memory for the new node
    (*new).letter = letter;                     //set the letter to the given one
    (*new).count = 0;                           //initiate the counter
    int j;
    for(j = 0 ; j < NUM_LETTERS ; j++){         //initiate the children array
        (*new).children[j] = NULL;
    }
    return new;
}

/**
 * enter a word to the trie
 * @param trie
 * @param word
 * @param len
 */
void enter_word(node* trie , char word[] , size_t len){
    int i = 0;
    node* current = trie;
    while( i < len && (*current).children[word[i] - 'a'] != NULL){  //enter the trie until end of word / non exiting nodes
        current = (*current).children[word[i] - 'a'];
        i++;
    }

    if(i == len){               //if end of word,
        (*current).count++;     //update the counter
    }
    else{                       //if non existing nodes,
        for(;i<len;i++) {       //create the missing nodes
            (*current).children[word[i] - 'a'] = new_node(word[i]);
            current = (*current).children[word[i] - 'a'];
        }
        (*current).count++;     //update the counter
    }
}

/**
 * initiate a new trie
 * @return pointer to root a new trie
 */
node* init_trie(){
    return new_node('\0');
}

/**
 * print all the words store in the trie, and the number of times they appear
 * @param trie
 * @param word
 * @param i
 */
void print_words(node* trie , char* word , int i){

    if(trie == NULL){return;}   // end - case for recursion

    char* new_word = (char*)malloc((i + 1)*sizeof(char));   //store the current word

    strcpy(new_word , word);    //copy the previews word
    if(i == 0){                 //if root - set to empty string
    new_word[i] = '\0';
    }
    else{                       //else - add the current letter
        new_word[i-1] = (*trie).letter;
        new_word[i] = '\0';
    }

    if((*trie).count != 0){     //if the current word appear - print it
        printf("%s %lu\n" , new_word , (*trie).count);
    }
    int j;
    for(j = 0 ; j < NUM_LETTERS ; j++){     //recursively call the function for the children
        print_words((*trie).children[j] , new_word , i+1);
    }
    free(new_word);            //free the allocated memory
}

/**
 * print all the words store in the trie, and the number of times they appear , in revered order
 * @param trie
 * @param word
 * @param i
 */
void print_words_r(node* trie , char* word , int i){

    if(trie == NULL){return;}       // end - case for recursion

    char* new_word = (char*)malloc((i + 1)*sizeof(char));   //store the current word

    strcpy(new_word , word);        //copy the previews word
    if(i == 0){                     //if root - set to empty string
        new_word[i] = '\0';
    }
    else{                           //else - add the current letter
        new_word[i-1] = (*trie).letter;
        new_word[i] = '\0';
    }

    if((*trie).count != 0){     //if the current word appear - print it
        printf("%s %lu\n" , new_word , (*trie).count);
    }
    int j;
    for(j = (NUM_LETTERS - 1) ; j >=0 ; j--){       //recursively call the function for the children
        print_words_r((*trie).children[j] , new_word , i+1);
    }
    free(new_word);                 //free the allocated memory
}

/**
 * delete a trie - free all the allocated memory
 * @param trie
 */
void del_trie(node* trie){
    if(trie == NULL){       //end case for recursion
        return;
    }
    else{
        int i;
        for(i = 0 ; i < NUM_LETTERS ; i++){     //delete each of the children
            del_trie((*trie).children[i]);
        }
        free(trie);     //free the current node
    }
}


/**
 * read a word from the standard input. turn upper case to lower case, and ignore not-letter symbols
 * @param w
 * @return the number of letters in the read word
 */
int read_word(char** w){
    *w = (char*)malloc(sizeof(char));       //allocate memory for one letter
    *w[0] = '\0';       //initiate to empty string
    int i = 0;
    char c;
    while((c=getchar()) != EOF && c != ' ' && c != '\t' && c != '\n'){      //until the end of a word:
        if(c >= 'a' && c <= 'z') {                                          //if a letter,
            char *w_n = (char *) malloc((i + 2) * sizeof(char));       //increase the size by one
            strcpy(w_n, *w);            //copy the sub-word
            free(*w);                   //free the old string
            w_n[i] = c;                 //add the new letter
            w_n[i + 1] = '\0';
            *w = w_n;                   //replace the pointer
            i++;
        }
        else if (c >= 'A' && c <= 'Z') {        //if an upper case letter,
            c = (c - 'A') + 'a';                //replace to lower case
            char *w_n = (char *) malloc((i + 2) * sizeof(char));    //increase the size by one
            strcpy(w_n, *w);        //copy the sub-word
            free(*w);               //free the old string
            w_n[i] = c;             //add the new letter
            w_n[i + 1] = '\0';
            *w = w_n;               //replace the pointer
            i++;
        }
    }
    return i;       //return the number of letters read
}

/**
 * build a new trie from input given in the standard input
 * @return a pointer to the new trie
 */
node* build_trie_from_stdin(){
    node* trie = init_trie();   //initiate the trie
    char* w;
    char** p_w = &w;
    int i = read_word(p_w);     //read a word
    while (i != 0){             //while we read valid words:
        enter_word(trie , w , i);   //enter the word to the trie
        free(w);                    //free the string
        i = read_word(p_w);         //read the next word
    }
    free(w);            //free the string
    return trie;        //return the pointer to the trie root
}


int main(int argc , char* argv[]){

    node *trie = build_trie_from_stdin();   //build a trie from the input

    if(argc == 2 && strcmp(argv[1] , "r") == 0){      //if an 'r' argument is given,
    print_words_r(trie , "" , 0);             //print the words in reversed order
    }
    else if (argc == 1){                               //if no arguments were given,
        print_words(trie, "", 0);              //print the words normally
    }
    else{                                               //else - not a valid input
        printf("too many arguments");
    }

    del_trie(trie);     //delete the trie and free all the allocated memory

    return 0;
}