/********************************************************************
* File: math.h
* Author: biebera@msoe.edu <Andrew Bieber>
* Class: CPE 2600
* Date: 9/30/25
*
* Summary:
*   Header file for vector operations in 3D space.
*   Declares the vector struct and function prototypes
*   for add, subtract, dot product, cross product,
*   and scalar multiplication.
* NOTE: CHATGPT ONLY USED FOR COMMENTS/DOCUMENTATION ALL CODE
* WRITTEN BY ME AND ONLY ME ANDREW BIEBER
********************************************************************/

#ifndef MATH_H
#define MATH_H

/**
 * Structure representing a 3D vector.
 */
typedef struct vectors {
    double x;
    double y;
    double z;
} vector;

/**
 * Adds two vectors component-wise.
 */
vector add(vector a, vector b);

/**
 * Subtracts vector b from vector a component-wise.
 */
vector subtract(vector a, vector b);

/**
 * Computes the dot product of two vectors.
 * @return scalar value (double)
 */
double dotproduct(vector a, vector b);

/**
 * Computes the cross product of two vectors.
 * @return vector perpendicular to a and b
 */
vector crossproduct(vector a, vector b);

/**
 * Multiplies each component of a vector by a scalar.
 */
vector scalarmultiplication(vector a, double scalar);

#endif /* MATH_H */
