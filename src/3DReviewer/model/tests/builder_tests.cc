#include "model/exception.h"
#include "model/object.h"
#include "model/object_builder.h"
#include "model/tests/utils.h"

#include <array>
#include <vector>

#include <gtest/gtest.h>

TEST(ObjectBuilderTest, SingleVertice) {
    s21::ObjectBuilder builder;

    builder.AddVertice(s21::Vertex(1., 2., 3.));

    s21test::TestableObject obj(builder.Build());

    std::vector<s21::Vertex> vertices({s21::Vertex(1., 2., 3.)});

    s21test::TestableObject expected;
    expected.vertices = vertices;

    EXPECT_EQ(expected, obj);
}

TEST(ObjectBuilderTest, SeveralVertices) {
    const int n = 10;

    s21::ObjectBuilder builder;

    std::vector<s21::Vertex> expected_vertices;

    for (size_t i = 0; i < n; ++i) {
        double x = i;
        double y = 2 * i;
        double z = 3 * i;

        s21::Vertex v(x, y, z);
        builder.AddVertice(v);
        expected_vertices.push_back(v);
    }

    s21test::TestableObject expected;
    expected.vertices = expected_vertices;

    s21test::TestableObject obj(builder.Build());

    EXPECT_EQ(expected, obj);
}

TEST(ObjectBuilderTest, SingleFace) {
    s21::ObjectBuilder builder;

    builder.
        AddVertice(s21::Vertex(0., 0., 0.)).
        AddVertice(s21::Vertex(0., 1., 0.)).
        AddVertice(s21::Vertex(0., 0., 1.)).
        AddVertice(s21::Vertex(1., 0., 0.));

    std::vector<int32_t> vertices_indices({1, 2, 3, 4});

    builder.AddRawFace(s21::RawFace(vertices_indices));

    std::vector<s21::Vertex> expected_vertices({
                                s21::Vertex(0., 0., 0.),
                                s21::Vertex(0., 1., 0.),
                                s21::Vertex(0., 0., 1.),
                                s21::Vertex(1., 0., 0.),
                              });

    std::vector<s21::Face> expected_faces({
                                s21::Face(std::vector<size_t>({0, 1, 2, 3})),
                           });

    s21test::TestableObject expected;
    expected.vertices = expected_vertices;
    expected.faces = expected_faces;

    s21test::TestableObject obj(builder.Build());

    EXPECT_EQ(expected, obj);
}

TEST(ObjectBuilderTest, SingleFaceNegativeIndices) {
    s21::ObjectBuilder builder;

    builder.
        AddVertice(s21::Vertex(0., 0., 0.)).
        AddVertice(s21::Vertex(0., 1., 0.)).
        AddVertice(s21::Vertex(0., 0., 1.)).
        AddVertice(s21::Vertex(1., 0., 0.));

    std::vector<int32_t> vertices_indices({-4, -3, -2, -1});

    builder.AddRawFace(s21::RawFace(vertices_indices));

    std::vector<s21::Vertex> expected_vertices({
                                s21::Vertex(0., 0., 0.),
                                s21::Vertex(0., 1., 0.),
                                s21::Vertex(0., 0., 1.),
                                s21::Vertex(1., 0., 0.),
                              });

    std::vector<s21::Face> expected_faces({
                                s21::Face(std::vector<size_t>({0, 1, 2, 3})),
                           });

    s21test::TestableObject expected;
    expected.vertices = expected_vertices;
    expected.faces = expected_faces;

    s21test::TestableObject obj(builder.Build());

    EXPECT_EQ(expected, obj);
}

TEST(ObjectBuilderTest, FaceWithZeroIndex) {
    s21::ObjectBuilder builder;

    builder.
        AddVertice(s21::Vertex(0., 0., 0.)).
        AddVertice(s21::Vertex(0., 1., 0.)).
        AddVertice(s21::Vertex(0., 0., 1.)).
        AddVertice(s21::Vertex(1., 0., 0.));

    std::vector<int32_t> vertices_indices({0, 1, 2, 3});

    EXPECT_THROW({
        s21::RawFace rf;
        rf.vertices_indices = vertices_indices;
        builder.AddRawFace(rf);
    }, s21::Exception);
}

TEST(ObjectBuilderTest, FaceWithIndexOversize) {
    s21::ObjectBuilder builder;

    builder.
        AddVertice(s21::Vertex(0., 0., 0.)).
        AddVertice(s21::Vertex(0., 1., 0.)).
        AddVertice(s21::Vertex(0., 0., 1.)).
        AddVertice(s21::Vertex(1., 0., 0.));

    std::vector<int32_t> vertices_indices({1, 2, 3, 5});

    EXPECT_THROW({
        builder.AddRawFace(s21::RawFace(vertices_indices));
    }, s21::Exception);
}
