#ifndef SRC_MODEL_PARSER_H
#define SRC_MODEL_PARSER_H

#include <vector>

namespace s21 {

struct Object {
  std::vector<int32_t> vertices;
  std::vector<int32_t> faces;
};

class ObjectFileParser {
 public:
  explicit ObjectFileParser(const std::string& file) : file_(file) {}

  Object ParseFile();

 private:
  void AddVertice(Object& obj, std::string& str);
  void AddFace(Object& obj, std::string& str);

  std::string file_;
};

}  // namespace s21

#endif  // SRC_MODEL_PARSER_H