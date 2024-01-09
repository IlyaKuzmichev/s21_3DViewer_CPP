#include "model/tests/utils.h"
#include "model/object.h"
#include "model/object_builder.h"

#include <array>
#include <cmath>
#include <vector>
#include <ostream>

static const double kEpsilon = 1e-7;

bool are_numbers_equal(const double a, const double b) {
    return std::fabs(a - b) < kEpsilon;
}

bool are_vertices_equal(const s21::Vertex& a, const s21::Vertex& b) {
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

void PrintTo(const TestableObject& obj, std::ostream* os) {
    *os << "Vertices:" << std::endl;
    for (const auto& v : obj.vertices) {
        *os << "\t(" << v.coords[0] << "," << v.coords[1] << "," << v.coords[2] << ")" << std::endl;
    }
    *os << "Faces:" << std::endl;
    for (const auto& f : obj.faces) {
        *os << "\t(";
        for (size_t i = 0; i < f.vertices_indices.size(); ++i) {
            if (i > 0) {
                *os << ",";
            }
            *os << f.vertices_indices[i];
        }
        *os << ")" << std::endl;
    }
}

}

s21::Object s21test::ConstructTestCube(std::array<s21::Vertex, 8> vertices) {
    s21::ObjectBuilder builder;
    for (auto v : vertices) {
        builder.AddVertice(v);
    }

    s21::RawFace rf(std::vector<int32_t>({1, 2, 3, 4}));
    builder.AddRawFace(rf);

    for (int32_t i = 0; i < 4; ++i) {
        std::vector<int32_t> indices({i + 1, (i + 1) % 4 + 1, i + 5, (i + 1) % 4 + 5});
        builder.AddRawFace(s21::RawFace(indices));
    }

    rf = s21::RawFace(std::vector<int32_t>({5, 6, 7, 8}));
    builder.AddRawFace(rf);

    return builder.Build();
}

s21::Object s21test::ConstructBasicTestCube() {
    std::array<s21::Vertex, 8> vertices({
                                    s21::Vertex(1., 1., 0.),
                                    s21::Vertex(-1., 1., 0.),
                                    s21::Vertex(-1., -1., 0.),
                                    s21::Vertex(1., -1., 0.),
                                    s21::Vertex(1., 1., 2.),
                                    s21::Vertex(-1., 1., 2.),
                                    s21::Vertex(-1., -1., 2.),
                                    s21::Vertex(1., -1., 2.),
                                });
    return ConstructTestCube(vertices);
}
