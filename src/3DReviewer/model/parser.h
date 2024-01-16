#ifndef SRC_MODEL_PARSER_H_
#define SRC_MODEL_PARSER_H_

#include <istream>
#include <sstream>

#include "object.h"
#include "object_builder.h"

namespace s21 {

class ObjectParser {
    public:
        Object Parse(FILE* f) const;

    private:
        void ParseVertice(std::string& line, s21::ObjectBuilder& builder) const;
        void ParseFace(std::string& line, s21::ObjectBuilder& builder) const;
        void ProcessStream(const std::string& data, s21::ObjectBuilder& builder) const;
};

}

#endif  // SRC_MODEL_PARSER_H_
