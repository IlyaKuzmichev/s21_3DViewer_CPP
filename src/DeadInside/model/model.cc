#include "model/model.h"

#include <algorithm>
#include <cmath>
#include <utility>

void s21::ModelHandler::LoadObject(const std::string& file) {
  if (file == file_) {
    return;
  }
  file_ = file;

  ObjectFileParser parse(file);
  Object temp = parse.ParseFile();
  std::swap(temp, initialState_);
  params_.Init();
  returnState_ = initialState_;
}

const s21::Object& s21::ModelHandler::GetObject() const noexcept {
  return returnState_;
}

void s21::ModelHandler::SetRotation(Axis axis, double angle) noexcept {
  params_.rotation[axis] = angle;
  CountNewPosition();
}

void s21::ModelHandler::SetTranslation(Axis axis, double shift) noexcept {
  TranslateObject(axis, shift - params_.translation[axis]);
  params_.translation[axis] = shift;
}

void s21::ModelHandler::SetScale(double scale) noexcept {
  params_.scale = scale;
  CountNewPosition();
}

void s21::ModelHandler::CountNewPosition() {
  returnState_ = initialState_;
  RotateOxObject();
  RotateOyObject();
  RotateOzObject();
  ScaleObject();
  TranslateObject();
}

void s21::ModelHandler::TranslateObject(Axis axis, double shift) {
  for (uint32_t i = 0; i < returnState_.vertices.size() / 3; ++i) {
    returnState_.vertices[i + axis] += shift;
  }
}

void s21::ModelHandler::TranslateObject() {
  for (uint32_t i = 0; i < returnState_.vertices.size(); ++i) {
    returnState_.vertices[i] += params_.translation[i % 3];
  }
}

void s21::ModelHandler::RotateOxObject() {
  double angle = params_.rotation[0];
  auto& vert = returnState_.vertices;
  for (uint32_t i = 0; i < vert.size(); i += 3) {
    double y = vert[i + 1];
    double z = vert[i + 2];
    vert[i + 1] = y * std::cos(angle) - z * std::sin(angle);
    vert[i + 2] = y * std::sin(angle) + z * std::cos(angle);
  }
}

void s21::ModelHandler::RotateOyObject() {
  double angle = params_.rotation[1];
  auto& vert = returnState_.vertices;
  for (uint32_t i = 0; i < vert.size(); i += 3) {
    double x = vert[i];
    double z = vert[i + 2];
    vert[i] = x * std::cos(angle) + z * std::sin(angle);
    vert[i + 2] = -x * std::sin(angle) + z * std::cos(angle);
  }
}

void s21::ModelHandler::RotateOzObject() {
  double angle = params_.rotation[2];
  auto& vert = returnState_.vertices;
  for (uint32_t i = 0; i < vert.size(); i += 3) {
    double x = vert[i];
    double y = vert[i + 1];
    vert[i] = x * std::cos(angle) - y * std::sin(angle);
    vert[i + 1] = x * std::sin(angle) + y * std::cos(angle);
  }
}

void s21::ModelHandler::ScaleObject() {
  for (auto& point : returnState_.vertices) {
    point *= params_.scale;
  }
}

void s21::ModelHandler::NormalizeObject() {
  double maxModulo =
      std::max({std::fabs(initialState_.xMax), std::fabs(initialState_.xMin),
                std::fabs(initialState_.yMax), std::fabs(initialState_.yMin)});

  for (auto& point : initialState_.vertices) {
    point /= maxModulo;
  }
}
