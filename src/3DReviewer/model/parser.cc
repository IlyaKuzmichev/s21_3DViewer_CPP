#include "model/parser.h"

#include <cmath>
#include <istream>
#include <limits>
#include <sstream>
#include <string>

#include "model/exception.h"
#include "model/object.h"
#include "model/object_builder.h"

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

void s21::ObjectParser::ParseVertice(std::string& line,
                                     s21::ObjectBuilder& builder) const {
  double x, y, z;

  int n = std::sscanf(line.c_str(), "v %lf %lf %lf", &x, &y, &z);
  if (n != 3) {
    throw Exception("Not valid object file!");
  }
  builder.AddVertice(Vertex(x, y, z));
}

static const int8_t kParseFaceStateOmitSymbols = 1;
static const int8_t kParseFaceStateOmitWhitespaces = 2;
static const int8_t kParseFaceStateReadNumber = 3;

void s21::ObjectParser::ParseFace(std::string& line,
                                  s21::ObjectBuilder& builder) const {
  const char* iter = line.c_str();
  ++iter;

  s21::RawFace f;
  char* end = nullptr;
  double num;
  int8_t parse_state = kParseFaceStateOmitWhitespaces;

  while (*iter != '\0' && *iter != '#') {
    switch (parse_state) {
      case kParseFaceStateOmitWhitespaces:
        if (!std::isspace(*iter)) {
          parse_state = kParseFaceStateReadNumber;
        } else {
          ++iter;
        }
        break;
      case kParseFaceStateReadNumber:
        num = std::strtod(iter, &end);
        if (iter == end) {
          throw Exception("Invalid face");
        }
        f.vertices_indices.push_back(num);
        iter = end;
        parse_state = kParseFaceStateOmitSymbols;
        break;
      case kParseFaceStateOmitSymbols:
        if (std::isspace(*iter)) {
          parse_state = kParseFaceStateOmitWhitespaces;
        } else {
          ++iter;
        }
        break;
    }
  }

  if (f.vertices_indices.size() == 0) {
    throw Exception("Empty face");
  }

  builder.AddRawFace(f);
}
