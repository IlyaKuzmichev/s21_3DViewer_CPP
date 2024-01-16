#include "parser.h"

#include <stdio.h>
#include <istream>
#include <limits>
#include <sstream>
#include <string>

#include <QDebug>
#include <QString>
#include <QElapsedTimer>

#include "model/exception.h"
#include "model/object.h"
#include "model/object_builder.h"

constexpr auto kStreamMaxSize = std::numeric_limits<std::streamsize>::max();
constexpr uint32_t kStringSize = 1024U;

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

s21::Object s21::ObjectParser::Parse(FILE* f) const {
  QElapsedTimer debug;
  debug.start();
  s21::ObjectBuilder builder;
  char* line = NULL;
  size_t linecap;
  std::string str;
  str.reserve(kStringSize);

  while (getline(&line, &linecap, f) > 0) {
      str = line;
      if (startsWith(str, "v ")) {
        ParseVertice(str, builder);
      } else if (startsWith(str, "f ")) {
        ParseFace(str, builder);
      }
    }

  qDebug() << "Parsing time: " << debug.elapsed()  << '\n';
  return builder.Build();
}

//s21::Object s21::ObjectParser::Parse(std::istream& input) const {
//  s21::ObjectBuilder builder;

//  std::vector<char> buffer(kBufferSize);
//  std::string leftover;

//  while (input.read(buffer.data(), kBufferSize)) {
//      size_t bytesRead = input.gcount();
//      std::string data(leftover + buffer.data(), leftover.size() + bytesRead);
//      leftover.clear();

//    size_t lastNewline = data.find_last_of('\n');
//    if (lastNewline != std::string::npos) {
//      leftover = data.substr(lastNewline + 1);
//      data = data.substr(0, lastNewline + 1);
//    }

//    ProcessStream(data, builder);
//  }
//  ProcessStream(leftover, builder);

//  return builder.Build();
//}

//void s21::ObjectParser::ProcessStream(const std::string& data, s21::ObjectBuilder& builder) const {
//    std::istringstream stream(data);
//    std::string line;

//    while (std::getline(stream, line)) {
//      if (startsWith(line, "v ")) {
//        ParseVertice(line, builder);
//      } else if (startsWith(line, "f ")) {
//        ParseFace(line, builder);
//      }
//    }
//}

void s21::ObjectParser::ParseVertice(std::string& line,
                                     s21::ObjectBuilder& builder) const {
  double x, y, z;
  std::istringstream stream(line);
  char skip;
  if (stream >> skip >> x >> y >> z) {
    builder.AddVertice(s21::Vertex(x, y, z));
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
    stream.ignore(kStreamMaxSize, ' ');
  } else {
    throw Exception("Empty face");
  }

  while (stream >> idx) {
    f.vertices_indices.push_back(idx);
    stream.ignore(kStreamMaxSize, ' ');
  }

  builder.AddRawFace(f);
}
