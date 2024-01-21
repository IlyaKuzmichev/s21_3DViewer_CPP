#ifndef SRC_MODEL_TESTS_UTILS_H_
#define SRC_MODEL_TESTS_UTILS_H_

#include <array>
#include <ostream>

#include "model/object.h"

namespace s21test {

class TestableObject : public s21::Object {
 public:
  TestableObject() : s21::Object() {}
  explicit TestableObject(s21::Object obj) : s21::Object(obj) {}
  friend bool operator==(const TestableObject& a, const TestableObject& b);

  friend void PrintTo(const TestableObject& obj, std::ostream* os);
};

s21::Object* ConstructTestCube(std::array<s21::Vertex, 8> vertices,
                               bool normalize = false);
s21::Object* ConstructBasicTestCube(bool normalize = false);

}  // namespace s21test

#endif  // SRC_MODEL_TESTS_UTILS_H_
