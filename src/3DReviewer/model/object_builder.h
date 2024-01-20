#ifndef SRC_MODEL_OBJECT_BUILDER_H_
#define SRC_MODEL_OBJECT_BUILDER_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "model/object.h"

namespace s21 {

struct RawFace {
  RawFace() = default;
  RawFace(std::vector<int32_t> vertices_indices) : vertices_indices(vertices_indices) {}
  std::vector<int32_t> vertices_indices;
};

class ObjectBuilder {
 public:
  ObjectBuilder() : obj_(std::make_unique<Object>(Object())) {}
  ObjectBuilder(size_t vertices_amount, size_t faces_amount) {
    obj_ = std::make_unique<Object>(Object());
    obj_->vertices.reserve(vertices_amount);
    obj_->faces.reserve(faces_amount);
  }

  ObjectBuilder& AddVertice(const Vertex v);
  ObjectBuilder& AddRawFace(const RawFace f);
  Object* Build() noexcept;

 private:
  std::unique_ptr<Object> obj_;

  Face ConvertRawFace(const RawFace f);
};

}  // namespace s21

#endif  // SRC_MODEL_OBJECT_BUILDER_H_
