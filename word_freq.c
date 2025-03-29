#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void save_to_sql(const char *filename, const char *word, int freq, FILE *sql_file) {
    fprintf(sql_file, "INSERT INTO word_freq (Filename, Word, Frequency) VALUES ('%s', '%s', %d);\n",
            filename, word, freq);
}

struct words {
    char *arr;
    int cap;
    int len;
    int count;
    int zero;
};

struct words newWord() {
    struct words word;
    word.arr = malloc(sizeof(char) * 4);
    if (word.arr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    word.cap = 4;
    word.len = 0;
    word.count = 0;
    return word;
}
void freeWord(struct words *list, int i) {
    if (list[i].arr != NULL) {  
        free(list[i].arr);
        list[i].arr = NULL;
        list[i].len = 0;
        list[i].cap = 0;
        list[i].count = 0;  
    }
}

void push(struct words *a, char x) {
    if ((x >= 'A' && x <= 'Z')) {
        x += 32;  
    }
    if ((x >= 'a' && x <= 'z') || (x >= '0' && x <= '9')) { 
        if (a->cap == a->len + 1) {
            char *newArr = malloc(sizeof(char) * 2 * a->cap);
            if (newArr == NULL) {
                fprintf(stderr, "Memory allocation failed.\n");
                exit(1);
            }
            for (int i = 0; i < a->len; ++i) {
                newArr[i] = a->arr[i];
            }
            free(a->arr);
            a->arr = newArr;
            a->cap = a->cap * 2;
        }
        a->arr[a->len++] = x;
        a->arr[a->len] = '\0';
    }
}

void realcheck(struct words *list, int *len_list) {
    if (list[*len_list - 1].len == 0) return;
    for (int i = 0; i < *len_list - 1; ++i) { 
        if (strcmp(list[i].arr, list[*len_list - 1].arr) == 0) {
            list[i].count++;  
            freeWord(list, *len_list - 1);
            (*len_list)--;  
            return;  
        }
    }
}

void list_grow(struct words **list, int *len_list ,int *cap_list) {
    struct words *newList = malloc(sizeof(struct words) * 2 * (*cap_list));
    if (newList == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    for (int i = 0; i < *len_list ; ++i){
        newList[i] = (*list)[i];
    }
    free(*list);
    *cap_list = *cap_list * 2;
    *list = newList;
}

void check(struct words *word, struct words **list, int *len_list, int *cap_list) {
    if (*len_list == *cap_list) list_grow(list, len_list, cap_list);
    (*list)[*len_list] = *word;
    word->arr = NULL ;
    (*list)[*len_list].count = 1;
    (*len_list)++;
    if (*len_list > 1) realcheck(*list, len_list); 
    if (word->arr != NULL) free(word->arr);
    *word = newWord();  
}


void freeList(struct words *list, int len_list) {
    for (int i = 0; i < len_list; i++) {
        freeWord(list, i);
    }
    if (list != NULL) {
        free(list);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <file_name>\n", argv[0]);
        return 1;
    }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File does not exist.\n");
        return 0;
    }

    struct words word = newWord();
    struct words *list = malloc(sizeof(struct words) * 4);
    if (list == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    int len_list = 0;
    int cap_list = 4;
    int ch;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            
            if (word.len > 0) {
                check(&word, &list, &len_list, &cap_list);
            }
            continue;
        }
        push(&word, ch);
    }

    if (word.len > 0) check(&word, &list, &len_list, &cap_list);
    if (word.arr != NULL) free(word.arr);  
    if (len_list == 0) {
        printf("No words found in the file.\n");
        fclose(file);
        freeList(list, len_list);
        return 0;
    }
    FILE *sql_file = fopen("word_freq.sql", "a");
    if (sql_file == NULL) {
        fprintf(stderr, "Failed to open word_freq.sql for writing.\n");
        freeList(list, len_list);
        fclose(file);
        return 1;
    }

    for (int i = 0; i < len_list; i++) {
        save_to_sql(argv[1], list[i].arr, list[i].count, sql_file);
    }

    fclose(sql_file);
    freeList(list, len_list);
    fclose(file);
    return 0;  
}
