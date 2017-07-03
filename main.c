/*
 * inverted.c
 * Copyright (C) 2017 Ingrid Spangler
 *
 * Distributed under terms of the MIT license.
 */

#include "inverted.h"
#include "sort.h"
#define PATH_MAX_SIZE 256

int main(){
    int mem, conv, i, capacity, WordCount = 0;
    char dir_I[PATH_MAX_SIZE], dir_C[PATH_MAX_SIZE], fullPath[PATH_MAX_SIZE];
    FILE *blob, *indexed, *ordered, *final;
    Register* Page;
    //input conversas, memoria, arquivos das conversas, arquivo final
    scanf("%d %d %s %s", &conv, &mem, dir_C, dir_I);
    strcat(dir_I, "index");
    
    indexed = fopen("indexed", "w+b");
    
    capacity = mem/sizeof(Register);
    Page = calloc(sizeof(Register), capacity);

    //le conversas, printa no indexed
    for (i = 1; i <= conv; i++) {
        sprintf(fullPath, "%s%d", dir_C, i);
        blob = fopen(fullPath, "r");
        make_index(blob, &indexed, i, &WordCount);
        fclose(blob);
    }
    fflush(indexed);

    //ordena
    ordered = fopen("indexed", "r+b");
    QuickSortExt(indexed, &ordered, Page, capacity, 0, WordCount-1);
    fflush(ordered);
    
    //conta frequencia
    final = fopen(dir_I, "w+");
    add_frequency(ordered, &final);
    
    free(Page);
    fclose(indexed);
    fclose(ordered);
    fclose(final);
    
    return 0;
}
