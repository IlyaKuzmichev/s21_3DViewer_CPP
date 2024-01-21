/* ==========================================================================*/
/* Copyright 2024 © Moscow, Russian Federaion
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
 * are permitted by confirmation of its original creator.
 */
/* ==========================================================================*/

/**
 * @author Kirill Safin aka Tabatha Evilhawk
 * @author Ilya Kuzmichev aka Wilmer Norma
 * @mainpage
 * 3dViewer_v2.0, hard OOP and MVC
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

/**
 * @file
 * @brief Header file of main viewer model
 */

#ifndef SRC_MODEL_VIEWER_H_
#define SRC_MODEL_VIEWER_H_

#include <array>
#include <memory>
#include <string>
#include <unordered_map>

#include "model/object.h"
#include "model/transformer.h"

namespace s21 {
/**
 * @brief The Viewer class
 * @author tabathae
 * @author wilmerno
 * @version 1.0
 * @date 01/2024
 */
class Viewer {
 public:
  using Axis = Transformer::Axis;

  Viewer() = default;
  /**
   * @brief LoadObject - function for loading object from file to model
   * @param filepath - name of the object file
   */
  void LoadObject(const std::string& filepath);

  /**
   * @brief GetObject - function to get object from model
   * @return constant reference to the object
   */
  const Object& GetObject() const noexcept;

  /**
   * @brief SetRotation - function to set rotation value
   * @param axis - name of the axis to rotate on
   * @param angle - value of the angle
   */
  void SetRotation(Axis axis, double angle) noexcept;

  /**
   * @brief SetTranslation - function to set translation value
   * @param axis - name of the axis to translate on
   * @param shift - value of the shift
   */
  void SetTranslation(Axis axis, double shift) noexcept;

  /**
   * @brief SetScale - function to set scale value
   * @param scale - value of the scale
   */
  void SetScale(double scale) noexcept;

  /**
   * @brief GetTranslation - getter for translation value by axis
   * @param axis - name of the axis
   * @return value of translation
   */
  double GetTranslation(Axis axis) const noexcept;

  /**
   * @brief GetRotation - getter for rotation value by axis
   * @param axis - name of the axis
   * @return value of rotation
   */
  double GetRotation(Axis axis) const noexcept;

  /**
   * @brief GetScale - getter for scale value
   * @return value of the scale
   */
  double GetScale() const noexcept;

 private:
  /**
   * @brief RecountCurrentState - function, that init recounting current state
   * of the object
   */
  void RecountCurrentState() noexcept;

  /**
   * @brief TranslateObject - function for fast translate of the object only by
   * one axis
   * @param axis - name of the axis
   * @param shift - value of the shift
   */
  void TranslateObject(Axis axis, double shift) noexcept;

  /**
   * @brief TranslateObject - function for translating object from initial state
   * by all axises
   */
  void TranslateObject() noexcept;

  /**
   * @brief RotateOxObject - function for rotating object by X axis
   */
  void RotateOxObject() noexcept;

  /**
   * @brief RotateOyObject function for rotating object by Y axis
   */
  void RotateOyObject() noexcept;

  /**
   * @brief RotateOzObject - function for rotating object by Z axis
   */
  void RotateOzObject() noexcept;

  /**
   * @brief ScaleObject - function for changing scale of the object
   */
  void ScaleObject() noexcept;

  void ResetCurrentState() noexcept;

  /**
   * @brief The ObjectParameters struct Handling parameters of the current
   * object - translation, rotation and scale
   */
  struct ObjectParameters {
    ObjectParameters() { Init(); };
    /**
     * @brief Init - initialization of parameters by zero values
     */
    void Init() {
      translation = {0};
      rotation = {0};
      scale = 1;
    }
    /**
     * @brief translation - static array with parameters of translation by X, Y
     * and Z axis, accordingly
     */
    std::array<double, 3> translation;
    /**
     * @brief rotation - static array with parameters of translation by X, Y and
     * Z axis, accordingly
     */
    std::array<double, 3> rotation;
    double scale;
  };

  /**
   * @brief baseState_ - initial state of the object, parsed from *.obj file
   */
  std::unique_ptr<Object> base_state_;
  /**
   * @brief currentState_ - we keep current position of the object in this
   * variable, because of sometimes we don't need to recount fully new position
   * from initial state, that imptoves the perfomance
   */
  std::shared_ptr<Object> current_state_;
  /**
   * @brief params_ - parameters of current state
   */
  ObjectParameters params_;
  /**
   * @brief current_file_ - name of the file, from witch we keep current object
   * in the model
   */
  std::string current_file_;
  /**
   * @brief transformer_ - class for hardly processing transformation
   */
  Transformer transformer_;
};
}  // namespace s21

#endif  // SRC_MODEL_VIEWER_H_
