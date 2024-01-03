#include <exception>
#include <iostream>

#include <cmath>

#include "viewer.h"

int main() {
    s21::Viewer viewer;

    viewer.LoadObject("/Users/tabathae/s21_3DViewer_CPP/src/chair.txt");

    try {
        s21::Object obj = viewer.GetObject();
        std::cout << obj.vertices.size() << " - number of vertices, "
            << obj.faces.size() << " - number of faces!" << std::endl;

        std::cout << "Some vertices before rotation" << std::endl;
        std::cout << "One: x:" << obj.vertices[0].coords[s21::kAxisX] <<
                          "y:" << obj.vertices[0].coords[s21::kAxisY] <<
                          "z:" << obj.vertices[0].coords[s21::kAxisZ] <<
                          std::endl;

        std::cout << "Two: x:" << obj.vertices[1].coords[s21::kAxisX] <<
                          "y:" << obj.vertices[1].coords[s21::kAxisY] <<
                          "z:" << obj.vertices[1].coords[s21::kAxisZ] <<
                          std::endl;

        std::cout << "Three: x:" << obj.vertices[2].coords[s21::kAxisX] <<
                          "y:" << obj.vertices[2].coords[s21::kAxisY] <<
                          "z:" << obj.vertices[2].coords[s21::kAxisZ] <<
                          std::endl;    

        viewer.SetRotation(s21::kAxisZ, 160. / 180. * M_PI);
        viewer.SetRotation(s21::kAxisX, 160. / 180. * M_PI);

        obj = viewer.GetObject();
        std::cout << "Some vertices after rotation" << std::endl;
        std::cout << "One: x:" << obj.vertices[0].coords[s21::kAxisX] <<
                          "y:" << obj.vertices[0].coords[s21::kAxisY] <<
                          "z:" << obj.vertices[0].coords[s21::kAxisZ] <<
                          std::endl;

        std::cout << "Two: x:" << obj.vertices[1].coords[s21::kAxisX] <<
                          "y:" << obj.vertices[1].coords[s21::kAxisY] <<
                          "z:" << obj.vertices[1].coords[s21::kAxisZ] <<
                          std::endl;

        std::cout << "Three: x:" << obj.vertices[2].coords[s21::kAxisX] <<
                          "y:" << obj.vertices[2].coords[s21::kAxisY] <<
                          "z:" << obj.vertices[2].coords[s21::kAxisZ] <<
                          std::endl;   

    } catch (std::exception& e) {
        std::cout << "got exception: " << e.what() << std::endl;
    }
}