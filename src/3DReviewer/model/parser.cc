#include "parser.h"

#include <cmath>
#include <istream>
#include <limits>
#include <sstream>
#include <string>

#include "model/exception.h"
#include "model/object.h"
#include "model/object_builder.h"

//constexpr auto kStreamMaxSize = std::numeric_limits<std::streamsize>::max();

bool startsWith(const std::string& str, const std::string& prefix) {
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

s21::Object* s21::ObjectParser::Parse(std::istream& input) const {


//   input.clear();
//   input.seekg(0);

  s21::ObjectBuilder builder(vertices_preallocation_, faces_preallocation_);
  std::string line;
  while (std::getline(input, line)) {
    if (startsWith(line, "v ")) {
      ParseVertice(line, builder);
    } else if (startsWith(line, "f ")) {
      ParseFace(line, builder);
    }
  }
  return builder.Build();
}

std::pair<size_t, size_t> s21::ObjectParser::ParseObjectSizes(std::istream& input) const {
  std::string line;
  std::pair<size_t, size_t> result(0, 0);
  while (std::getline(input, line)) {
    if (startsWith(line, "v ")) {
      ++result.first;
    } else if (startsWith(line, "f ")) {
      ++result.second;
    }
  }
  return result;
}

void s21::ObjectParser::ParseVertice(std::string& line,
                                     s21::ObjectBuilder& builder) const {
  double x, y, z;

  int n = std::sscanf(line.c_str(), "v %lf %lf %lf", &x, &y, &z);
  if (n != 3) {
    throw Exception("Not valid object file!");
  } 
  builder.AddVertice(Vertex(x, y, z));
}

void s21::ObjectParser::ParseFace(std::string& line,
                                  s21::ObjectBuilder& builder) const {
  
  
  const char* iter = line.c_str();
  ++iter;

  while (*iter != '\0' && std::isspace(*iter)) {
    ++iter;
  }

  if (*iter == '\0') {
    throw Exception("Empty face");
  }

  s21::RawFace f;

  char* end = nullptr;
  auto num = std::strtod(iter, &end);
  if (iter == end) {
    throw Exception("Empty face");
  }
  f.vertices_indices.push_back(num);
  iter = end;

  while (*iter != '\0') {
    while (*iter != '\0' && !std::isspace(*iter)) {
      ++iter;
    }

    if (*iter == '\0') {
      break;
    }

    while (*iter != '\0' && std::isspace(*iter)) {
      ++iter;
    }

    if (*iter == '\0') {
      break;
    }

    auto num = std::strtod(iter, &end);
    if (iter == end) {
      throw Exception("Empty face");
    }
    f.vertices_indices.push_back(num);
    iter = end;
  }

  builder.AddRawFace(f);
  
  // std::istringstream stream(line);

  // stream.ignore(kStreamMaxSize, ' ');
  // int32_t idx;
  // s21::RawFace f;

  // if (stream >> idx) {
  //   f.vertices_indices.push_back(idx);
  //   stream.ignore(kStreamMaxSize, ' ');
  // } else {
  //   throw Exception("Empty face");
  // }

  // while (stream >> idx) {
  //   f.vertices_indices.push_back(idx);
  //   stream.ignore(kStreamMaxSize, ' ');
  // }

  // builder.AddRawFace(f);
}
