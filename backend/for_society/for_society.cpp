#include "for_society.hpp"

namespace FS {

uint32_t GetDigit(uint32_t number, uint32_t index) noexcept {
  for (uint32_t i = 0; i < index; ++i) {
    number /= 10;
  }
  return number % 10;
}

}

bool CP::IsOperationCode(int code) noexcept {
  if (code == CP::OpTrash) {
    return true;
  }
  if (code == 73 || code == 75) {
    return false;
  }
  if (code <= 0 || code >= 97 || FS::GetDigit(code, 0) == 0) {
    return false;
  }
  if (code > 30 && code < 80) {
    return true;
  }

  return FS::GetDigit(code, 0) <= 6;
}