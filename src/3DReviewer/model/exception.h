#ifndef SRC_MODEL_EXCEPTION_H_
#define SRC_MODEL_EXCEPTION_H_

#include <exception>
#include <string>

namespace s21 {

class Exception : public std::exception {
 public:
  explicit Exception(const char* message)
      : msg_(kMsgPrefix + std::string(message)) {}

  explicit Exception(const std::string message) : msg_(kMsgPrefix + message) {}

  const char* what() const noexcept override { return msg_.c_str(); }

 private:
  std::string msg_;

  inline const static std::string kMsgPrefix = "Viewer error: ";
};

}  // namespace s21

#endif  // SRC_MODEL_EXCEPTION_H_