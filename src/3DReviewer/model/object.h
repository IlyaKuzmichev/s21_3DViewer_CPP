#ifndef SRC_MODEL_OBJECT_H_
#define SRC_MODEL_OBJECT_H_

#include <array>
#include <vector>

namespace s21 {

struct Vertex {
    Vertex() : coords({0, 0, 0}) {}
    Vertex(double x, double y, double z) : coords({x, y, z}) {}
    std::array<double, 3> coords;
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
