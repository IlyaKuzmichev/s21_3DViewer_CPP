#ifndef SRC_MODEL_OBJECT_H_
#define SRC_MODEL_OBJECT_H_

#include <vector>

namespace s21 {

struct Vertex {
    Vertex() : coords({0, 0, 0}) {}
    Vertex(double x, double y, double z) {
        coords.resize(3);
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
    }
    std::vector<double> coords;
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
