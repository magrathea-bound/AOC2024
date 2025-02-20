#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TABLE_SIZE 800
#define INT_LENGTH 5
#define INPUT_LENGTH 1000

typedef struct Node{
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct hashTable {
    Node *table[TABLE_SIZE];
} HashTable;

HashTable *init_hash_table() {
    HashTable *ht = (HashTable*)malloc(sizeof(HashTable));
    for(int i = 0; i < TABLE_SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

int hash(char *key) {
    int hash = 0;

    for(int i = 0; key[i] != '\0'; i++) {
        hash = ((hash<<5) + hash) + key[i];
    }

    return hash % TABLE_SIZE;
}

void add_key(char *key, int value, HashTable *ht) {
    Node *new_node = (Node*)malloc(sizeof(Node));
    new_node->key = key;
    new_node->value = value;

    int hi = hash(key);
    if(ht->table[hi] != NULL) {
        new_node->next = ht->table[hi];
    }
    ht->table[hi] = new_node;
}

int retrieve(char *key, HashTable *ht) {
    int hi = hash(key);
    Node *hash_node = ht->table[hi];
    if(hash_node == NULL) {
        return -1;
    }

    while(hash_node) {
        if (hash_node->key == key) {
            return hash_node->value;
        }
        hash_node = hash_node->next;

    }
    return -1;
}

void free_hash_table(HashTable *ht) {
    for(int i = 0; i < TABLE_SIZE; i++) {
        Node *free_node = ht->table[i];
        while(free_node != NULL){
            Node *temp = free_node;
            free_node = free_node->next;
            free(temp);
            
        }
    }
    free(ht);
}

int calc_duplicity(int duplicity_num, int checking[]) {
    int duplicated = 0;
    for(int i = 0; i < INPUT_LENGTH; i++){
        if(duplicity_num == checking[i]) {
            duplicated += 1;
        }             
    }
    return duplicity_num * duplicated;
}

 int main(int argc, char *argv[]) {
    if(argc != 2) {
        errno = EINVAL;
        perror("Invalid Argument");
        return -1;
    }
    
    FILE *file = fopen(argv[1], "r");
    if(file == NULL){
        perror("Error opening file");
        return -1;
    }

    char col1[INPUT_LENGTH][INT_LENGTH + 1];
    int col2[INPUT_LENGTH];
    int i = 0;
    while(fscanf(file, "%5s %d", col1[i], &col2[i]) == 2) {
        i++;
    }

    int tot = 0;
    HashTable *ht = init_hash_table();
    for(int i = 0; i < INPUT_LENGTH; i++) {
        int duplicity_score = retrieve(col1[i], ht);
        if(duplicity_score != -1) {
            tot += duplicity_score;
            continue;
        }
        int duplicity_num = atoi(col1[i]);
        duplicity_score = calc_duplicity(duplicity_num, col2);
        tot += duplicity_score;
        add_key(col1[i], duplicity_score, ht);
    }
    free_hash_table(ht);
    printf("Total Duplicity: %d \n", tot);
    return 0;
}
