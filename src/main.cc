#include <exception>
#include <iostream>

#include <cmath>

#include "model/viewer.h"

int main() {
    s21::Viewer viewer;

    try {
        viewer.LoadObject("/Users/wilmerno/github_wilmerno/s21_3DViewer_CPP/src/chair.txt");

        s21::Object obj = viewer.GetObject();
        std::cout << obj.vertices.size() << " - number of vertices, "
            << obj.faces.size() << " - number of faces!" << std::endl;

        std::cout << "Some vertices before rotation" << std::endl;
        std::cout << "One: x:" << obj.vertices[0].coords[static_cast<int>(s21::Axis::kX)] <<
                          "y:" << obj.vertices[0].coords[static_cast<int>(s21::Axis::kY)] <<
                          "z:" << obj.vertices[0].coords[static_cast<int>(s21::Axis::kZ)] <<
                          std::endl;

        std::cout << "Two: x:" << obj.vertices[1].coords[static_cast<int>(s21::Axis::kX)] <<
                          "y:" << obj.vertices[1].coords[static_cast<int>(s21::Axis::kY)] <<
                          "z:" << obj.vertices[1].coords[static_cast<int>(s21::Axis::kZ)] <<
                          std::endl;

        std::cout << "Three: x:" << obj.vertices[2].coords[static_cast<int>(s21::Axis::kX)] <<
                          "y:" << obj.vertices[2].coords[static_cast<int>(s21::Axis::kY)] <<
                          "z:" << obj.vertices[2].coords[static_cast<int>(s21::Axis::kZ)] <<
                          std::endl;    

        viewer.SetRotation(s21::Axis::kZ, 360. / 180. * M_PI);
        // viewer.SetRotation(s21::Axis::kX, 160. / 180. * M_PI);

        obj = viewer.GetObject();
        std::cout << "Some vertices after rotation" << std::endl;
         std::cout << "One: x:" << obj.vertices[0].coords[static_cast<int>(s21::Axis::kX)] <<
                          "y:" << obj.vertices[0].coords[static_cast<int>(s21::Axis::kY)] <<
                          "z:" << obj.vertices[0].coords[static_cast<int>(s21::Axis::kZ)] <<
                          std::endl;

        std::cout << "Two: x:" << obj.vertices[1].coords[static_cast<int>(s21::Axis::kX)] <<
                          "y:" << obj.vertices[1].coords[static_cast<int>(s21::Axis::kY)] <<
                          "z:" << obj.vertices[1].coords[static_cast<int>(s21::Axis::kZ)] <<
                          std::endl;

        std::cout << "Three: x:" << obj.vertices[2].coords[static_cast<int>(s21::Axis::kX)] <<
                          "y:" << obj.vertices[2].coords[static_cast<int>(s21::Axis::kY)] <<
                          "z:" << obj.vertices[2].coords[static_cast<int>(s21::Axis::kZ)] <<
                          std::endl;      

    } catch (std::exception& e) {
        std::cout << "got exception: " << e.what() << std::endl;
    }
}