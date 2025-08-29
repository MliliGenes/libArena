#include "../include/lib_arena.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct s_data
{
    char *name;
    int *values;
} t_data;

/* Finalizer for t_data */
void data_finalizer(void *ptr)
{
    t_data *d = (t_data *)ptr;
    if (d->name)
        free(d->name);
    if (d->values)
        free(d->values);
    printf("Finalized t_data at %p\n", d);
}

int main(void)
{
    Collector *c = collector_init(2);
    if (!c)
        return (1);

    /* Allocate simple ints */
    int *x = collector_start(c, sizeof(int));
    int *y = collector_start(c, sizeof(int) * 3);
    *x = 10;
    y[0] = 1; y[1] = 2; y[2] = 3;

    /* Allocate a nested struct */
    t_data *d1 = collector_start(c, sizeof(t_data));
    d1->name = malloc(20);
    strcpy(d1->name, "First Data");
    d1->values = malloc(sizeof(int) * 5);
    for (int i = 0; i < 5; i++) d1->values[i] = i;
    collector_register_finalizer(c, d1, data_finalizer);

    t_data *d2 = collector_start(c, sizeof(t_data));
    d2->name = malloc(30);
    strcpy(d2->name, "Second Data Longer Name");
    d2->values = malloc(sizeof(int) * 3);
    for (int i = 0; i < 3; i++) d2->values[i] = i * 10;
    collector_register_finalizer(c, d2, data_finalizer);

    /* Take a checkpoint */
    size_t cp1 = collector_checkpoint(c);

    /* Allocate temporary memory in scope */
    char *tmp = collector_start(c, 50);
    strcpy(tmp, "Temporary buffer in scope");
    printf("tmp content: %s\n", tmp);

    /* Reallocation test */
    y = collector_realloc(c, y, sizeof(int) * 6);
    y[3] = 4; y[4] = 5; y[5] = 6;

    /* End first scope: frees tmp only */
    collector_scope_end(c, cp1);

    /* Allocate more after scope */
    int *z = collector_start(c, sizeof(int));
    *z = 999;

    /* Manual free x */
    collector_free_ptr(c, x);

    /* End collector: finalizes d1, d2, y, z */
    collector_end(c);

    printf("Advanced test finished safely.\n");
    return 0;
}