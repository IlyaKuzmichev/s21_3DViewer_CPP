#ifndef SRC_MODEL_TRANSFORMER_H_
#define SRC_MODEL_TRANSFORMER_H_

#include "model/object.h"

namespace s21 {

class Transformer {
 public:
  enum class Axis { kX = 0, kY = 1, kZ = 2 };

  Transformer() = default;
  void TranslateObject(s21::Object& obj, Axis axis, double shift) noexcept;
  void RotateObject(s21::Object& obj, Axis axis, double angle) noexcept;
  void ScaleObject(s21::Object& obj, double scale) noexcept;
 private:
  void RotateOxObject(s21::Object& obj, double angle) noexcept;
  void RotateOyObject(s21::Object& obj, double angle) noexcept;
  void RotateOzObject(s21::Object& obj, double angle) noexcept;
};

}

#endif  // SRC_MODEL_TRANSFORMER_H_
