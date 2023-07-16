#include <stdio.h>
#include <stdlib.h>

#include "virtual_clock.h"
#include "customer.h"
#include "globals.h"


void* customer_run(void* arg) {
    /* 
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  A PRIMEIRA AÇÃO REALIZADA SERÁ ESPERAR NA FILA GLOBAL DE CLIENTES, ATÉ QUE O HOSTESS
            GUIE O CLIENTE PARA UM ASSENTO LIVRE.
        2.  APÓS SENTAR-SE, O CLIENTE COMEÇARÁ PEGAR E COMER OS PRATOS DA ESTEIRA.
        3.  O CLIENTE SÓ PODERÁ PEGAR UM PRATO QUANDO A ESTEIRA ESTIVER PARADA.
        4.  O CLIENTE SÓ PEGARÁ PRATOS CASO ELE DESEJE-OS, INFORMAÇÃO CONTIDA NO ARRAY self->_wishes[...].
        5.  APÓS CONSUMIR TODOS OS PRATOS DESEJADOS, O CLIENTE DEVERÁ SAIR IMEDIATAMENTE DA ESTEIRA.
        6.  QUANTO O RESTAURANTE FECHAR, O CLIENTE DEVERÁ SAIR IMEDIATAMENTE DA ESTEIRA. 
        7.  CASO O CLIENTE ESTEJA COMENDO QUANDO O SUSHI SHOP FECHAR, ELE DEVE TERMINAR DE COMER E EM SEGUIDA
            SAIR IMEDIATAMENTE DA ESTEIRA.
        8.  LEMBRE-SE DE TOMAR CUIDADO COM ERROS DE CONCORRÊNCIA!
    */ 
    customer_t* self = (customer_t*) arg;
    conveyor_belt_t* conveyor = globals_get_conveyor_belt();
    virtual_clock_t* virtual_clock = globals_get_virtual_clock();

    /*espera sentar em alguma mesa*/
    while (self->_seat_position == -1){
        if (virtual_clock->current_time >= virtual_clock->closing_time)
            break;
    }
    

    while (TRUE){ 
        if (virtual_clock->current_time >= virtual_clock->closing_time)
            break;
        
        /*Tenta pegar a comida, se conseguir há come*/
        for(int i = -1; i <= 1; i++){
            int pick_position = self->_seat_position + i;
            if (pick_position < conveyor->_size)
                customer_eat(self, customer_pick_food(self, pick_position));
        }
        
        /*Verifica sé ja comeu tudo que deseja*/
        int finish = TRUE;
        for (int i = 0; i < 5; i++){
            if (self->_wishes[i] > 1)
                finish = FALSE;
        }

        if (finish){
            store* restaurant_store  = globals_get_restaurant_store();
            pthread_mutex_lock(&restaurant_store->_satisfied_customers_mutex);
            restaurant_store->_satisfied_customers += 1;
            pthread_mutex_unlock(&restaurant_store->_satisfied_customers_mutex);
            break;
        }
        
    }
    
    if(self->_seat_position != -1)
        customer_leave(self);

    pthread_exit(NULL);


}

int customer_pick_food(customer_t* self, int food_slot) {
    /* 
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  O CLIENTE SÓ PODE COMEÇAR A PEGAR COMIDA APÓS ESTAR SENTADO EM UMA VAGA DA ESTEIRA.
        2.  O CLIENTE SÓ SENTARÁ QUANDO O HOSTESS ATUALIZAR O VALOR customer_t->_seat_position.
        3.  SE VOCÊ AINDA NÃO IMPLEMENTOU O HOSTESS, COMECE POR ELE (VEJA O ARQUIVO `hostess.c`)!
        4.  O CLIENTE PODERÁ PEGAR COMIDA DE TRÊS POSSÍVEIS SLOTS: {i-1, i, i+1}, ONDE i É O ÍNDICE 
            POSICIONAL DO CLIENTE NA ESTEIRA (O ASSENTO ONDE ELE ESTÁ SENTADO).
        5.  NOTE QUE CLIENTES ADJACENTES DISPUTARÃO OS MESMOS PRATOS. CUIDADO COM PROBLEMAS DE SINCRONIZAÇÃO!
    */
   
    conveyor_belt_t* conveyor_belt = globals_get_conveyor_belt();   


    pthread_mutex_lock(&conveyor_belt->_food_slots_mutex_list[food_slot]);
    int food = conveyor_belt->_food_slots[food_slot];
    
    if (food != -1){
        
        if(self->_wishes[food] > 0){
            conveyor_belt->_food_slots[food_slot] = -1;
        pthread_mutex_unlock(&conveyor_belt->_food_slots_mutex_list[food_slot]);
            return food;
        }
    }

    pthread_mutex_unlock(&conveyor_belt->_food_slots_mutex_list[food_slot]);
    return -1;
}

