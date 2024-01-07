#ifndef SRC_MODEL_TESTS_UTILS_H_
#define SRC_MODEL_TESTS_UTILS_H_

#include "model/object.h"

#include <array>
#include <ostream>

namespace s21test {

class TestableObject : public s21::Object {
    public:
        TestableObject() : s21::Object() {}
        TestableObject(s21::Object obj) : s21::Object(obj) {}
        friend bool operator==(const TestableObject& a, const TestableObject& b);

        friend void PrintTo(const TestableObject& obj, std::ostream* os);
};

s21::Object ConstructTestCube(std::array<s21::Vertice, 8> vertices);
s21::Object ConstructBasicTestCube();


}


#endif // SRC_MODEL_TESTS_UTILS_H_
