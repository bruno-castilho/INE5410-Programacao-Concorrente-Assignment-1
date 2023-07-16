#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "store.h"
#include "args.h"
#include "customer.h"
#include "sushi_chef.h"
#include "hostess.h"
#include "globals.h"


int main (int argc, char** argv) {
    /* Read command line options */
    config_t config = parse(argc, argv);

    /* Setup random seed */
    srand(time(0));

    /*Setup store*/

    globals_set_restaurant_store(store_init());

    /* Setup simulation virtual clock */
    globals_set_virtual_clock(virtual_clock_init(&config));

    /* Setup sushi conveyor belt */
    globals_set_conveyor_belt(conveyor_belt_init(&config));

    /* Init the Sushi Chef */
    sushi_chef_t* sushi_chef = sushi_chef_init();

    /* Setup customer queue */
    globals_set_queue(queue_init());

    /* Init the Hostess */
    globals_set_hostess(hostess_init());


    /* Join threads and free used memory */

    sushi_chef_finalize(sushi_chef);
    globals_finalize();

    /* Print results and free used memory*/
    
    print_store(globals_get_restaurant_store());
    store_finalize(globals_get_restaurant_store());

    return EXIT_SUCCESS;
}
