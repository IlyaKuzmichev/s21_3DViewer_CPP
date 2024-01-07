#include <cmath>
#include <fstream>

#include "exception.h"
#include "viewer.h"
#include "parser.h"

int axisInt(s21::Viewer::Axis axis) noexcept {
    return static_cast<int>(axis);
}

void s21::Viewer::LoadObject(const std::string& filepath) {
    if (filepath == current_file_) {
        return;
    }

    std::ifstream stream(filepath);
    if (!stream.is_open()) {
        throw Exception("Cringe");
    }

    ObjectParser parser;
    current_file_ = filepath;
    Object obj = parser.Parse(stream);
    std::swap(obj, baseState_);
    params_.Init();
    currentState_ = baseState_;
}

const s21::Object& s21::Viewer::GetObject() const noexcept {
    return currentState_;
}

void s21::Viewer::SetRotation(Axis axis, double angle) noexcept {
    params_.rotation[axisInt(axis)] = angle;
    RecountCurrentState();
}

void s21::Viewer::SetTranslation(Axis axis, double shift) noexcept {
    TranslateObject(axis, shift - params_.translation[axisInt(axis)]);
    params_.translation[axisInt(axis)] = shift;
}

void s21::Viewer::SetScale(double scale) noexcept {
    params_.scale = scale;
    ScaleObject();
}

void s21::Viewer::RecountCurrentState() {
    currentState_ = baseState_;
    RotateOxObject();
    RotateOyObject();
    RotateOzObject();
    ScaleObject();
    TranslateObject();
}

void s21::Viewer::TranslateObject(Axis axis, double shift) {
    for (auto& v : currentState_.vertices) {
        v.coords[axisInt(axis)] += shift;
    }
}

void s21::Viewer::TranslateObject() {
    for (auto& v : currentState_.vertices) {
        v.coords[axisInt(Axis::kX)] += params_.translation[axisInt(Axis::kX)];
        v.coords[axisInt(Axis::kY)] += params_.translation[axisInt(Axis::kY)];
        v.coords[axisInt(Axis::kZ)] += params_.translation[axisInt(Axis::kZ)];
    }
}

void s21::Viewer::RotateOxObject() {
    double angle = params_.rotation[axisInt(Axis::kX)];
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : currentState_.vertices) {
        double y = v.coords[axisInt(Axis::kY)];
        double z = v.coords[axisInt(Axis::kZ)];
        v.coords[axisInt(Axis::kY)] = y * angle_cos - z * angle_sin;
        v.coords[axisInt(Axis::kZ)] = y * angle_sin + z * angle_cos;
    }
}

void s21::Viewer::RotateOyObject() {
    double angle = params_.rotation[axisInt(Axis::kY)];
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : currentState_.vertices) {
        double x = v.coords[axisInt(Axis::kX)];
        double z = v.coords[axisInt(Axis::kZ)];
        v.coords[axisInt(Axis::kX)] = x * angle_cos + z * angle_sin;
        v.coords[axisInt(Axis::kZ)] = -x * angle_sin + z * angle_cos;
    }
}

void s21::Viewer::RotateOzObject() {
    double angle = params_.rotation[axisInt(Axis::kZ)];
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : currentState_.vertices) {
        double x = v.coords[axisInt(Axis::kX)];
        double y = v.coords[axisInt(Axis::kY)];
        v.coords[axisInt(Axis::kX)] = x * angle_cos - y * angle_sin;
        v.coords[axisInt(Axis::kY)] = x * angle_sin + y * angle_cos;
    }
}

void s21::Viewer::ScaleObject() {
    for (auto& v : currentState_.vertices) {
        v.coords[axisInt(Axis::kX)] *= params_.scale;
        v.coords[axisInt(Axis::kY)] *= params_.scale;
        v.coords[axisInt(Axis::kZ)] *= params_.scale;
    }
}
