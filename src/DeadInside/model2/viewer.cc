#include <cmath>
#include <fstream>

#include "viewer.h"
#include "parser.h"

void s21::Viewer::LoadObject(const std::string& filepath) {
    if (filepath == current_file_) {
        return;
    }
    current_file_ = filepath;
    ObjectParser parser;

    std::ifstream stream(filepath);

    Object obj = parser.Parse(stream);
    std::swap(obj, baseState_);
    params_.Init();
    currentState_ = baseState_;
}

const s21::Object& s21::Viewer::GetObject() const noexcept {
    return currentState_;
}

void s21::Viewer::SetRotation(Axis axis, double angle) noexcept {
    params_.rotation[axis] = angle;
    RecountCurrentState();
}

void s21::Viewer::SetTranslation(Axis axis, double shift) noexcept {
    TranslateObject(axis, shift - params_.translation[axis]);
    params_.translation[axis] = shift;
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
        v.coords[axis] += shift;
    }
}

void s21::Viewer::TranslateObject() {
    for (auto& v : currentState_.vertices) {
        v.coords[kAxisX] += params_.translation[kAxisX];
        v.coords[kAxisY] += params_.translation[kAxisY];
        v.coords[kAxisZ] += params_.translation[kAxisZ];
    }
}

void s21::Viewer::RotateOxObject() {
    double angle = params_.rotation[kAxisX];
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : currentState_.vertices) {
        double y = v.coords[kAxisY];
        double z = v.coords[kAxisZ];
        v.coords[kAxisY] = y * angle_cos - z * angle_sin;
        v.coords[kAxisZ] = y * angle_sin + z * angle_cos;
    }
}

void s21::Viewer::RotateOyObject() {
    double angle = params_.rotation[kAxisY];
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : currentState_.vertices) {
        double x = v.coords[kAxisX];
        double z = v.coords[kAxisZ];
        v.coords[kAxisX] = x * angle_cos + z * angle_sin;
        v.coords[kAxisZ] = -x * angle_sin + z * angle_cos;
    }
}

void s21::Viewer::RotateOzObject() {
    double angle = params_.rotation[kAxisZ];
    double angle_cos = std::cos(angle);
    double angle_sin = std::sin(angle);
    for (auto& v : currentState_.vertices) {
        double x = v.coords[kAxisX];
        double y = v.coords[kAxisY];
        v.coords[kAxisX] = x * angle_cos - y * angle_sin;
        v.coords[kAxisY] = x * angle_sin + y * angle_cos;
    }
}

void s21::Viewer::ScaleObject() {
    for (auto& v : currentState_.vertices) {
        v.coords[kAxisX] *= params_.scale;
        v.coords[kAxisY] *= params_.scale;
        v.coords[kAxisZ] *= params_.scale;
    }
}