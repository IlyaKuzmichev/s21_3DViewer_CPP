#include "model/object.h"
#include "model/transformer.h"
#include "model/tests/utils.h"

#include <array>
#include <cmath>
#include <vector>

#include <gtest/gtest.h>

TEST(AffineTransformationsTest, MoveX) {
    double shift = 5.;

    std::array<s21::Vertex, 8> expected_vertices({
                                  s21::Vertice(1 + shift, 1., 0.),
                                  s21::Vertice(-1 + shift, 1., 0.),
                                  s21::Vertice(-1 + shift, -1., 0.),
                                  s21::Vertice(1 + shift, -1., 0.),
                                  s21::Vertice(1 + shift, 1., 2.),
                                  s21::Vertice(-1 + shift, 1., 2.),
                                  s21::Vertice(-1 + shift, -1., 2.),
                                  s21::Vertice(1 + shift, -1., 2.),
                                });
    s21test::TestableObject expected(s21test::ConstructTestCube(expected_vertices));
    s21test::TestableObject actual(s21test::ConstructBasicTestCube());
    
    s21::Transformer transformer;

    transformer.TranslateObject(actual, s21::Transformer::Axis::kX, shift);

    EXPECT_EQ(expected, actual);
}

TEST(AffineTransformationsTest, MoveY) {
    double shift = -3.;

    std::array<s21::Vertex, 8> expected_vertices({
                                  s21::Vertice(1, 1. + shift, 0.),
                                  s21::Vertice(-1, 1. + shift, 0.),
                                  s21::Vertice(-1, -1. + shift, 0.),
                                  s21::Vertice(1, -1. + shift, 0.),
                                  s21::Vertice(1, 1. + shift, 2.),
                                  s21::Vertice(-1, 1. + shift, 2.),
                                  s21::Vertice(-1, -1. + shift, 2.),
                                  s21::Vertice(1, -1. + shift, 2.),
                                });
    s21test::TestableObject expected(s21test::ConstructTestCube(expected_vertices));
    s21test::TestableObject actual(s21test::ConstructBasicTestCube());
    
    s21::Transformer transformer;

    transformer.TranslateObject(actual, s21::Transformer::Axis::kY, shift);

    EXPECT_EQ(expected, actual);
}

TEST(AffineTransformationsTest, MoveZ) {
    double shift = 16.;

    std::array<s21::Vertex, 8> expected_vertices({
                                  s21::Vertice(1, 1., 0. + shift),
                                  s21::Vertice(-1, 1., 0. + shift),
                                  s21::Vertice(-1, -1., 0. + shift),
                                  s21::Vertice(1, -1., 0. + shift),
                                  s21::Vertice(1, 1., 2. + shift),
                                  s21::Vertice(-1, 1., 2. + shift),
                                  s21::Vertice(-1, -1., 2. + shift),
                                  s21::Vertice(1, -1., 2. + shift),
                                });
    s21test::TestableObject expected(s21test::ConstructTestCube(expected_vertices));
    s21test::TestableObject actual(s21test::ConstructBasicTestCube());
    
    s21::Transformer transformer;

    transformer.TranslateObject(actual, s21::Transformer::Axis::kZ, shift);

    EXPECT_EQ(expected, actual);
}

TEST(AffineTransformationsTest, RotateX) {
    double angle = M_PI_2;

    std::array<s21::Vertex, 8> expected_vertices({
                                  s21::Vertice(1, 0., -1.),
                                  s21::Vertice(-1, 0., -1.),
                                  s21::Vertice(-1, 0., 1.),
                                  s21::Vertice(1, 0., 1.),
                                  s21::Vertice(1, 2., -1.),
                                  s21::Vertice(-1, 2., -1.),
                                  s21::Vertice(-1, 2., 1.),
                                  s21::Vertice(1, 2., 1.),
                                });
    s21test::TestableObject expected(s21test::ConstructTestCube(expected_vertices));
    s21test::TestableObject actual(s21test::ConstructBasicTestCube());
    
    s21::Transformer transformer;

    transformer.RotateObject(actual, s21::Transformer::Axis::kX, angle);

    EXPECT_EQ(expected, actual);
}

TEST(AffineTransformationsTest, RotateY) {
    double angle = M_PI;

    std::array<s21::Vertex, 8> expected_vertices({
                                  s21::Vertice(-1, 1., 0.),
                                  s21::Vertice(1, 1., 0.),
                                  s21::Vertice(1, -1., 0.),
                                  s21::Vertice(-1, -1., 0.),
                                  s21::Vertice(-1, 1., -2.),
                                  s21::Vertice(1, 1., -2.),
                                  s21::Vertice(1, -1., -2.),
                                  s21::Vertice(-1, -1., -2.),
                                });
    s21test::TestableObject expected(s21test::ConstructTestCube(expected_vertices));
    s21test::TestableObject actual(s21test::ConstructBasicTestCube());
    
    s21::Transformer transformer;

    transformer.RotateObject(actual, s21::Transformer::Axis::kY, angle);

    EXPECT_EQ(expected, actual);
}

TEST(AffineTransformationsTest, RotateZ) {
    double angle = M_PI_4;

    std::array<s21::Vertex, 8> expected_vertices({
                                  s21::Vertice(M_SQRT2, 0., 0.),
                                  s21::Vertice(0, M_SQRT2, 0.),
                                  s21::Vertice(-M_SQRT2, 0., 0.),
                                  s21::Vertice(0., -M_SQRT2, 0.),
                                  s21::Vertice(M_SQRT2, 0., 2.),
                                  s21::Vertice(0, M_SQRT2, 2.),
                                  s21::Vertice(-M_SQRT2, 0., 2.),
                                  s21::Vertice(0., -M_SQRT2, 2.),
                                });
    s21test::TestableObject expected(s21test::ConstructTestCube(expected_vertices));
    s21test::TestableObject actual(s21test::ConstructBasicTestCube());
    
    s21::Transformer transformer;

    transformer.RotateObject(actual, s21::Transformer::Axis::kZ, angle);

    EXPECT_EQ(expected, actual);
}

TEST(AffineTransformationsTest, Scale) {
    double scale = 10000.;

    std::array<s21::Vertex, 8> expected_vertices({
                                  s21::Vertice(scale, scale, 0.),
                                  s21::Vertice(-scale, scale, 0.),
                                  s21::Vertice(-scale, -scale, 0.),
                                  s21::Vertice(scale, -scale, 0.),
                                  s21::Vertice(scale, scale, 2 * scale),
                                  s21::Vertice(-scale, scale, 2 * scale),
                                  s21::Vertice(-scale, -scale, 2 * scale),
                                  s21::Vertice(scale, -scale, 2 * scale),
                                });
    s21test::TestableObject expected(s21test::ConstructTestCube(expected_vertices));
    s21test::TestableObject actual(s21test::ConstructBasicTestCube());
    
    s21::Transformer transformer;

    transformer.ScaleObject(actual, scale);

    EXPECT_EQ(expected, actual);
}
