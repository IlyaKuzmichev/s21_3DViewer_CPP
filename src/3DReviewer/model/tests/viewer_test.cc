#include "model/exception.h"
#include "model/object.h"
#include "model/viewer.h"
#include "model/transformer.h"
#include "model/tests/utils.h"

#include <cmath>

#include <gtest/gtest.h>

TEST(ViewerTest, LoadCube) {
    s21::Viewer viewer;

    viewer.LoadObject("3DReviewer/model/tests/objects/cube.obj");

    auto obj = viewer.GetObject();
    EXPECT_EQ(obj.vertices.size(), 8);
    EXPECT_EQ(obj.faces.size(), 12);
}

TEST(ViewerTest, TranslateCube) {
    s21::Viewer viewer;

    viewer.LoadObject("3DReviewer/model/tests/objects/cube.obj");

    auto obj = viewer.GetObject();

    s21test::TestableObject expected(obj);
    
    viewer.Translate(s21::Viewer::Axis::kX, 2.);
    viewer.Translate(s21::Viewer::Axis::kY, 2.);
    viewer.Translate(s21::Viewer::Axis::kX, -2.);
    viewer.Translate(s21::Viewer::Axis::kY, -2.);
    viewer.Translate(s21::Viewer::Axis::kZ, 3.);
    viewer.Translate(s21::Viewer::Axis::kZ, -2.);
    viewer.Translate(s21::Viewer::Axis::kZ, -1.);

    EXPECT_EQ(expected, s21test::TestableObject(viewer.GetObject()));
}

TEST(ViewerTest, RotateCube) {
    s21::Viewer viewer;

    viewer.LoadObject("3DReviewer/model/tests/objects/cube.obj");

    auto obj = viewer.GetObject();

    s21test::TestableObject expected(obj);
    
    viewer.Rotate(s21::Viewer::Axis::kX, M_PI / 2);
    viewer.Rotate(s21::Viewer::Axis::kX, 3 * M_PI / 2);
    viewer.Rotate(s21::Viewer::Axis::kY, -M_PI);
    viewer.Rotate(s21::Viewer::Axis::kY, -M_PI);
    viewer.Rotate(s21::Viewer::Axis::kZ, 3 * M_PI);
    viewer.Rotate(s21::Viewer::Axis::kZ, -5 * M_PI);
    
    EXPECT_EQ(expected, s21test::TestableObject(viewer.GetObject()));
}

TEST(ViewerTest, ScaleCube) {
    s21::Viewer viewer;

    viewer.LoadObject("3DReviewer/model/tests/objects/cube.obj");

    auto obj = viewer.GetObject();

    s21test::TestableObject expected(obj);
    s21::Transformer transformer;
    transformer.ScaleObject(expected, 10);
    
    viewer.Scale(2.);
    viewer.Scale(0.5);
    viewer.Scale(0.1);
    viewer.Scale(10);
        
    EXPECT_EQ(expected, s21test::TestableObject(viewer.GetObject()));
}
