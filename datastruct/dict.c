#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include "dict.h"

static unsigned long aval_size[23] = {
    21911,       43853,        87719,       175447,      350899,
    701819,      1403641,      2807303,     5614657,     11229331,
    22458671,    44917381,     89834777,    179669557,   359339171,
    718678369,   1437356741,   2147483647
};

static int rehash(dict_t *dict);
static int expand(dict_t *dict);
static void free_ht(dictht_t *ht);
static int init_dictht(dict_t *dict, int index, int size);
static void init_crypt(unsigned long *crypt);
static unsigned long hash(dict_t *dict, char *key, int type);
static int get_pos(dict_t *dict, char *key, int type);

dict_t *dict_init()
{
    dict_t *dict = calloc(1, sizeof(dict_t));
    if (dict == NULL)
        return NULL;
    dict->rehashing = 0;
    dict->index = 0;
    init_crypt(dict->crypt);
    if (init_dictht(dict, 0, aval_size[0]) == -1)
        return NULL;
    return dict;
}

int dict_push(dict_t *dict, char *key, void *data)
{
    dictht_t *ht = &dict->ht[dict->index];
    if (ht->size <= ht->used * 2)
        expand(dict);
    if (dict->rehashing != 1 && rehash(dict) == -1)
        return -1;
    ht = &dict->ht[dict->index];
    int pos = get_pos(dict, key, INSERT);
    ht->node[pos].exist = EXIST;
    strncpy(ht->node[pos].key, key, strlen(key));
    ht->node[pos].data = data;
    ht->used++;
    return 0;
}

int dict_delete(dict_t *dict, char *key)
{
    if (rehash(dict) == -1)
        return -1;
    int pos = get_pos(dict, key, SEARCH);
    dictht_t *ht = &dict->ht[dict->index];
    if (ht->node[pos].exist == NON)
        return -1;
    ht->node[pos].exist = DELETE;
    memset(ht->node[pos].key, 0, strlen(key));
    ht->used--;
    return 0;
}

int dict_search(dict_t *dict, char *key, void **result)
{
    if (rehash(dict) == -1)
        return -1;
    int pos = get_pos(dict, key, SEARCH);
    dictht_t *ht = &dict->ht[dict->index];
    if (ht->node[pos].exist == EXIST) {
        *result = ht->node[pos].data;
        return 0;
    }
    return -1;
}

void dict_free(dict_t *dict)
{
    free_ht(&dict->ht[dict->index]);
    free(dict);
}

static void free_ht(dictht_t *ht)
{
    int i;
    for (i = 0; i < ht->size; i++)
        if (ht->node[i].exist != NON) {
            ht->node[i].exist = NON;
            free(ht->node[i].data);
        }
    free(ht->node);
    ht->size = ht->used = 0;
}

static int rehash(dict_t *dict)
{
    int i;
    dictht_t *ht = &dict->ht[1 - dict->index];
    if (ht->used <= 0)
        return 0;
    dict->rehashing = 1;
    for (i = ht->used - 1; i > ht->used - 101 && i >= 0; i--)
        if (ht->node[i].exist == EXIST
                && trp_dict_push(dict, ht->node[i].key, ht->node[i].data) == -1) {
            dict->rehashing = 0;
            return -1;
        }
    ht->used = (ht->used - 100 > 0) ? ht->used - 100 : 0;
    dict->rehashing = 0;
    if (ht->used == 0)
        free(dict->ht[1 - dict->index].node);
    return 0;
}

static int expand(dict_t *dict)
{
    int i;
    for (i = 0; i < 23; i++)
        if (aval_size[i] > dict->ht[dict->index].size)
            break;
    if (init_dictht(dict, 1 - dict->index, aval_size[i]) == -1)
        return -1;
    dict->ht[dict->index].used = dict->ht[dict->index].size;
    dict->index = 1 - dict->index;
    if (rehash(dict) == -1)
        return -1;
    return 0;
}

static int init_dictht(dict_t *dict, int index, int size)
{
    dictht_t *ht = &dict->ht[index];
    ht->size = size;
    ht->used = 0;
    ht->node = calloc(size, sizeof(htnode_t));
    if (ht->node == NULL)
        return -1;
    return 0;
}

static void init_crypt(unsigned long *crypt)
{
    unsigned long seed = 0x00100001, index1 = 0, index2 = 0;
    unsigned long i, temp1, temp2;
    for (index1 = 0; index1 < 0x100; index1++)
        for (index2 = index1, i = 0; i < 5; i++, index2 += 0x100) {
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);
            crypt[index2] = (temp1 | temp2);
        }
}

static unsigned long hash(dict_t *dict, char *key, int type)
{
    unsigned char *ukey = (unsigned char *)key;
    unsigned long seed1 = 0x7FED7FED;
    unsigned long seed2 = 0xEEEEEEEE;
    int ch;
    while (*ukey) {
        ch = toupper(*ukey++);
        seed1 = dict->crypt[(type << 8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    return seed1;
}

static int get_pos(dict_t *dict, char *key, int type)
{
    unsigned long value = hash(dict, key, 0);
    dictht_t *ht = &dict->ht[dict->index];
    int start = value % ht->size;
    int pos = start;
    if (type == INSERT)
        while (ht->node[pos].exist == EXIST)
            pos = (pos + 100) % ht->size;
    else if (type == SEARCH)
        while (ht->node[pos].exist != NON)
            if (ht->node[pos].exist != DELETE
                    && strcmp(ht->node[pos].key, key) == 0)
                break;
            else
                pos = (pos + 100) % ht->size;
    //printf("pos: %d\n", pos);
    return pos;
}


