/**
 * read items from a file of items
 * Each item will be on three lines in the file.
 * The first line is it's name (less than 80 characters).
 * The second line is it's weight (a decimal number)
 * The third line is the items value
 *
 * Take these items and fit them into a knapsack with a limited capacity. We do
 * this by creating multiple threads and trying every possible combination of
 * items
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/**
 * A struct defining an item with a name and a weight
 */
typedef struct _item {
    char name[80];
    double weight;
    double value;
} item_t;

/**
 * Given k and a list of items, finds all possible ways to select k items from
 * the set.
 *
 * Returns an array containing all of these combinations, it is the user's
 * responsibility to free the memory associated with this array
 */
item_t *** configuration_maker(size_t k, item_t * items, size_t num_items, size_t * num_confs) {
    if (k < 1) { // shouldn't go below zero for size_t
        *num_confs = 0;
        return NULL;
    }
    if (k == 1) {
        // there are num_items ways to pick 1 item
        item_t *** conf = malloc( 2 * num_items * sizeof *** conf );
        for (size_t i = 0; i < num_items; ++i) {
            size_t ii = i + num_items;
            conf[i] = malloc( sizeof ** conf );
            conf[ii] = malloc( sizeof ** conf );

            conf[i][0] = &items[i];
            conf[ii][0] = NULL;
        }
        *num_confs = num_items * 2;
        return conf;
    }
    // get older configurations
    size_t prev_num_confs = -1;
    item_t *** previous_conf = configuration_maker(k-1, items + 1, num_items - 1, &prev_num_confs);

    // I will have 2 * number of prev_num_confs
    item_t *** my_config = malloc( 2 * prev_num_confs * sizeof *** my_config );

    for (size_t i = 0; i < prev_num_confs; ++i) {
        size_t ii = i + prev_num_confs;
        my_config[i] = malloc( k * sizeof ** my_config );
        my_config[ii] = malloc( k * sizeof ** my_config );

        for (size_t j = 0; j < k-1; ++j) {
            my_config[i][j] = previous_conf[i][j];
            my_config[ii][j] = previous_conf[i][j];
        }
        my_config[i][k-1] = NULL;
        my_config[ii][k-1] = items;
        free(previous_conf[i]);
    }
    free(previous_conf);

    *num_confs = 2 * prev_num_confs;
    return my_config;
}


double configuration_value(item_t ** conf, size_t len) {
    double s = -1;
    for (size_t i = 0; i < len; ++i) {
        if (conf[i] != NULL) {
            s += conf[i]->value;
        }
    }
    return s;
}

double configuration_weight(item_t ** conf, size_t len) {
    double s = 0;
    for (size_t i = 0; i < len; ++i) {
        if (conf[i] != NULL) {
            s += conf[i]->weight;
        }
    }
    return s;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "usage: knapsack item_list weight_limit\n");
        fprintf(stderr, "where: item_list:    file specifying items to fit into knapsack\n");
        fprintf(stderr, "       weight_limit: maximum amount of weight the knapsack can hold\n");
        return -1;
    }

    // open the file
    const char * fname = argv[1];
    FILE * fp = fopen(fname, "r");
    if (fp == NULL) {
        fprintf(stderr, "Something went wrong while opening the file!\n");
        return -1;
    }

    double max_weight = atof(argv[2]);

    // We aren't sure how many items are in the file. So, we will make enough
    // space for a limited set of items, if the array fills up we can make it
    // larger later

    size_t space = 10;
    size_t used = 0;
    item_t * items = malloc( space * sizeof * items );

    // stuff for getline and temporary storage
    char * name_str   = NULL;
    char * weight_str = NULL;
    char * value_str  = NULL;
    size_t n_name     = 0;
    size_t n_weight   = 0;
    size_t n_value    = 0;
    ssize_t read;

    while ( (read = getline(&name_str, &n_name, fp)) != -1 ) {
        // read one item out of the file
        // at this point line holds the items name, now we need to get the
        // item's weight and value
        if ((read = getline(&weight_str, &n_weight, fp)) == -1) {
            fprintf(stderr, "Invalid input format\n");
        }
        if ((read = getline(&value_str, &n_value, fp)) == -1) {
            fprintf(stderr, "Invalid input format\n");
        }

        // convert types
        double my_weight = atof(weight_str);
        double my_value = atof(value_str);

        // strip newline off of name
        name_str[ strlen(name_str) - 1 ] = '\0';


        // check if we have enough space to hold this item
        if (used == space - 1) {
            // we are out of space
            space = space*2;
            items = realloc(items, space * sizeof * items);
        }

        // store the item in the array
        strcpy(items[used].name, name_str);
        items[used].weight = my_weight;
        items[used].value = my_value;

        used += 1;
    }
    // free the things getline allocated
    free(name_str);
    free(weight_str);
    free(value_str);

    printf("Items read from input file:\n");
    for (size_t i = 0; i < used; ++i) {
        printf("%s with weight %f and value %f\n",
                items[i].name, items[i].weight, items[i].value);
    }

    printf("\n");

    size_t options = -1;
    size_t k = used;
    item_t *** confs = configuration_maker(k, items, used, &options);
    item_t ** best_conf = NULL;
    double best_value = -1;
    for (size_t i = 0; i < options; ++i) {
        double mw = configuration_weight(confs[i], k);
        double mv = configuration_value(confs[i], k);
        //printf("mw: %f mv: %f\n", mw, mv);
        if (mw <= max_weight) {
            if (mv > best_value) {
                free(best_conf);
                best_conf = confs[i];
                best_value = mv;
            }
        } else {
            free(confs[i]);
        }
    }

    if (best_conf != NULL) {
        printf("The winning configuration is:\n");
        for (size_t i = 0; i < k; ++i) {
            if (best_conf[i] != NULL) {
                printf("%s weight: %f value %f\n", best_conf[i]->name, best_conf[i]->weight, best_conf[i]->value);
            }
        }
        free(best_conf);
    } else {
        printf("Couldn't find any valid configurations\n");
    }
    free(confs);

    // cleanup!
    free(items);
    fclose(fp);
    return 0;
}
