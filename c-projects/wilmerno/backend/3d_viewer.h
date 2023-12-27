/* ==========================================================================*/
/* Copyright 2023 © Moscow, Russian Federaion
 *
 * This file contains Original Code created by
 * Michail Kopylov aka sarellad,
 * Igor Baidikov aka lorenttr,
 * Ilya Kuzmichev aka wilmerno.
 *
 * The Original Code and all software developed in the process of
 * participation on learning by experimental programming educational method.
 * The whole methodology was developed and distributed by
 * Autonomous non-profit organization «School 21» (ANO «School 21»).
 *
 * Redistribution and use of this file, its parts, or entire project
 * are permitted by confirmation of its original creators.
 */
/* ==========================================================================*/

/**
 * @author sarellad, lorenttr & wilmerno
 * @mainpage
 * 3dViewer_v1.0, backend realised in C language, gui written
 * on QT Creator
 *
 * Implementation of a program to visualise 3D wireframe models
 *
 * The program provide the ability to:
 * Load a wireframe model from an obj file (vertices and surfaces list support
 * only). Translate the model by a given distance in relation to the X, Y, Z
 * axes. Rotate the model by a given angle relative to its X, Y, Z axes. Scale
 * the model by a given value.
 *
 * The program allow customizing the type of projection (parallel and central)
 * The program allow setting up the type (solid, dashed), color and thickness
 * of the edges, display method (none, circle, square), color and size of the
 * vertices
 * The program allow choosing the background color
 * Settings saved between program restarts
 *
 * The program allow saving the captured (rendered) images as bmp and jpeg
 * files. The program allow recording small screencasts by a special button -
 * the current custom affine transformation of the loaded object into
 * gif-animation (640x480, 10fps, 5s)
 */

#pragma once

#include <stdlib.h>
/**
 * point_t
 * @brief Union for point structure
 */
typedef union point_u {
  /// to call as array
  double arr[3];
  struct {
    /// x coord
    double x;
    /// y coord
    double y;
    /// z coord
    double z;
  };
} point_t;

/**
 * face_t
 * @brief face's structure with various points quantity
 */
typedef struct face_s {
  /// quantity of points
  uint64_t v_count;
  /// dynamicly allocated array of points indexes
  uint64_t* v_array;
} face_t;

/**
 * object_t
 * @brief Parsed .obj structure
 */
typedef struct object_s {
  /// number of vertexes
  uint64_t v_count;
  /// number of faces
  uint64_t f_count;
  /// number of edges;
  uint64_t e_count;
  /// limit values of the object
  double x_min, x_max, y_min, y_max;
  /// matrice of vertexes (each point has 3 coordinates)
  point_t* v_array;
  /// array of faces
  face_t* f_array;
} object_t;

/**
 * axis_t
 * @brief enum axes, with which it is convenient to iterate over an array of
 * points (index + axis == x, y or z coordinate of the point)
 */
typedef enum axis_t { X_AXIS = 0, Y_AXIS, Z_AXIS } axis_t;

// Affine transformations

/**
 * @brief Function to translate model by a given distance
 *
 * @param obj --- pointer to object structure
 * @param axis --- x, y or z
 * @param shift --- value of the shift
 */
void translate_object(object_t* obj, axis_t axis, double shift);

/**
 * @brief Function to rotate model by a given angle relative to its X axis
 *
 * @param obj - pointer to object structure
 * @param angle - value of the angle to rotate
 */
void rotate_ox_object(object_t* obj, double angle);

/**
 * @brief Function to rotate model by a given angle relative to its Y axis
 *
 * @param obj - pointer to object structure
 * @param angle - value of the angle to rotate
 */
void rotate_oy_object(object_t* obj, double angle);

/**
 * @brief Function to rotate model by a given angle relative to its Z axis
 *
 * @param obj - pointer to object structure
 * @param angle - value of the angle to rotate
 */
void rotate_oz_object(object_t* obj, double angle);

/**
 * @brief Function to scale model by a given value
 *
 * @param obj - pointer to object structure
 * @param scale - value to scale
 */
void scale_object(object_t* obj, double scale);

/**
 * @brief Normalize object to show it in Widget 1x1
 *
 * @param obj - object, needs to be normalized
 * @param normalized_obj - pointer to new normalized object
 */

void normalize_object(const object_t obj, object_t* normalized_obj);

// Affine transformations for points

/**
 * @brief Function to translate point by a given distance
 *
 * @param point --- pointer to point structure
 * @param axis --- x, y or z
 * @param shift --- value of the shift
 */
void translate_point(point_t* point, axis_t axis, double shift);

/**
 * @brief Function to find the new position of the point with rotate by X
 * transformation
 *
 * @param point - pointer to point structure
 * @param angle - value of the angle to rotate
 */
void rotate_ox_point(point_t* point, double angle);

/**
 * @brief Function to find the new position of the point with rotate by Y
 * transformation
 *
 * @param point - pointer to point structure
 * @param angle - value of the angle to rotate
 */
void rotate_oy_point(point_t* point, double angle);

/**
 * @brief Function to find the new position of the point with rotate by Z
 * transformation
 *
 * @param point - pointer to point structure
 * @param angle - value of the angle to rotate
 */
void rotate_oz_point(point_t* point, double angle);

/**
 * @brief Function to find the new position of the point with scale
 * transformation
 *
 * @param obj - pointer to point structure
 * @param scale - value to scale
 */
void scale_point(point_t* point, double scale);
