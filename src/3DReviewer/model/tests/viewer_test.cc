#include "model/viewer.h"

#include <gtest/gtest.h>

#include <cmath>

#include "model/exception.h"
#include "model/object.h"
#include "model/tests/utils.h"
#include "model/transformer.h"

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

  viewer.SetTranslation(s21::Viewer::Axis::kX, 2.);
  viewer.SetTranslation(s21::Viewer::Axis::kY, 2.);
  viewer.SetTranslation(s21::Viewer::Axis::kX, 2.);
  viewer.SetTranslation(s21::Viewer::Axis::kY, 2.);

  EXPECT_EQ(2, viewer.GetTranslation(s21::Viewer::Axis::kX));
  EXPECT_EQ(2, viewer.GetTranslation(s21::Viewer::Axis::kY));

  viewer.SetTranslation(s21::Viewer::Axis::kX, 0);
  viewer.SetTranslation(s21::Viewer::Axis::kY, 0);
  viewer.SetTranslation(s21::Viewer::Axis::kZ, 3.);
  viewer.SetTranslation(s21::Viewer::Axis::kZ, -2.);
  viewer.SetTranslation(s21::Viewer::Axis::kZ, -1.);

  EXPECT_EQ(-1, viewer.GetTranslation(s21::Viewer::Axis::kZ));

  viewer.SetTranslation(s21::Viewer::Axis::kZ, 0);

  EXPECT_EQ(expected, s21test::TestableObject(viewer.GetObject()));
}

TEST(ViewerTest, RotateCube) {
  s21::Viewer viewer;

  viewer.LoadObject("3DReviewer/model/tests/objects/cube.obj");

  auto obj = viewer.GetObject();

  s21test::TestableObject expected(obj);

  viewer.SetRotation(s21::Viewer::Axis::kX, M_PI / 2);
  viewer.SetRotation(s21::Viewer::Axis::kX, 3 * M_PI / 2);
  viewer.SetRotation(s21::Viewer::Axis::kX, 0);
  viewer.SetRotation(s21::Viewer::Axis::kY, -M_PI);
  viewer.SetRotation(s21::Viewer::Axis::kY, -M_PI);
  viewer.SetRotation(s21::Viewer::Axis::kY, 0);
  viewer.SetRotation(s21::Viewer::Axis::kZ, 3 * M_PI);
  viewer.SetRotation(s21::Viewer::Axis::kZ, -5 * M_PI);
  viewer.SetRotation(s21::Viewer::Axis::kZ, 0.);

  EXPECT_EQ(0, viewer.GetRotation(s21::Viewer::Axis::kX));
  EXPECT_EQ(0, viewer.GetRotation(s21::Viewer::Axis::kY));
  EXPECT_EQ(0, viewer.GetRotation(s21::Viewer::Axis::kZ));
  EXPECT_EQ(expected, s21test::TestableObject(viewer.GetObject()));
}

TEST(ViewerTest, ScaleCube) {
  s21::Viewer viewer;

  viewer.LoadObject("3DReviewer/model/tests/objects/cube.obj");

  auto obj = viewer.GetObject();

  s21test::TestableObject expected(obj);
  s21::Transformer transformer;
  transformer.ScaleObject(expected, 10);

  viewer.SetScale(2.);
  viewer.SetScale(0.5);
  viewer.SetScale(0.1);
  viewer.SetScale(10);
  viewer.SetScale(10);

  EXPECT_EQ(10, viewer.GetScale());
  EXPECT_EQ(expected, s21test::TestableObject(viewer.GetObject()));
}

TEST(ViewerTest, LoadUnexistingFile) {
  s21::Viewer viewer;

  EXPECT_THROW({ viewer.LoadObject("/path/dont/exist/dont/even/try"); },
               s21::Exception);
}
