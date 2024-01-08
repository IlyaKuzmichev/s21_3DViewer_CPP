#ifndef SRC_MODEL_VIEWER_H_
#define SRC_MODEL_VIEWER_H_

#include <array>
#include <string>
#include <unordered_map>

#include "model/object.h"
#include "model/transformer.h"

namespace s21 {
class Viewer {
 public:
  using Axis = Transformer::Axis;

  void LoadObject(const std::string& filepath);
  const Object& GetObject() const noexcept;

  void SetRotation(Axis axis, double angle) noexcept;
  void SetTranslation(Axis axis, double shift) noexcept;
  void SetScale(double scale) noexcept;

  double GetTranslation(Axis axis) const noexcept;
  double GetRotation(Axis axis) const noexcept;
  double GetScale() const noexcept;

 private:
  void RecountCurrentState();
  void TranslateObject(Axis axis, double shift);
  void TranslateObject();
  void RotateOxObject();
  void RotateOyObject();
  void RotateOzObject();
  void ScaleObject();

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

  Object baseState_;
  Object currentState_;
  ObjectParameters params_;
  std::string current_file_;
  Transformer transformer_;
};
}  // namespace s21

#endif  // SRC_MODEL_VIEWER_H_
