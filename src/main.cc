#include <exception>
#include <iostream>

#include <cmath>
#include "model/model.h"

int main() {
  s21::ModelHandler model;

  model.LoadObject(
      "/Users/wilmerno/github_wilmerno/s21_3DViewer_CPP/src/chair.txt");

  try {
    s21::Object obj = model.GetObject();
    std::cout << obj.vertices.size() / 3 << " - number of vertices, "
              << obj.faces.size() << " parced faces indexes" << std::endl;
    std::cout << obj.facesCount << " - number of faces!" << std::endl;

    std::cout << "Some vertices before rotation" << std::endl;
    std::cout << obj.vertices[1] << " " << obj.vertices[4] << " " << obj.vertices[16] << std::endl;
    model.SetRotation(s21::Axis::kAxisZ, 160. / 180. * M_PI);
    model.SetRotation(s21::Axis::kAxisX, 720. / 180. * M_PI);
    std::cout << "Some vertices after rotation" << std::endl;
    obj = model.GetObject();
    std::cout << obj.vertices[1] << " " << obj.vertices[4] << " " << obj.vertices[16] << std::endl;
    
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}