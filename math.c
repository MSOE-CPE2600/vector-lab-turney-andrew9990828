/********************************************************************
* File: math.c
* Author: biebera@msoe.edu 
* Class: CPE 2600
* Date: 9/30/25
*
* Summary:
*   Implements vector math operations: add, subtract,
*   dot product, cross product, and scalar multiplication.
* NOTE: CHATGPT ONLY USED FOR COMMENTS/DOCUMENTATION ALL CODE
* WRITTEN BY ME AND ONLY ME ANDREW BIEBER
********************************************************************/

#include "math.h"

/**
 * Adds two vectors component-wise.
 */
vector add(vector a, vector b) {
    vector r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

/**
 * Subtracts vector b from vector a component-wise.
 */
vector subtract(vector a, vector b) {
    vector r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

/**
 * Computes the dot product of two vectors.
 */
double dotproduct(vector a, vector b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

/**
 * Computes the cross product of two vectors.
 */
vector crossproduct(vector a, vector b) {
    vector r;
    r.x = a.y * b.z - a.z * b.y;
    r.y = a.z * b.x - a.x * b.z;
    r.z = a.x * b.y - a.y * b.x;
    return r;
}

/**
 * Multiplies each component of a vector by a scalar.
 */
vector scalarmultiplication(vector a, double s) {
    vector r;
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    return r;
}
