#pragma

#include <exception>
#include <string>

class AddressException: public std::exception {
 public:
  AddressException(const std::string& msg): msg_(msg) {}
  virtual const char* what() throw() { return msg_.c_str(); }
 private:
  std::string msg_;
};
