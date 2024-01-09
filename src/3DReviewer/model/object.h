#ifndef SRC_MODEL_OBJECT_H_
#define SRC_MODEL_OBJECT_H_

#include <vector>

namespace s21 {

struct Vertex {
    Vertex() : x(0), y(0), z(0) {}
    Vertex(double x, double y, double z) : x(x), y(y), z(z) {
    }
    double x;
    double y;
    double z;
};

struct Face {
    Face() = default;
    Face(std::vector<std::size_t> vi) : vertices_indices(vi) {}
    std::vector<std::size_t> vertices_indices;
};

struct Object {
    Object() {}

    std::vector<Vertex> vertices;
    std::vector<Face> faces;
};

}

#endif  // SRC_MODEL_OBJECT_H_
