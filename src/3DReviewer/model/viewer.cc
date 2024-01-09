#include <cmath>
#include <sstream>
#include <fstream>
#include <vector>

#include "exception.h"
#include "viewer.h"
#include "parser.h"

static int axisInt(s21::Viewer::Axis axis) noexcept {
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

//    stream.seekg(0, std::ios::end);
//    size_t file_size = stream.tellg();
//    stream.seekg(0, std::ios::beg);
//    std::vector<char> data(file_size);
//    stream.read(&data[0], file_size);

//    std::string string_stream(data.begin(), data.end());
//    std::istringstream file_data(string_stream);

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
    if (params_.rotation[axisInt(axis)] == angle) {
        return;
    }
    params_.rotation[axisInt(axis)] = angle;
    RecountCurrentState();
}

void s21::Viewer::SetTranslation(Axis axis, double shift) noexcept {
    if (params_.translation[axisInt(axis)] == shift) {
        return;
    }
    TranslateObject(axis, shift);
    params_.translation[axisInt(axis)] = shift;
}

void s21::Viewer::SetScale(double scale) noexcept {
    if (params_.scale == scale) {
        return;
    }
    params_.scale = scale;
    RecountCurrentState();
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
    transformer_.TranslateObject(currentState_, axis, shift - params_.translation[axisInt(axis)]);
}

void s21::Viewer::TranslateObject() {
    transformer_.TranslateObject(currentState_, Axis::kX, params_.translation[axisInt(Axis::kX)]);
    transformer_.TranslateObject(currentState_, Axis::kY, params_.translation[axisInt(Axis::kY)]);
    transformer_.TranslateObject(currentState_, Axis::kZ, params_.translation[axisInt(Axis::kZ)]);
}

void s21::Viewer::RotateOxObject() {
    double angle = params_.rotation[axisInt(Axis::kX)];
    transformer_.RotateObject(currentState_, Axis::kX, angle); 
}

void s21::Viewer::RotateOyObject() {
    double angle = params_.rotation[axisInt(Axis::kY)];
    transformer_.RotateObject(currentState_, Axis::kY, angle); 
}

void s21::Viewer::RotateOzObject() {
    double angle = params_.rotation[axisInt(Axis::kZ)];
    transformer_.RotateObject(currentState_, Axis::kZ, angle); 
}

void s21::Viewer::ScaleObject() {
    transformer_.ScaleObject(currentState_, params_.scale);
}

double s21::Viewer::GetTranslation(Axis axis) const noexcept {
    return params_.translation[axisInt(axis)];
}

double s21::Viewer::GetRotation(Axis axis) const noexcept {
    return params_.rotation[axisInt(axis)];
}

double s21::Viewer::GetScale() const noexcept {
    return params_.scale;
}
