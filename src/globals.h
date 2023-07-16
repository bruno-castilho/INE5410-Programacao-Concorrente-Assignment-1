#ifndef __GLOBALS_H__
#define __GLOBALS_H__


#include "conveyor_belt.h"
#include "virtual_clock.h"
#include "queue.h"
#include "customer.h"
#include "hostess.h"
#include "store.h"



/**
 * @brief Inicia um relógio virtual (de modo global)
 * 
 * @param virtual_clock 
 */
extern void globals_set_virtual_clock(virtual_clock_t *virtual_clock);

/**
 * @brief Retorna um relógio virtual (de modo global)
 * 
 * @return virtual_clock_t* 
 */
extern virtual_clock_t *globals_get_virtual_clock();

/**
 * @brief Inicia uma esteira de sushi (de modo global).
 * 
 * @param conveyor_belt
 */
extern void globals_set_conveyor_belt(conveyor_belt_t *conveyor_belt);

/**
 * @brief Retorna uma esteira de sushi (de modo global)
 * 
 * @return conveyor_belt_t* 
 */
extern conveyor_belt_t *globals_get_conveyor_belt();

/**
 * @brief Inicia uma fila (de modo global)
 * 
 * @param queue 
 */
extern void globals_set_queue(queue_t *queue);

/**
 * @brief Retorna uma fila (de modo global)
 * 
 * @return queue_t* 
 */
extern queue_t *globals_get_queue();

/**
 * @brief Inicia hostess (de modo global)
 * 
 * @param hostess 
 */
extern void globals_set_hostess (hostess_t* hostess);

/**
 * @brief Retorna hostess (de modo global)
 * 
 * @return hostess_t* 
 */
extern hostess_t *globals_get_hostess();

extern void globals_set_restaurant_store (store* _store);

extern store* globals_get_restaurant_store ();

/**
 * @brief Finaliza todas as variáveis globais.
 * 
 */

extern void globals_finalize();

#endif  // __GLOBALS_H__
