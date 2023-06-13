#pragma once
#include <iostream>
#include <vector>

#include "backend/for_society/for_society.hpp"
#include "backend/number/number.hpp"

namespace CM {

const uint32_t kRoundedBuffSize = 6;
const uint32_t kNumeratedBuffSize = 9;

enum Round { ClockWise, ContrClockWise };

class Buffer {
 public:
  Buffer() = default;
  Buffer(const Buffer&) = default;
  Buffer& operator=(const Buffer&) = default;

  const std::vector<CN::Number>& GetNumeratedBuffer() const noexcept;
  std::vector<CN::Number> GetRoundedBuffer() const noexcept;

  void RoundBuffer(Round) noexcept;
  void PutFromZToX(uint32_t);
  void PutFromXToZ(uint32_t);

  CN::Number& GetX0() noexcept;

  // for backup / restore
  Buffer(const std::vector<CN::Number>& numerated_buffer,
         const std::vector<CN::Number>& rounded_buffer);

 private:
  //const int kRealRoundBuffer = kRoundedBuffSize - 1;

  std::vector<CN::Number> buffer_body_ =
      std::vector<CN::Number>(kNumeratedBuffSize);
  std::vector<CN::Number> round_buffer_body_ =
      std::vector<CN::Number>(kRoundedBuffSize - 1);
  void RoundClockWise();
};

}  // namespace CM