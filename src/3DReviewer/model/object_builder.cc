#include "model/object_builder.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

#include "model/exception.h"

s21::ObjectBuilder& s21::ObjectBuilder::AddVertice(const s21::Vertex& v) {
  SetMaxModuloValue(v);
  obj_->vertices.push_back(v);
  return *this;
}

s21::ObjectBuilder& s21::ObjectBuilder::AddRawFace(const s21::RawFace& rf) {
  obj_->faces.push_back(ConvertRawFace(rf));
  return *this;
}

s21::Object* s21::ObjectBuilder::Build(bool normalize) noexcept {
  if (normalize) {
    NormalizeObject();
  }
  return obj_.release();
}

s21::Face s21::ObjectBuilder::ConvertRawFace(const s21::RawFace& rf) {
  s21::Face f;
  f.vertices_indices.reserve(rf.vertices_indices.size());

  for (const auto& idx : rf.vertices_indices) {
    size_t real_idx = 0;
    if (idx == 0 ||
        (static_cast<size_t>(std::abs(idx) - 1)) >= obj_->vertices.size()) {
      throw Exception("vertice index is out of range");
    }

    if (idx < 0) {
      real_idx = static_cast<size_t>(obj_->vertices.size() + idx);
    } else {
      real_idx = idx - 1;
    }
    f.vertices_indices.push_back(real_idx);
  }

  return f;
}

void s21::ObjectBuilder::SetMaxModuloValue(const s21::Vertex& v) {
  double modulo_x = std::fabs(v.x);
  double modulo_y = std::fabs(v.y);
  double max = std::max(modulo_x, modulo_y);

  if (max > max_coord_modulo_) {
    max_coord_modulo_ = max;
  }
}

void s21::ObjectBuilder::NormalizeObject() {
  for (auto& vertex : obj_->vertices) {
    vertex.x /= max_coord_modulo_;
    vertex.y /= max_coord_modulo_;
    vertex.z /= max_coord_modulo_;
  }
}
