#include <cmath>
#include <stdexcept>
#include <utility>

#include "exception.h"
#include "object_builder.h"

s21::ObjectBuilder& s21::ObjectBuilder::AddVertice(const s21::Vertice v) {
   obj_.vertices.push_back(v);
   return *this;
}

s21::ObjectBuilder& s21::ObjectBuilder::AddRawFace(const s21::RawFace rf) {
    obj_.faces.push_back(ConvertRawFace(rf));
    return *this;
}

s21::Object s21::ObjectBuilder::Build() noexcept {
    s21::Object result;
    std::swap(result, obj_);
    return result;
}

s21::Face s21::ObjectBuilder::ConvertRawFace(const s21::RawFace rf) {
    s21::Face f;
    f.vertices_indices.reserve(rf.vertices_indices.size());

    for (const auto& idx : rf.vertices_indices) {
        size_t real_idx = 0;
        if (idx == 0 || (static_cast<size_t>(std::abs(idx) - 1)) >= obj_.vertices.size()) {
            throw Exception("vertice index is out of range");
        }

        if (idx < 0) {
            real_idx = static_cast<size_t>(obj_.vertices.size() + idx);
        } else {
            real_idx = idx - 1;
        }
        f.vertices_indices.push_back(real_idx);
    }

    return f;
}