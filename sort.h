/*
 * sort.h
 * Copyright (C) 2017 Ingrid Spangler
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SORT_H
#define SORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Register{
    char Word[20];
    int Document;
    int Location;
    int Frequency;
}Register;

void Sort(Register* Page, int bot, int top);
void PartialSort(Register** Page, int capacity);
int compareR(Register a, Register b);
void copyR(Register* a, Register* b);
void QuickSortExt(FILE* indexed, FILE** ordered, Register* Page, int capacity, int inf, int sup);

#endif /* !SORT_H */
