#ifndef SRC_MODEL_PARSER_H_
#define SRC_MODEL_PARSER_H_

#include <istream>
#include <sstream>
#include <string>

#include "model/object.h"
#include "model/object_builder.h"

namespace s21 {

class ObjectParser {
 public:
  ObjectParser() : vertices_preallocation_(0), faces_preallocation_(0) {}
  ObjectParser(size_t vertices_prealloc, size_t faces_prealloc)
      : vertices_preallocation_(vertices_prealloc),
        faces_preallocation_(faces_prealloc) {}

  Object* Parse(std::istream& input) const;

 private:
  void ParseVertice(std::string& line, s21::ObjectBuilder& builder) const;
  void ParseFace(std::string& line, s21::ObjectBuilder& builder) const;
  void ProcessStream(const std::string& data,
                     s21::ObjectBuilder& builder) const;

  size_t vertices_preallocation_;
  size_t faces_preallocation_;
};

}  // namespace s21

#endif  // SRC_MODEL_PARSER_H_
