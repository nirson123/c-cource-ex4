#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_LETTERS ((int)26)

typedef struct node {
    char letter;
    long unsigned int count;
    struct node* children[NUM_LETTERS];
} node;


node* new_node(char letter){
    node* new = (node*)malloc(sizeof(node));
    (*new).letter = letter;
    (*new).count = 0;
    int j;
    for(j = 0 ; j < NUM_LETTERS ; j++){
        (*new).children[j] = NULL;
    }
    return new;
}


void enter_word(node* trie , char word[] , size_t len){
    int i = 0;
    node* current = trie;
    while( i < len && (*current).children[word[i] - 'a'] != NULL){
        current = (*current).children[word[i] - 'a'];
        i++;
    }

    if(i == len){
        (*current).count++;
    }
    else{
        for(;i<len;i++) {
            (*current).children[word[i] - 'a'] = new_node(word[i]);
            current = (*current).children[word[i] - 'a'];
        }
        (*current).count++;
    }
}

node* init_trie(){
    return new_node('\0');
}

void print_words(node* trie , char* word , int i){

    if(trie == NULL){return;}

    char* new_word = (char*)malloc((i + 1)*sizeof(char));

    strcpy(new_word , word);
    if(i == 0){
    new_word[i] = '\0';
    }
    else{
        new_word[i-1] = (*trie).letter;
        new_word[i] = '\0';
    }

    if((*trie).count != 0){
        printf("%s %lu\n" , new_word , (*trie).count);
    }
    int j;
    for(j = 0 ; j < NUM_LETTERS ; j++){
        print_words((*trie).children[j] , new_word , i+1);
    }
    free(new_word);
}

void print_words_r(node* trie , char* word , int i){

    if(trie == NULL){return;}

    char* new_word = (char*)malloc((i + 1)*sizeof(char));

    strcpy(new_word , word);
    if(i == 0){
        new_word[i] = '\0';
    }
    else{
        new_word[i-1] = (*trie).letter;
        new_word[i] = '\0';
    }

    if((*trie).count != 0){
        printf("%s %lu\n" , new_word , (*trie).count);
    }
    int j;
    for(j = (NUM_LETTERS - 1) ; j >=0 ; j--){
        print_words_r((*trie).children[j] , new_word , i+1);
    }
    free(new_word);
}


void del_trie(node* trie){
    if(trie == NULL){
        return;
    }
    else{
        int i;
        for(i = 0 ; i < NUM_LETTERS ; i++){
            del_trie((*trie).children[i]);
        }
        free(trie);
    }
}

int read_word(char** w){
    *w = (char*)malloc(sizeof(char));
    *w[0] = '\0';
    int i = 0;
    char c;
    while((c=getchar()) != EOF && c != ' ' && c != '\t' && c != '\n'){
        if(c >= 'a' && c <= 'z') {
            char *w_n = (char *) malloc((i + 2) * sizeof(char));
            strcpy(w_n, *w);
            free(*w);
            w_n[i] = c;
            w_n[i + 1] = '\0';
            *w = w_n;
            i++;
        }
        else if (c >= 'A' && c <= 'Z') {
            c = (c - 'A') + 'a';
            char *w_n = (char *) malloc((i + 2) * sizeof(char));
            strcpy(w_n, *w);
            free(*w);
            w_n[i] = c;
            w_n[i + 1] = '\0';
            *w = w_n;
            i++;
        }
    }
    return i;
}


node* build_trie_from_stdin(){
    node* trie = init_trie();
    char* w;
    char** p_w = &w;
    int i = read_word(p_w);
    while (i != 0){
        enter_word(trie , w , i);
        free(w);
        i = read_word(p_w);
    }
    free(w);
    return trie;
}


int main(int argc , char* argv[]){

    node *trie = build_trie_from_stdin();

    if(argc == 2 && strcmp(argv[1] , "r") == 0){
    print_words_r(trie , "" , 0);
    }
    else if (argc == 1){
        print_words(trie, "", 0);
    }
    else{
        printf("too many arguments");
    }

    del_trie(trie);

    return 0;
}