#include "model/parser.h"

#include <fstream>
#include <limits>
#include <sstream>

constexpr auto maxSize = std::numeric_limits<std::streamsize>::max();

bool startsWith(std::string& str, const std::string& prefix) {
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

s21::Object s21::ObjectFileParser::ParseFile() const {
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

void s21::ObjectFileParser::AddVertice(Object& obj, std::string& str) const {
  double x, y, z;
  std::istringstream stream(str);
  char skip;
  if (stream >> skip >> x >> y >> z) {
    obj.vertices.push_back(x);
    obj.vertices.push_back(y);
    obj.vertices.push_back(z);
  } else {
    throw("Not valid object file!");
  }

  obj.xMax = x > obj.xMax ? x : obj.xMax;
  obj.xMin = x < obj.xMin ? x : obj.xMin;
  obj.yMax = y > obj.yMax ? y : obj.yMax;
  obj.yMin = y < obj.yMin ? y : obj.yMin;
}

void s21::ObjectFileParser::AddFace(Object& obj, std::string& str) const {
  int32_t index, firstIndex;
  std::istringstream stream(str);
  int32_t pointsCount = static_cast<int32_t>(obj.vertices.size() / 3);

  stream.ignore(maxSize, ' ');
  stream >> firstIndex;
  firstIndex = firstIndex > 0 ? firstIndex - 1 : pointsCount + firstIndex;
  obj.faces.push_back(firstIndex);
  stream.ignore(maxSize, ' ');

  while (stream >> index) {
    index = index > 0 ? index - 1 : pointsCount + index;
    obj.faces.push_back(index);
    obj.faces.push_back(index);
    stream.ignore(maxSize, ' ');
  }
  obj.faces.push_back(firstIndex);
  ++obj.facesCount;
}
