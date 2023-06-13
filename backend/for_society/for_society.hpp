#pragma once
#include <iostream>

namespace FS {

template <uint64_t from_not, uint64_t to_not>
uint64_t FromNotToNot(uint64_t number) noexcept {
  uint64_t result = 0;

  for (uint64_t multiply = 1; number != 0; number /= 10, multiply *= from_not) {
    result += multiply * (number % 10);
  }

  number = 1;
  for (; result != 0; result /= to_not) {
    number *= 10;
    number += result % to_not;
  }
  for (; number != 1; number /= 10) {
    result *= 10;
    result += number % 10;
  }

  return result;
}

// extract digit from integer by index
uint32_t GetDigit(uint32_t number, uint32_t index) noexcept;

}  // namespace FS

namespace CE {
const std::chrono::milliseconds kWait = std::chrono::milliseconds(500);

const char kMessageQueueFile[] = "message_queue_file";

enum MessageToVisualize { UpdateData, Error, GoodBie };

const uint32_t kOperationQuantity = 96;
enum Mode { Working, Programming, ExecutingProg, TurnedOff };
enum Button {
  ButP = 0,
  ButF = 1,
  ButNull = 2,

  ButNum0 = 421,
  ButArrowUp = 653,
  ButNum1 = 1421,
  ButXArrowY = 1653,
  ButNum2 = 2421,
  ButMultiply = 2653,
  ButNum3 = 3421,
  ButDivision = 3653,
  ButXExpY = 3879,
  ButNum4 = 4421,
  ButComma = 4653,
  ButBO = 4879,
  ButNum5 = 5421,
  ButPrefixMinus = 5653,
  ButBbP = 5879,
  ButNum6 = 6421,
  ButBvP = 6653,
  ButPP = 6879,
  ButNum7 = 7421,
  ButCx = 7666,
  ButCP = 7879,
  ButNum8 = 8421,
  ButMinus = 8653,
  ButNum9 = 9421,
  ButPlus = 9653,

  ButStepLeft,
  ButStepRight
};

}  // namespace CE

namespace CP {
const uint32_t kNotation = 6;
const uint32_t kProgBufferSize = 60;
const uint32_t kMaxOperationNum = 96;

enum OperationCodes {
  OpTrash = -1,
  OpNeutral = 0,
  OpP0 = 1,
  OpF0 = 2,
  OpPArrowUp = 3,
  OpNum0 = 4,
  OpFArrowUp = 5,
  OpArrowUp = 6,
  OpPNum1 = 11,
  OpFNum1 = 12,
  OpPXArrowY = 13,
  OpNum1 = 14,
  OpFXArrowY = 15,
  OpXArrowY = 16,
  OpPNum2 = 21,
  OpFNum2 = 22,
  OpPMultiply = 23,
  OpFNUm2 = 24,
  OpFMultiply = 25,
  OpMultiply = 26,
  OpPNum3 = 31,
  OpFNum3 = 32,
  OpPDivision = 33,
  OpNum3 = 34,
  OpFDivision = 35,
  OpDivision = 36,
  OpFXExpY = 37,
  OpXExpY = 38,
  OpPXExpY = 39,
  OpPNum4 = 41,
  OpFNum4 = 42,
  OpPComma = 43,
  OpNum4 = 44,
  OpFComma = 45,
  OpComma = 46,
  OpFBO = 47,
  OpBO = 48,
  OpPBO = 49,
  OpPNum5 = 51,
  OpFNum5 = 52,
  OpPPrefixMinus = 53,
  OpNum5 = 54,
  OpFPrefixMinus = 55,
  OpPrefixMinus = 56,
  OpFBbP = 57,
  OpBbP = 58,
  OpPBbP = 59,
  OpPNum6 = 61,
  OpFNum6 = 62,
  OpPBvP = 63,
  OpNum6 = 64,
  OpFBvP = 65,
  OpBvP = 66,
  OpFPP = 67,
  OpPP = 68,
  OpPPP = 69,
  OpPNum7 = 71,
  OpFNum7 = 72,
  OpNum7 = 74,
  OpCx = 76,
  OpFCP = 77,
  OpCP = 78,
  OpPCP = 79,
  OpPNum8 = 81,
  OpFNum8 = 82,
  OpPMinus = 83,
  OpNum8 = 84,
  OpFMinus = 85,
  OpMinus = 86,
  OpPNum9 = 91,
  OpFNum9 = 92,
  OpPPlus = 93,
  OpNum9 = 94,
  OpFPlus = 95,
  OpPlus = 96
};

enum TransferStatus {
  TsNoCommand,
  TsTransfer,
  TsNoTransfer,
  TsUnconditionalTransfer
};

enum Direction { DirLeft = -1, DirRight = 1 };

enum ProgramStatus { ContinueUpdate, ContinueNoUpdate, Stop, Error };

bool IsOperationCode(int) noexcept;

}  // namespace CP