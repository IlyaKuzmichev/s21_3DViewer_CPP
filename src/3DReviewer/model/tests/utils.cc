#include "model/tests/utils.h"
#include "model/object.h"

#include <cmath>

static const double kEpsilon = 1e-7;

bool are_numbers_equal(const double a, const double b) {
    return std::fabs(a - b) < kEpsilon;
}

bool are_vertices_equal(const s21::Vertice& a, const s21::Vertice& b) {
    return are_numbers_equal(a.coords[0], b.coords[0]) &&
        are_numbers_equal(a.coords[1], b.coords[1]) &&
        are_numbers_equal(a.coords[2], b.coords[2]); 
}

bool are_faces_equal(const s21::Face& a, const s21::Face& b) {
    if (a.vertices_indices.size() != b.vertices_indices.size()) {
        return false;
    }
    for (size_t i = 0; i < a.vertices_indices.size(); ++i) {
        if (a.vertices_indices[i] != b.vertices_indices[i]) {
            return false;
        }
    }
    return true;
}

namespace s21test {

bool operator==(const s21test::TestableObject& a, const s21test::TestableObject& b) {
    if (a.vertices.size() != b.vertices.size()) {
        return false;
    }
    if (a.faces.size() != b.faces.size()) {
        return false;
    }

    for (size_t i = 0; i < a.vertices.size(); ++i) {
        if (!are_vertices_equal(a.vertices[i], b.vertices[i])) {
            return false;
        }
    }

    for (size_t i = 0; i < a.faces.size(); ++i) {
        if (!are_faces_equal(a.faces[i], b.faces[i])) {
            return false;
        }
    }

    return true;
}

}


