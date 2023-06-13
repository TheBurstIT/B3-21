#pragma once

#include <optional>
#include <vector>

#include "backend/calc_exec/calc_exec.hpp"
#include "backend/for_society/for_society.hpp"

namespace CE {
class Calc;
}

namespace CP {

class Program {
 public:
  Program() = default;
  Program(const Program&) = default;
  Program& operator=(const Program&) = default;

  // для визуализации
  const std::vector<OperationCodes>& GetProgram() const noexcept;
  const uint32_t& GetStep() const noexcept;
  const TransferStatus& GetTransferStatus() const noexcept;

  // интерфейс взаимодействия
  void EnterCode(OperationCodes) noexcept;

  ProgramStatus ExecuteStep(CE::Calc&) noexcept;

  void StepToZero() noexcept;
  void MakeStep(Direction) noexcept;

  // for restore
  Program(const std::vector<OperationCodes>& data, uint32_t step,
          TransferStatus transfer_status);
  void SetProgram(const std::vector<OperationCodes>& program);

 private:
  std::vector<OperationCodes> data_ =
      std::vector<OperationCodes>(kProgBufferSize, OpTrash);
  uint32_t step_ = 0;
  mutable TransferStatus transfer_status_ = TsNoCommand;

  void ResetTransferStatus() noexcept;

  // проверочные методы
  int FindEnd() const;
  bool IsThereEndBefore() const noexcept;
  bool IsEnd() const noexcept;
  bool IsAbleToStepRight() const noexcept;
  std::optional<uint32_t> IsFork(const CE::Calc&) const;
};

}  // namespace CP