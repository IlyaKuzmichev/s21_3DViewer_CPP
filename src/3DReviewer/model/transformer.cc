#include "model/transformer.h"
#include "model/object.h"

#include <cmath>

static int axisInt(s21::Transformer::Axis axis) noexcept {
    return static_cast<int>(axis);
}

void s21::Transformer::TranslateObject(s21::Object& obj, Axis axis, double shift) noexcept {
    for (auto& v : obj.vertices) {
        v.coords[axisInt(axis)] += shift;
    }
}

void s21::Transformer::RotateObject(s21::Object& obj, Axis axis, double angle) noexcept {
    switch (axis) {
    case Axis::kX:
        RotateOxObject(obj, angle);
        break;
    case Axis::kY:
        RotateOyObject(obj, angle);
        break;
    case Axis::kZ:
        RotateOzObject(obj, angle);
        break;
    }
}

void s21::Transformer::ScaleObject(s21::Object& obj, double scale) noexcept {
    for (auto& v : obj.vertices) {
        v.coords[axisInt(Axis::kX)] *= scale;
        v.coords[axisInt(Axis::kY)] *= scale;
        v.coords[axisInt(Axis::kZ)] *= scale;
    }
}

void s21::Transformer::RotateOxObject(s21::Object& obj, double angle) noexcept {
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : obj.vertices) {
        double y = v.coords[axisInt(Axis::kY)];
        double z = v.coords[axisInt(Axis::kZ)];
        v.coords[axisInt(Axis::kY)] = y * angle_cos - z * angle_sin;
        v.coords[axisInt(Axis::kZ)] = y * angle_sin + z * angle_cos;
    }
}

void s21::Transformer::RotateOyObject(s21::Object& obj, double angle) noexcept {
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : obj.vertices) {
        double x = v.coords[axisInt(Axis::kX)];
        double z = v.coords[axisInt(Axis::kZ)];
        v.coords[axisInt(Axis::kX)] = x * angle_cos + z * angle_sin;
        v.coords[axisInt(Axis::kZ)] = -x * angle_sin + z * angle_cos;

    }
}

void s21::Transformer::RotateOzObject(s21::Object& obj, double angle) noexcept {
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : obj.vertices) {
        double x = v.coords[axisInt(Axis::kX)];
        double y = v.coords[axisInt(Axis::kY)];
        v.coords[axisInt(Axis::kX)] = x * angle_cos - y * angle_sin;
        v.coords[axisInt(Axis::kY)] = x * angle_sin + y * angle_cos;
    }
}
