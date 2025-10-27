/********************************************************************
* File: vector_dynamic.h
* Author: biebera@msoe.edu <Andrew Bieber>
* Class: CPE 2600
* Date: 10/27/25
*
* Summary:
*   Header file defining the dynamic vector storage structure and
*   associated functions for memory management.
*
*   NOTE: All implementation code authored solely by Andrew Bieber.
*   ChatGPT (AI) was used exclusively for generating Javadoc-style
*   documentation and descriptive comments.
********************************************************************/

#ifndef VECTOR_DYNAMIC_H
#define VECTOR_DYNAMIC_H

#include "math.h"

/**
 * @struct VectorStore
 * @brief Represents a dynamically allocated collection of vectors.
 *
 * This structure holds an expandable array of vector structs
 * and a corresponding "set" array used to track which indices
 * currently contain valid vectors.
 */
typedef struct {
    vector *data;   /**< Pointer to dynamically allocated vector array. */
    int count;      /**< Current count of assigned vectors. */
    int capacity;   /**< Total allocated capacity of the store. */
    int *set;       /**< Boolean-like array tracking assigned indices. */
} VectorStore;

/**
 * @brief Initializes the vector store with default capacity (10).
 * @param vs Pointer to the VectorStore structure to initialize.
 */
void init_store(VectorStore *vs);

/**
 * @brief Frees all allocated memory associated with the store.
 * @param vs Pointer to the VectorStore structure.
 */
void free_store(VectorStore *vs);

/**
 * @brief Ensures that the store has capacity for the given index.
 * @param vs Pointer to the VectorStore structure.
 * @param index The required index.
 * @return 1 on success, 0 on failure.
 */
int ensure_capacity(VectorStore *vs, int index);

/**
 * @brief Stores or updates a vector at a given index.
 * @param vs Pointer to the VectorStore structure.
 * @param index Index of the vector.
 * @param v The vector to set.
 */
void set_vector(VectorStore *vs, int index, vector v);

/**
 * @brief Retrieves a vector from a given index.
 * @param vs Pointer to the VectorStore structure.
 * @param index Index to retrieve.
 * @return The stored vector.
 */
vector get_vector(VectorStore *vs, int index);

/**
 * @brief Checks whether a vector exists at the specified index.
 * @param vs Pointer to the VectorStore structure.
 * @param index Index to check.
 * @return 1 if set, 0 otherwise.
 */
int is_set(VectorStore *vs, int index);

/**
 * @brief Clears all vectors in the store (does not free memory).
 * @param vs Pointer to the VectorStore structure.
 */
void clear_vectors(VectorStore *vs);

#endif /* VECTOR_DYNAMIC_H */
