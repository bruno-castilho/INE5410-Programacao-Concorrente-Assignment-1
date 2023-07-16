#include <stdlib.h>
#include <time.h>

#include "store.h"
#include "globals.h"
#include "args.h"
#include "menu.h"
#include "conveyor_belt.h"

store* store_init() {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    store* self =  malloc(sizeof(store));
    
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `store* store_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<=4; i++) {
        self->_eaten[i] = 0;
        self->_produced[i] = 0;
        pthread_mutex_init(&self->_eaten_mutex[i], NULL);
        pthread_mutex_init(&self->_produced_mutex[i], NULL);
    }

    self->_satisfied_customers = 0;
    self->_first = NULL;
    self->_last = NULL;
    self->_length = 0;

    pthread_mutex_init(&self->_satisfied_customers_mutex, NULL);

    return self;
}

void store_finalize(store* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    free(self);
}

void print_store(store* self) {

    fprintf(stdout, 
            GREEN "[RESULT]" NO_COLOR " Sushis produzido: %d%s %d%s %d%s %d%s %d%s\n", 
            self->_produced[0], SUSHI,
            self->_produced[1], DANGO,
            self->_produced[2], RAMEN,
            self->_produced[3], ONIGIRI,
            self->_produced[4], TOFU );

    fprintf(stdout, 
            GREEN "[RESULT]" NO_COLOR " Sushis comido: %d%s %d%s %d%s %d%s %d%s\n", 
            self->_eaten[0], SUSHI,
            self->_eaten[1], DANGO,
            self->_eaten[2], RAMEN,
            self->_eaten[3], ONIGIRI,
            self->_eaten[4], TOFU);                                                                         


    fprintf(stdout, GREEN "[RESULT]" NO_COLOR " Clientes satisfeito: %d%s \n",self->_satisfied_customers, CUSTOMER);
}
