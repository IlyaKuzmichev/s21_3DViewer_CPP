#ifndef SRC_MODEL_MODEL_H
#define SRC_MODEL_MODEL_H

/* ==========================================================================*/
/* Copyright 2023 © Moscow, Russian Federaion
 *
 * This file contains Original Code created by
 * Kirill Safin aka tabathae,
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
 * @author tabathae & wilmerno
 * @mainpage
 * 3dViewer_v2.0
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

#include <array>
#include <string>
#include <vector>

#include "model/parser.h"

namespace s21 {

enum Axis { kAxisX = 0, kAxisY = 1, kAxisZ = 2 };

struct ObjectParameters {
  void Init() {
    translation = {0};
    rotation = {0};
    scale = 1;
  }

  std::array<double, 3> translation;
  std::array<double, 3> rotation;
  double scale;
};

class ModelHandler {
 public:
  void LoadObject(const std::string& file);
  const Object& GetObject() const noexcept;
  void SetRotation(Axis axis, double angle) noexcept;
  void SetTranslation(Axis axis, double shift) noexcept;
  void SetScale(double scale) noexcept;

 private:
  void CountNewPosition();
  void TranslateObject(Axis axis, double shift);
  void TranslateObject();
  void RotateOxObject();
  void RotateOyObject();
  void RotateOzObject();
  void ScaleObject();
  void NormalizeObject();
  Object initialState_;
  Object returnState_;
  ObjectParameters params_;
  std::string file_;
};

}  // namespace s21

#endif  // SRC_MODEL_MODEL_H