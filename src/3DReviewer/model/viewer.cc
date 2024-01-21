#include "viewer.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QString>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "exception.h"
#include "parser.h"

static const double kVerticesPreallocCoeff = 300'000. / 70'000'000.;
static const double kFacesPreallocCoeff = 300'000. / 70'000'000.;

static int axisInt(s21::Viewer::Axis axis) noexcept {
  return static_cast<int>(axis);
}

void s21::Viewer::LoadObject(const std::string& filepath) {
  QElapsedTimer debug;
  debug.start();

  if (filepath == current_file_) {
    return;
  }

  size_t file_bytes = std::filesystem::file_size(filepath);

  std::ifstream stream(filepath);
  if (!stream.is_open()) {
    throw Exception("Cringe");
  }

  size_t vertices_prealloc = kVerticesPreallocCoeff * file_bytes;
  size_t faces_prealloc = kFacesPreallocCoeff * file_bytes;

  ObjectParser parser(vertices_prealloc, faces_prealloc);
  current_file_ = filepath;
  auto obj = std::unique_ptr<Object>(parser.Parse(stream));
  std::swap(obj, base_state_);

  params_.Init();
  auto new_current_state = std::make_shared<Object>(*base_state_.get());
  std::swap(new_current_state, current_state_);
  qDebug() << "Parsing time: " << debug.elapsed() << '\n';
}

const s21::Object& s21::Viewer::GetObject() const noexcept {
  return *current_state_;
}

void s21::Viewer::SetRotation(Axis axis, double angle) noexcept {
  if (params_.rotation[axisInt(axis)] == angle) {
    return;
  }
  params_.rotation[axisInt(axis)] = angle;
  RecountCurrentState();
}

void s21::Viewer::SetTranslation(Axis axis, double shift) noexcept {
  if (params_.translation[axisInt(axis)] == shift) {
    return;
  }
  TranslateObject(axis, shift);
  params_.translation[axisInt(axis)] = shift;
}

void s21::Viewer::SetScale(double scale) noexcept {
  if (params_.scale == scale) {
    return;
  }
  params_.scale = scale;
  RecountCurrentState();
}

void s21::Viewer::RecountCurrentState() noexcept {
  ResetCurrentState();
  RotateOxObject();
  RotateOyObject();
  RotateOzObject();
  ScaleObject();
  TranslateObject();
}

void s21::Viewer::TranslateObject(Axis axis, double shift) noexcept {
  transformer_.TranslateObject(*current_state_, axis,
                               shift - params_.translation[axisInt(axis)]);
}

void s21::Viewer::TranslateObject() noexcept {
  transformer_.TranslateObject(*current_state_, Axis::kX,
                               params_.translation[axisInt(Axis::kX)]);
  transformer_.TranslateObject(*current_state_, Axis::kY,
                               params_.translation[axisInt(Axis::kY)]);
  transformer_.TranslateObject(*current_state_, Axis::kZ,
                               params_.translation[axisInt(Axis::kZ)]);
}

void s21::Viewer::RotateOxObject() noexcept {
  double angle = params_.rotation[axisInt(Axis::kX)];
  transformer_.RotateObject(*current_state_, Axis::kX, angle);
}

void s21::Viewer::RotateOyObject() noexcept {
  double angle = params_.rotation[axisInt(Axis::kY)];
  transformer_.RotateObject(*current_state_, Axis::kY, angle);
}

void s21::Viewer::RotateOzObject() noexcept {
  double angle = params_.rotation[axisInt(Axis::kZ)];
  transformer_.RotateObject(*current_state_, Axis::kZ, angle);
}

void s21::Viewer::ScaleObject() noexcept {
  transformer_.ScaleObject(*current_state_, params_.scale);
}

double s21::Viewer::GetTranslation(Axis axis) const noexcept {
  return params_.translation[axisInt(axis)];
}

double s21::Viewer::GetRotation(Axis axis) const noexcept {
  return params_.rotation[axisInt(axis)];
}

double s21::Viewer::GetScale() const noexcept { return params_.scale; }

void s21::Viewer::ResetCurrentState() noexcept {
  for (size_t i = 0; i < current_state_->vertices.size(); ++i) {
    current_state_->vertices[i] = base_state_->vertices[i];
  }
  for (size_t i = 0; i < current_state_->faces.size(); ++i) {
    current_state_->faces[i] = base_state_->faces[i];
  }
}
