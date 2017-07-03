/*
 * sort.c
 * Copyright (C) 2017 Ingrid Spangler
 *
 * Distributed under terms of the MIT license.
 */

#include "sort.h"


int compareR(Register a, Register b){

    //compara a palavra
    int r = strcmp(a.Word, b.Word);
    if(r == 0){
        //compara o documento
        r = a.Document - b.Document;
        if(r == 0)
            //compara a posição
            r = a.Location - b.Location;
    }
    return r;
}

void copyR(Register* a, Register* b){
    //copia b em a
    strcpy(a->Word, b->Word);
    a->Document = b->Document;
    a->Location = b->Location;
    a->Frequency = b->Frequency;
}

void Sort(Register* Page, int bot, int top){
    //quicksort
    Register pivot, aux;
    int low_, up_, mid;
    low_ = bot; up_ = top;
    mid = (bot + top)/2;
    pivot = Page[mid];

    do{
        //encontra um elemento da parte de baixo (bot) maior do que o pivo
        while(compareR(Page[low_], pivot) < 0) low_++;
        //encontra um elemento da parte de cima (top) menor do que o pivo
        while(compareR(Page[up_], pivot) > 0) up_--;
        //troca os dois
        if(low_ <= up_){
            aux = Page[up_];
            Page[up_] = Page[low_];
            Page[low_] = aux;
            low_++;
            up_--;
        }
    }while(up_ >= low_);
    //se os limites não se encontrarem, recursão
    if(bot < low_ - 1) Sort(Page, bot, low_ -1);
    if(low_ < top) Sort(Page, low_, top);

    return;
}

void PartialSort(Register** Page, int capacity){
    //insertion sort
    int reg, d;
    Register aux;
    for (reg = 1; reg < capacity; reg++) {
        d = reg;
        while(d > 0 && compareR((*Page)[d - 1], (*Page)[d]) > 0){
            copyR(&aux, &(*Page)[d-1]);
            (*Page)[d - 1] = (*Page)[d];
            copyR(&(*Page)[d], &aux);
            d--;
        }
    }
}

void QuickSortExt(FILE* indexed, FILE** ordered, Register* Page, int capacity, int inf, int sup){
    
    int i, buffer = 0;
    int *rptr, *wptr, ir_ptr = inf, iw_ptr = inf, sw_ptr = sup, sr_ptr = sup;
    int ip_ptr = inf - 1, sp_ptr = sup + 1;
    Register aux;

    //condição de parada
    if(sup - inf < 1)
        return;
    //read pointer, write pointer começam no inferior
    rptr = &ir_ptr;
    wptr = &iw_ptr;
    //preenche a memória interna
    while(buffer < capacity && ir_ptr <= sr_ptr){
        //le um registro para a página
        fseek(indexed, *rptr*sizeof(Register), SEEK_SET);
        fread(&aux, sizeof(Register), 1, indexed);
        Page[buffer] = aux;
        buffer++;
        //avança o ponteiro atual
        *rptr = (rptr == &sr_ptr ? --(sr_ptr) : ++(ir_ptr));
        //troca os ponteiros de leitura
        rptr = (rptr == &sr_ptr ? &ir_ptr : &sr_ptr);
    }
    
    //ordena a página
    Sort(Page, 0, buffer - 1);

    //escreve
    while(ir_ptr <= sr_ptr){
        
        //se os ponteiros estiverem juntos, lê para evitar sobrescrever
        if(ir_ptr == iw_ptr)
            rptr = &ir_ptr;
        else if(sw_ptr == sr_ptr)
            rptr = &sr_ptr;

        //lê no registro auxiliar 
        fseek(indexed, *rptr*sizeof(Register), SEEK_SET);
        fread(&aux, sizeof(Register), 1, indexed);
       
        //update & toggle
        *rptr = (rptr == &sr_ptr ? --(sr_ptr) : ++(ir_ptr));
        rptr = (rptr == &sr_ptr ? &ir_ptr : &sr_ptr);
        
        //se aux for menor que o menor do buffer escreve no inferior write
        if(compareR(aux, Page[0]) < 0){
            fseek(*ordered, iw_ptr*sizeof(Register), SEEK_SET);
            fwrite(&aux, sizeof(Register), 1, *ordered);
            //atualiza o partition pointer
            ip_ptr = iw_ptr;
            ++(iw_ptr);
        }
        //se for maior escreve no superior write
        else if(compareR(aux, Page[buffer-1]) > 0){
            fseek(*ordered, sw_ptr*sizeof(Register), SEEK_SET);
            fwrite(&aux, sizeof(Register), 1, *ordered);
            //atualiza o partition pointer
            sp_ptr = sw_ptr;
            --(sw_ptr);
        }
        //se estiver no meio escreve o extremo correspondente no ponteiro menos avançado,
        //substitui e ordena
        else{ 
            wptr = ((sup - sw_ptr) - (iw_ptr - inf) > 0) ? &iw_ptr : &sw_ptr;
            i = (wptr == &iw_ptr ? 0: buffer - 1);
            
            fseek(*ordered, *wptr*sizeof(Register), SEEK_SET);
            fwrite(&Page[i], sizeof(Register), 1, *ordered);
            copyR(&Page[i], &aux);
            
            *wptr = (wptr == &sw_ptr ? --(sw_ptr) : ++(iw_ptr));
            //mantem ordenado
            PartialSort(&Page, buffer);
        }
    }
    
    //escreve o que ficou no buffer
    fseek(*ordered, iw_ptr*sizeof(Register), SEEK_SET);
    fwrite(Page, sizeof(Register), buffer, *ordered);
    fflush(*ordered);

    //recursão
    QuickSortExt(*ordered, ordered, Page, capacity, inf, ip_ptr);
    QuickSortExt(*ordered, ordered, Page, capacity, sp_ptr, sup);

}
