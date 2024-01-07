#ifndef SRC_MODEL_OBJECT_BUILDER_H_
#define SRC_MODEL_OBJECT_BUILDER_H_

#include <cstdint>
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
  ObjectBuilder() : obj_() {}

  ObjectBuilder& AddVertice(const Vertice v);
  ObjectBuilder& AddRawFace(const RawFace f);
  Object Build() noexcept;

 private:
  Object obj_;

  Face ConvertRawFace(const RawFace f);
};

}  // namespace s21

#endif  // SRC_MODEL_OBJECT_BUILDER_H_
