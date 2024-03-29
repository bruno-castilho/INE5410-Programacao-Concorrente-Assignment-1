#include <stdlib.h>

#include "globals.h"



/*
    VOCÊ DEVE CRIAR VARIÁVEIS GLOBAIS PARA ARMAZENAR DADOS SOBRE A SIMULAÇÃO.
    NOTAS:
    1.  OS SEGUINTES DADOS DEVEM SER ARMAZENADOS E PRINTADOS AO FIM DA SIMULAÇÃO:
        a. Quantidade de clientes que sentaram-se no conveyor e consumiram todos os itens desejados
        b. Quantidades produzidas de cada alimento
        c. Quantidades consumidas de cada alimento
    2.  SIGA OS EXEMPLOS DE VARIÁVEIS GLOBAIS JÁ EXISTENTES NESSE ARQUIVO PARA CRIAR AS NOVAS.
*/


virtual_clock_t* global_virtual_clock = NULL;
conveyor_belt_t* global_conveyor_belt = NULL;
queue_t* global_queue = NULL;
hostess_t* global_hostess = NULL;
store* restaurant_store = NULL;



void globals_set_virtual_clock(virtual_clock_t* virtual_clock) {
    global_virtual_clock = virtual_clock;
}

virtual_clock_t* globals_get_virtual_clock() {
    return global_virtual_clock;
}

void globals_set_conveyor_belt(conveyor_belt_t* conveyor_belt) {
    global_conveyor_belt = conveyor_belt;
}

conveyor_belt_t* globals_get_conveyor_belt() {
    return global_conveyor_belt;
}

void globals_set_queue(queue_t* queue) {
    global_queue = queue;
}

queue_t* globals_get_queue() {
    return global_queue;
}

void globals_set_hostess (hostess_t* hostess) {
    global_hostess = hostess;
}

hostess_t* globals_get_hostess () {
    return global_hostess;
}

void globals_set_restaurant_store (store* _store) {
    restaurant_store = _store;
}

store*  globals_get_restaurant_store () {
    return restaurant_store;
}
/**
 * @brief Finaliza todas as variáveis globais.
 * Se criar alguma variável global que faça uso de mallocs, lembre-se sempre 
 * de usar o free dentro dessa função.
 */
void globals_finalize() {

    virtual_clock_finalize(global_virtual_clock);
    hostess_finalize(global_hostess);
    queue_finalize(global_queue);   
    conveyor_belt_finalize(global_conveyor_belt); 

    
}
