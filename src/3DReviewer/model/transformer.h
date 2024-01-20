/**
 * @file
 * @brief Header file of the transformer for model
 */
#ifndef SRC_MODEL_TRANSFORMER_H_
#define SRC_MODEL_TRANSFORMER_H_

#include "model/object.h"

namespace s21 {
/**
 * @brief The Transformer class
 * @author tabathae
 * @version 1.0
 * @date 01/2024
 */
class Transformer {
 public:
    /**
   * @brief The Axis enum - enumeration for X, Y, Z axises for more comfortable usage
   */
  enum class Axis { kX = 0, kY = 1, kZ = 2 };

  Transformer() = default;

  /**
   * @brief TranslateObject - truthfully translation function
   * @param obj - reference to the object
   * @param axis - name of the axis
   * @param shift - value of the shift
   */
  void TranslateObject(s21::Object& obj, Axis axis, double shift) noexcept;

  /**
   * @brief RotateObject - truthfully rotation function
   * @param obj - reference to the object
   * @param axis - name of the axis
   * @param angle - value of the angle
   */
  void RotateObject(s21::Object& obj, Axis axis, double angle) noexcept;

  /**
   * @brief ScaleObject - truthfully scaling function
   * @param obj - reference to the object
   * @param scale
   */
  void ScaleObject(s21::Object& obj, double scale) noexcept;
 private:


  void RotateOxObject(s21::Object& obj, double angle) noexcept;
  void RotateOyObject(s21::Object& obj, double angle) noexcept;
  void RotateOzObject(s21::Object& obj, double angle) noexcept;
};

}

#endif  // SRC_MODEL_TRANSFORMER_H_
