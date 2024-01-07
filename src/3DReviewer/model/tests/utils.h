#ifndef SRC_MODEL_TESTS_UTILS_H_
#define SRC_MODEL_TESTS_UTILS_H_

#include "model/object.h"

namespace s21test {

class TestableObject : public s21::Object {
    public:
        TestableObject() : s21::Object() {}
        TestableObject(s21::Object obj) : s21::Object(obj) {}
        friend bool operator==(const TestableObject& a, const TestableObject& b);
};


}


#endif // SRC_MODEL_TESTS_UTILS_H_
