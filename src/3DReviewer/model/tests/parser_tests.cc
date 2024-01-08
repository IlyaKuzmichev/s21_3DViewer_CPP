#include "model/exception.h"
#include "model/object.h"
#include "model/object_builder.h"
#include "model/parser.h"
#include "model/tests/utils.h"

#include <array>
#include <vector>
#include <sstream>

#include <gtest/gtest.h>

TEST(ParserTest, Empty) {
    std::istringstream stream("");
    s21::ObjectParser parser;

    s21test::TestableObject obj(parser.Parse(stream));

    s21test::TestableObject expected;

    EXPECT_EQ(expected, obj);
}

TEST(ParserTest, SingleVertice) {
    std::istringstream stream("v 5.3 1.2 3.4");
    s21::ObjectParser parser;

    s21test::TestableObject obj(parser.Parse(stream));

    s21::ObjectBuilder builder;
    builder.AddVertice(s21::Vertex(5.3, 1.2, 3.4));

    s21test::TestableObject expected(builder.Build());

    EXPECT_EQ(expected, obj);
}

TEST(ParserTest, MultipleVertices) {
    std::istringstream stream(
        "# test vertices\n"
        "v 23.2 42.4 54.3\n"
        "v 0.2312 0.45 -15.3\n"
        "v 53.2 23.4 66.7\n"
        "v 15.23 0.42 -124.45\n"
        "v 23.44 54.21 0.32\n"
        "\n"
        "#and more\n"
        "v 0.25 0.13 0.1\n"
        "v -1 -2 -3\n"
        "v -3 -4 -5\n"
    );
    s21::ObjectParser parser;

    s21test::TestableObject obj(parser.Parse(stream));

    s21::ObjectBuilder builder;
    builder.AddVertice(s21::Vertex(23.2, 42.4, 54.3));
    builder.AddVertice(s21::Vertex(0.2312, 0.45, -15.3));
    builder.AddVertice(s21::Vertex(53.2, 23.4, 66.7));
    builder.AddVertice(s21::Vertex(15.23, 0.42, -124.45));
    builder.AddVertice(s21::Vertex(23.44, 54.21, 0.32));
    builder.AddVertice(s21::Vertex(0.25, 0.13, 0.1));
    builder.AddVertice(s21::Vertex(-1, -2, -3));
    builder.AddVertice(s21::Vertex(-3, -4, -5));

    s21test::TestableObject expected(builder.Build());

    EXPECT_EQ(expected, obj);
}

TEST(ParserTest, BasicCube) {
    std::istringstream stream(
        "v 1. 1. 0.\n"
        "v -1. 1. 0.\n"
        "v -1. -1. 0.\n"
        "v 1. -1. 0.\n"
        "v 1. 1. 2.\n"
        "v -1. 1. 2.\n"
        "v -1. -1. 2.\n"
        "v 1. -1. 2.\n"
        "f 1 2 3 4\n"
        "f 1 2 5 6\n"
        "f 2/2 3 6 7\n"
        "f 3 4 7 8 #good face\n"
        "f 4 1 8 5/\n"
        "f 5 6 7 8\n"
    );
    s21::ObjectParser parser;

    s21test::TestableObject obj(parser.Parse(stream));

    s21test::TestableObject expected(s21test::ConstructBasicTestCube());

    EXPECT_EQ(expected, obj);
}
