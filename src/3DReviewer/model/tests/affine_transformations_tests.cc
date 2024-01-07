#include "model/object.h"
#include "model/transformer.h"
#include "model/tests/utils.h"

#include <array>
#include <vector>

#include <gtest/gtest.h>

TEST(AffineTransformationsTest, MoveX) {
    double shift = 5.;

    std::array<s21::Vertice, 8> expected_vertices({
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
