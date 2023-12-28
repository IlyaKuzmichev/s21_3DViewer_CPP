#ifndef SRC_MODEL_PARSER_H
#define SRC_MODEL_PARSER_H

#include <cmath>
#include <string>
#include <vector>

namespace s21 {

struct Object {
  Object()
      : facesCount(0),
        xMin(INFINITY),
        xMax(-INFINITY),
        yMin(INFINITY),
        yMax(-INFINITY) {}

  std::vector<double> vertices;
  std::vector<int32_t> faces;
  uint32_t facesCount;
  double xMin, xMax, yMin, yMax;
};

class ObjectFileParser {
 public:
  explicit ObjectFileParser(const std::string& file) : file_(file) {}

  Object ParseFile() const;

 private:
  void AddVertice(Object& obj, std::string& str) const;
  void AddFace(Object& obj, std::string& str) const;

  std::string file_;
};

}  // namespace s21

#endif  // SRC_MODEL_PARSER_H