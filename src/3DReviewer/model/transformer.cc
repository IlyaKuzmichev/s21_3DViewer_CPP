#include "model/transformer.h"

#include <cmath>

#include "model/object.h"

// static int axisInt(s21::Transformer::Axis axis) noexcept {
//     return static_cast<int>(axis);
// }

void s21::Transformer::TranslateObject(s21::Object& obj, Axis axis,
                                       double shift) noexcept {
  for (auto& v : obj.vertices) {
    if (axis == Axis::kX) {
      v.x += shift;
    } else if (axis == Axis::kY) {
      v.y += shift;
    } else {
      v.z += shift;
    }
  }
}

void s21::Transformer::RotateObject(s21::Object& obj, Axis axis,
                                    double angle) noexcept {
  switch (axis) {
    case Axis::kX:
      RotateOxObject(obj, angle);
      break;
    case Axis::kY:
      RotateOyObject(obj, angle);
      break;
    case Axis::kZ:
      RotateOzObject(obj, angle);
      break;
  }
}

void s21::Transformer::ScaleObject(s21::Object& obj, double scale) noexcept {
  for (auto& v : obj.vertices) {
    v.x *= scale;
    v.y *= scale;
    v.z *= scale;
  }
}

void s21::Transformer::RotateOxObject(s21::Object& obj, double angle) noexcept {
  double angle_cos = std::cos(angle);
  double angle_sin = std::sin(angle);
  for (auto& v : obj.vertices) {
    double y = v.y;
    double z = v.z;
    v.y = y * angle_cos + z * angle_sin;
    v.z = -y * angle_sin + z * angle_cos;
  }
}

void s21::Transformer::RotateOyObject(s21::Object& obj, double angle) noexcept {
  double angle_cos = std::cos(angle);
  double angle_sin = std::sin(angle);
  for (auto& v : obj.vertices) {
    double x = v.x;
    double z = v.z;
    v.x = x * angle_cos + z * angle_sin;
    v.z = -x * angle_sin + z * angle_cos;
  }
}

void s21::Transformer::RotateOzObject(s21::Object& obj, double angle) noexcept {
  double angle_cos = std::cos(angle);
  double angle_sin = std::sin(angle);
  for (auto& v : obj.vertices) {
    double x = v.x;
    double y = v.y;
    v.x = x * angle_cos + y * angle_sin;
    v.y = -x * angle_sin + y * angle_cos;
  }
}