void customer_eat(customer_t* self, enum menu_item food) {
    /*
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  ESSA FUNÇÃO JÁ VEM COM PARTE DO CÓDIGO PRONTA (OS SLEEPS PARA CADA TIPO DE ALIMENTO).
        2.  LEMBRE-SE DE DECREMENTAR OS ITENS DA LISTA DE DESEJOS DO CLIENTE CONFORME ELE CONSUMIR OS PRATOS.
        3.  A LISTA DE DESEJOS DO CLIENTE É UM ARRAY COM AS QUANTIDADES DESEJADAS DE CADA PRATO.
        4.  CADA PRATO DO MENU (VER ENUM `menu_item` NO ARQUIVO `menu.h` É REPRESENTADO POR UM INTEIRO),
            ENTÃO UM self->_wishes = [0,0,1,2,0] CONDIZ COM O DESEJO DE COMER 1 RAMÉN E 2 ONIGUIRIS.
    */

    /* INSIRA SUA LÓGICA AQUI */

    store* restaurant_store  = globals_get_restaurant_store();

    if (food == -1)
        return;

    self->_wishes[food] -= 1;
    pthread_mutex_lock(&restaurant_store->_eaten_mutex[food]);
    restaurant_store->_eaten[food] += 1;
    pthread_mutex_unlock(&restaurant_store->_eaten_mutex[food]);

    
    /* NÃO EDITE O CONTEÚDO ABAIXO */
    virtual_clock_t* global_clock = globals_get_virtual_clock();
    switch (food) {
        case Sushi:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Sushi!\n", self->_id);
            msleep(SUSHI_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Sushi!\n", self->_id);
            break;
        case Dango:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Dango!\n", self->_id);
            msleep(DANGO_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Dango!\n", self->_id);
            break;
        case Ramen:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Ramen!\n", self->_id);
            msleep(RAMEN_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Ramen!\n", self->_id);
            break;
        case Onigiri:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Onigiri!\n", self->_id);
            msleep(ONIGIRI_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Onigiri!\n", self->_id);
            break;
        case Tofu:
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d started eating Tofu!\n", self->_id);
            msleep(TOFU_PREP_TIME/global_clock->clock_speed_multiplier);
            print_virtual_time(globals_get_virtual_clock());
            fprintf(stdout, GREEN "[INFO]" NO_COLOR " Customer %d finished eating Tofu!\n", self->_id);
            break; 
        default:
            fprintf(stdout, RED "[ERROR] Invalid menu_item variant passed to `customer_eat()`.\n" NO_COLOR);
            exit(EXIT_FAILURE);
    }
}

void customer_leave(customer_t* self) {
    /*
        MODIFIQUE ESSA FUNÇÃO PARA GARANTIR O COMPORTAMENTO CORRETO E EFICAZ DO CLIENTE.
        NOTAS:
        1.  ESSA FUNÇÃO DEVERÁ REMOVER O CLIENTE DO ASSENTO DO CONVEYOR_BELT GLOBAL QUANDO EXECUTADA.
    */
    conveyor_belt_t* conveyor_belt = globals_get_conveyor_belt();
    conveyor_belt->_seats[self->_seat_position] = -1;
    
}

customer_t* customer_init() {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    customer_t* self = malloc(sizeof(customer_t));
    if (self == NULL) {
        fprintf(stdout, RED "[ERROR] Bad malloc() at `customer_t* customer_init()`.\n" NO_COLOR);
        exit(EXIT_FAILURE);
    }
    self->_id = rand() % 1000;
    for (int i=0; i<=4; i++) {
        self->_wishes[i] = (rand() % 4);
    }
    self->_seat_position = -1;
    pthread_create(&self->thread, NULL, customer_run, (void *) self);
    

    return self;
}

void customer_finalize(customer_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    pthread_join(self->thread, NULL);
    free(self);
    self = NULL;
}

void print_customer(customer_t* self) {
    /* NÃO PRECISA ALTERAR ESSA FUNÇÃO */
    print_virtual_time(globals_get_virtual_clock());
    fprintf(stdout, BROWN "[DEBUG] Customer " NO_COLOR "{\n");
    fprintf(stdout, BROWN "    _id" NO_COLOR ": %d\n", self->_id);
    fprintf(stdout, BROWN "    _wishes" NO_COLOR ": [%d, %d, %d, %d, %d]\n", self->_wishes[0], self->_wishes[1], self->_wishes[2], self->_wishes[3], self->_wishes[4]);
    fprintf(stdout, BROWN "    _seat_position" NO_COLOR ": %d\n", self->_seat_position);
    fprintf(stdout, NO_COLOR "}\n" NO_COLOR);
}
