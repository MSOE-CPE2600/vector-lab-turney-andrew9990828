/********************************************************************
* File: vector_dynamic.c
* Author: biebera@msoe.edu <Andrew Bieber>
* Class: CPE 2600
* Date: 10/27/25
*
* Summary:
*   Implements dynamic memory management for the vector calculator.
*   Provides functions to initialize, resize, clear, and free
*   dynamically allocated vector storage.
*
*   NOTE: All functional code was written solely by Andrew Bieber.
*   ChatGPT (AI) was used only for documentation guidance and
*   Javadoc-style comment generation to ensure clarity and readability.
********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vector_dynamic.h"

/**
 * @brief Initializes a dynamic vector store with default capacity (10).
 * 
 * Allocates initial memory for both the vector array (`data`) and 
 * tracking array (`set`). Exits the program if memory allocation fails.
 *
 * @param vs Pointer to the VectorStore structure to initialize.
 */
void init_store(VectorStore *vs) {
    vs->count = 10;
    vs->capacity = 10;
    vs->data = malloc(vs->capacity * sizeof(vector));
    vs->set = calloc(vs->capacity, sizeof(int));
    if (!vs->data || !vs->set) {
        fprintf(stderr, "init_store: malloc failed\n");
        exit(1);
    }
}

/**
 * @brief Ensures that the store has capacity for a given index.
 *
 * Dynamically reallocates both `data` and `set` arrays when
 * the index exceeds current capacity. Capacity is doubled until
 * the index can be accommodated.
 *
 * @param vs Pointer to the VectorStore structure.
 * @param index The index that must be accommodated.
 * @return int 1 if successful, 0 if reallocation fails.
 */
int ensure_capacity(VectorStore *vs, int index) {
    if (index < vs->capacity) return 1;

    int newcap = vs->capacity * 2;
    while (newcap <= index) newcap *= 2;

    vector *newdata = realloc(vs->data, newcap * sizeof(vector));
    int *newset = realloc(vs->set, newcap * sizeof(int));

    if (!newdata || !newset) return 0;

    for (int i = vs->capacity; i < newcap; i++) newset[i] = 0;

    vs->data = newdata;
    vs->set = newset;
    vs->capacity = newcap;

    return 1;
}

/**
 * @brief Sets or updates a vector at a specified index.
 *
 * Automatically calls ensure_capacity() to expand storage if necessary.
 * Marks the vector as "set" and updates the overall count.
 *
 * @param vs Pointer to the VectorStore structure.
 * @param index Index of the vector to store.
 * @param v The vector value to set.
 */
void set_vector(VectorStore *vs, int index, vector v) {
    if (!ensure_capacity(vs, index)) {
        fprintf(stderr, "set_vector: realloc failed\n");
        exit(1);
    }
    vs->data[index] = v;
    vs->set[index] = 1;
    if (index >= vs->count) vs->count = index + 1;
}

/**
 * @brief Retrieves a vector from the store.
 * 
 * @param vs Pointer to the VectorStore structure.
 * @param index Index of the desired vector.
 * @return vector The stored vector.
 */
vector get_vector(VectorStore *vs, int index) {
    return vs->data[index];
}

/**
 * @brief Checks if a vector at a given index has been defined.
 * 
 * @param vs Pointer to the VectorStore structure.
 * @param index Index to check.
 * @return int 1 if vector is defined, 0 otherwise.
 */
int is_set(VectorStore *vs, int index) {
    return (index < vs->capacity) ? vs->set[index] : 0;
}

/**
 * @brief Clears all vectors, setting them to (0,0,0) and marking unset.
 * 
 * This does not free memory, but resets all values for reuse.
 *
 * @param vs Pointer to the VectorStore structure.
 */
void clear_vectors(VectorStore *vs) {
    for (int i = 0; i < vs->capacity; i++) {
        vs->set[i] = 0;
        vs->data[i] = (vector){0, 0, 0};
    }
}

/**
 * @brief Frees all dynamically allocated memory within the store.
 * 
 * Sets pointers to NULL and resets capacity and count to zero.
 *
 * @param vs Pointer to the VectorStore structure.
 */
void free_store(VectorStore *vs) {
    free(vs->data);
    free(vs->set);
    vs->data = NULL;
    vs->set = NULL;
    vs->capacity = vs->count = 0;
}
