#include <string>
#include <algorithm>

#include "utils.h"

namespace Utils {
  std::string ToLower(std::string str) {
    // for (char& c : str) // accessing str parameter address
    //   c = std::tolower(c);

    // recommended
    std::transform(
      std::begin(str),
      std::end(str),
      std::begin(str), 
      [](unsigned char c) {
        return std::tolower(c);
      }
    );

    return str;
  }

  bool HasWhitespaces(const std::string& str) {
    return str.find(' ') != std::string::npos;
  }
}