#include "model/parser.h"

#include <iostream>
#include <fstream>
#include <sstream>

constexpr bool startsWith(std::string& str, const std::string& prefix) {
  if (prefix.size() > str.size()) {
    return false;
  }
  size_t counter = 0;
  for (const auto& c : prefix) {
    if (str[counter] != c) {
      return false;
    }
    ++counter;
  }

  return true;
}

s21::Object s21::ObjectFileParser::ParseFile() {
  std::ifstream input(file_);

  if (input.is_open() == false) {
    throw("Error occured with file opening!");
  }

  s21::Object obj;
  std::string str;

  while (std::getline(input, str)) {
    if (startsWith(str, "v ")) {
      AddVertice(obj, str);
    } else if (startsWith(str, "f ")) {
      AddFace(obj, str);
    }
  }

  return obj;
}

void s21::ObjectFileParser::AddVertice(Object& obj, std::string& str) {
  double x;
  std::istringstream stream(str);
  

  obj.vertices.push_back(x);
}

void s21::ObjectFileParser::AddFace(Object& obj, std::string& str) {

}

