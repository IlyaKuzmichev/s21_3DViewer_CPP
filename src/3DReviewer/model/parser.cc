#include "parser.h"

#include <istream>
#include <limits>
#include <sstream>
#include <string>

#include "model/exception.h"
#include "model/object.h"
#include "model/object_builder.h"

constexpr auto kStreamMaxSize = std::numeric_limits<std::streamsize>::max();

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

s21::Object s21::ObjectParser::Parse(std::istream& input) const {
  s21::ObjectBuilder builder;
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

void s21::ObjectParser::ParseVertice(std::string& line,
                                     s21::ObjectBuilder& builder) const {
  double x, y, z;
  std::istringstream stream(line);
  char skip;
  if (stream >> skip >> x >> y >> z) {
    builder.AddVertice(s21::Vertice(x, y, z));
  } else {
    throw Exception("Not valid object file!");
  }
}

void s21::ObjectParser::ParseFace(std::string& line,
                                  s21::ObjectBuilder& builder) const {
  std::istringstream stream(line);

  stream.ignore(kStreamMaxSize, ' ');
  int32_t idx;
  s21::RawFace f;

  if (stream >> idx) {
    f.vertices_indices.push_back(idx);
  } else {
    throw Exception("Empty face");
  }

  while (stream >> idx) {
    f.vertices_indices.push_back(idx);
    stream.ignore(kStreamMaxSize, ' ');
  }

  builder.AddRawFace(f);
}