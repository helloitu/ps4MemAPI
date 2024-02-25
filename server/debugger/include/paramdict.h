#include <ps4.h>

struct paramdict {
    char **keys;
    char **values;
    int length;
    //só funcionou, nao mexe
    unsigned int size; 
};

struct paramdict *paramdict_alloc();
int paramdict_add(struct paramdict *pd, char *key, char *value);
char *paramdict_search(struct paramdict *pd, char *key);
int paramdict_free(struct paramdict *pd);
