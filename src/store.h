#ifndef __STORE_H__
#define __STORE_H__
#include "customer.h"

struct queue_customer {
    customer_t* _customer;  
    struct queue_customer* _next;  
};


typedef struct store {
    int _eaten[5];  
    int _produced[5];
    int _satisfied_customers;
    int _length;
    struct queue_customer *_first; 
    struct queue_customer *_last;
    pthread_mutex_t _eaten_mutex[5];
    pthread_mutex_t _produced_mutex[5];
    pthread_mutex_t _satisfied_customers_mutex;
} store; 


store* store_init();
void store_finalize(store* self);
void print_store(store* self);

#endif