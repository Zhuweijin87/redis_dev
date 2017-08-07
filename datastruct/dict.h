#ifndef _DICT_H
#defien _DICT_H

#define MAXLEN 1024

#define INSERT 1
#define SEARCH 2

#define NON 0
#define DELETE 1
#define EXIST 2

typedef struct {
    int exist;
    char key[MAXLEN];
    void *data;
} htnode_t;

typedef struct {
    int size;
    int used;
    htnode_t *node;
} dictht_t;

typedef struct {
    int rehashing;
    int index;
    dictht_t ht[2];
    unsigned long crypt[0x500];
} dict_t;

dict_t *dict_init();
int dict_push(dict_t *dict, char *key, void *data);
int dict_delete(dict_t *dict, char *key);
int dict_search(dict_t *dict, char *key, void **result);
void dict_free(dict_t *dict);

#endif
