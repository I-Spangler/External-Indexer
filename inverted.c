/*
 * inverted.c
 * Copyright (C) 2017 Ingrid Spangler
 *
 * Distributed under terms of the MIT license.
 */

#include "inverted.h"

void make_index(FILE* blob, FILE** index, int document, int* WordCount){
    int location = 0;
    Register a;
    a.Document = document;
    a.Frequency = 1;

    char word[WORD_MAX_SIZE];
    for (int i = 0; i < WORD_MAX_SIZE; i++) a.Word[i] = '\0';

    //le palavra para registro, escreve no binário
    while(fscanf(blob, "%s", word) != EOF){
        strcpy(a.Word, word);
        a.Location = location;
        fwrite(&a, sizeof(Register), 1, *index);
        location += strlen(word)+1;
        //retorna numero de palavras
        ++(*WordCount);
    }
}

void add_frequency(FILE* ordered, FILE** final){
    int frq = 1, pos = 0, i;
    Register a, b;
    fseek(ordered, 0, SEEK_SET);
    //le
    while(fread(&a, sizeof(Register), 1, ordered) == 1){
        //se não for EOF
        //le as palavras subsequentes até que (word, document) diferente apareça
        while(fread(&b, sizeof(Register), 1, ordered) == 1 
              && strcmp(a.Word, b.Word) == 0 
              && a.Document == b.Document)
        { 
            frq++;
        }
        
        //volta na primeira da sequencia
        fseek(ordered, pos, SEEK_SET);
        //escreve todas com a frequencia correta
        for (i = 0; i < frq; i++){
            fread(&a, sizeof(Register), 1, ordered);
            a.Frequency = frq;
            fprintf(*final, "%s,%d,%d,%d\n", a.Word, a.Document, a.Frequency, a.Location);
        }
        //passa pra proxima palavra
        pos += frq * (sizeof(Register));
        //reseta frequencia
        frq = 1; 
    }
}
