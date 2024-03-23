/* Include the system headers we need */
#include <stdlib.h>
#include <stdio.h>

/* Include our header */
#include "vector.h"

/* Define what our struct is */
struct vector_t {
    size_t size;
    int *data;
};

/* Bad example of how to create a new vector */
vector_t *bad_vector_new() {
    /* Create the vector and a pointer to it */
    vector_t *retval, v;
    retval = &v;

    /* Initialize attributes */
    retval->size = 1;
    retval->data = malloc(sizeof(int));
    if (retval->data == NULL) {
        fprintf(stderr, "Error: malloc() failed in bad_vector_new()\n");
        exit(1);
    }

    retval->data[0] = 0;
    return retval;
}

/* Another suboptimal way of creating a vector */
vector_t also_bad_vector_new() {
    /* Create the vector */
    vector_t v;

    /* Initialize attributes */
    v.size = 1;
    v.data = malloc(sizeof(int));
    if (v.data == NULL) {
        fprintf(stderr, "Error: malloc() failed in also_bad_vector_new()\n");
        exit(1);
    }
    v.data[0] = 0;
    return v;
}

/* Create a new vector with a size (length) of 1 and set its single component to zero... the right way */
/* TODO: uncomment the code that is preceded by // */
vector_t *vector_new() {
    /* Declare what this function will return */
    vector_t *retval;

    /* First, we need to allocate memory on the heap for the struct */
    retval = malloc(sizeof(int));

    /* Check our return value to make sure we got memory */
    if (retval == NULL) {
        fprintf(stderr, "Error: malloc() failed in vector_new()\n");
        exit(1);
    }

    /* Now we need to initialize our data.
       Since retval->data should be able to dynamically grow,
       what do you need to do? */
    retval->size = 1;
    retval->data = malloc(sizeof(int));

    /* Check the data attribute of our vector to make sure we got memory */
    if (retval->data == NULL) {
        free(retval); // Why is this line necessary?
        fprintf(stderr, "Error: malloc() failed in vector_new()\n");
        exit(1);
    }

    /* Complete the initialization by setting the single component to zero */
    retval->data[0] = 0;

    /* and return... */
    return retval; /* UPDATE RETURN VALUE */
}

/* Return the value at the specified location/component "loc" of the vector */
int vector_get(vector_t *v, size_t loc) {
    /* If we are passed a NULL pointer for our vector, complain about it and exit. */
    if(v == NULL) {
        fprintf(stderr, "vector_get: passed a NULL vector.\n");
        exit(1);
    }

    // If the requested location is higher than we have allocated, return 0.
    // Otherwise, return what is in the passed location.
    if (loc > v->size-1){
        return 0;
    } else {
        return v->data[loc];
    }
}

/* Free up the memory allocated for the passed vector.
   Remember, you need to free up ALL the memory that was allocated. */
void vector_delete(vector_t *v) {
    /* YOUR CODE HERE */
    free(v->data);
    free(v);
}

/* Set a value in the vector. If the extra memory allocation fails, call exit(1). */
void vector_set(vector_t *v, size_t loc, int value) {
    // What do you need to do if the location is greater than the size we have allocated?
    // Remember that unset locations should contain a value of 0.

    /* YOUR CODE HERE */
    if(v == NULL) {
        fprintf(stderr, "vector_set: passed a NULL vector.\n");
        exit(1);
    }
    if (loc < v->size){
        v->data[loc] = value;
    } else {
        v->data = (int *) realloc(v->data, sizeof(int) * (loc + 1));
        if (v->data == NULL) {
            exit(1);
        } else {
            for (int i = v->size; i < loc; i += 1) {
                v->data[i] = 0;
            }
            v->data[loc] = value;
            v->size = loc + 1;
        }
    }
}
