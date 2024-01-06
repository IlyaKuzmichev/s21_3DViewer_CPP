#ifndef SRC_MODEL_OBJECT_H_
#define SRC_MODEL_OBJECT_H_

#include <array>
#include <vector>

namespace s21 {

struct Vertice {
    Vertice() : coords({0, 0, 0}) {}
    Vertice(double x, double y, double z) : coords({x, y, z}) {}
    std::array<double, 3> coords;
};

struct Face {
    std::vector<size_t> vertices_indices;
};

struct Object {
    Object() {}

    std::vector<Vertice> vertices;
    std::vector<Face> faces;
};

}

#endif  // SRC_MODEL_OBJECT_H_