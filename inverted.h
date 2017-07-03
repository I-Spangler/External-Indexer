/*
 * inverted.h
 * Copyright (C) 2017 Ingrid Spangler
 *
 * Distributed under terms of the MIT license.
 */

#ifndef INVERTED_H
#define INVERTED_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sort.h"
#define WORD_MAX_SIZE 20

void make_index(FILE* blob, FILE** index, int document, int* WordCount);
void read_index(Register** Page, FILE* index, int mem);
void add_frequency(FILE* ordered, FILE** final);

#endif /* !INVERTED_H */
