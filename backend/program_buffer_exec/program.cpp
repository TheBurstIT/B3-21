#include "program.hpp"

#include <algorithm>
#include <optional>

#include "backend/for_society/for_society.hpp"
#include "backend/calc_exec/calc_exec.hpp"

namespace CP {

/*------------------------ вспомогательные функции ---------------------------*/
TransferStatus IsTransfer(const CN::Number& number,
                          OperationCodes operation) noexcept {
  switch (operation) {
    case OpPBO:
      return number >= 0 ? TsNoTransfer : TsTransfer;
    case OpPCP:
      return number != 0 ? TsNoTransfer : TsTransfer;
    case OpPBbP:
      return number == 0 ? TsNoTransfer : TsTransfer;
    case OpPPP:
      return number < 0 ? TsNoTransfer : TsTransfer;
    case OpBbP:
      return TsUnconditionalTransfer;
    default:
      return TsNoCommand;
  }
}

/*---------------------------- для визуализации ------------------------------*/
const std::vector<OperationCodes>& Program::GetProgram() const noexcept {
  return data_;
}

const uint32_t& Program::GetStep() const noexcept { return step_; }

const TransferStatus& Program::GetTransferStatus() const noexcept {
  return transfer_status_;
}

/*------------------------ интерфейс взаимодействия --------------------------*/
void Program::EnterCode(OperationCodes code) noexcept {
  ResetTransferStatus();
  data_[step_] = code;
  MakeStep(DirRight);
}

ProgramStatus Program::ExecuteStep(CE::Calc& calc) noexcept {
  try {
    FindEnd();
  } catch (std::overflow_error& error) {
    return Error;
  }

  // проверка на конец программы
  if (IsThereEndBefore()) {
    if (step_ != kProgBufferSize - 1 && data_[step_] != OpCP) {
      MakeStep(DirRight);
    }
    return Stop;
  }

  // проверка на мусор
  if (data_[step_] == OpTrash) {
    return Error;
  }

  // проверка наличия ветвления
  try {
    auto forking = IsFork(calc);
    if (forking.has_value()) {
      step_ = forking.value();
      if (IsEnd()) {
        return Stop;
      }

      if (step_ < kProgBufferSize) {
        return ContinueUpdate;
      }

      return Error;
    }
  } catch (...) {
    return Error;
  }

  // стандартный случай
  calc.ExecuteCommandInProgram(data_[step_]);
  if (IsAbleToStepRight()) {
    step_ += 1;
    return ContinueNoUpdate;
  }

  return Error;
}

void Program::StepToZero() noexcept {
  ResetTransferStatus();
  step_ = 0;
}

void Program::MakeStep(Direction direction) noexcept {
  ResetTransferStatus();
  if (step_ == 0 && direction == DirLeft) {
    return;
  }
  if (step_ == kProgBufferSize - 1 && direction == DirRight) {
    return;
  }
  step_ += direction;
}

/*------------------------------ for restore ---------------------------------*/
Program::Program(const std::vector<OperationCodes>& data, uint32_t step,
                 TransferStatus transfer_status)
    : data_(data), step_(step), transfer_status_(transfer_status) {}

void Program::SetProgram(const std::vector<OperationCodes>& program) {
  for (size_t i = 0; i < program.size(); ++i) {
    data_[i] = program[i];
  }
  for (size_t i = program.size(); i < data_.size() - program.size(); ++i) {
    data_[i] = OpTrash;
  }
}

/*---------------------------- приватные методы ------------------------------*/
void Program::ResetTransferStatus() noexcept { transfer_status_ = TsNoCommand; }

/*--------------------------- проверочные методы -----------------------------*/
int Program::FindEnd() const {
  auto end = std::find(data_.begin(), data_.end(), OpCP);
  if (end == data_.end()) {
    throw std::overflow_error("End is not found");
  }

  return end - data_.begin();
}

bool Program::IsThereEndBefore() const noexcept {
  return std::find(data_.begin(), data_.begin() + step_ + 1, OpCP) !=
         data_.begin() + step_ + 1;
}

bool Program::IsEnd() const noexcept { return data_[step_] == OpCP; }

bool Program::IsAbleToStepRight() const noexcept {
  return step_ < kProgBufferSize - 1;
}

std::optional<uint32_t> Program::IsFork(const CE::Calc& calc) const {
  // проверка состояния перехода
  if (transfer_status_ == TsTransfer) {
    // проверка корректности индекса
    if (FS::GetDigit(data_[step_], 0) >= kNotation ||
        FS::GetDigit(data_[step_], 1) >= kNotation) {
      throw data_[step_];
    }
    transfer_status_ = TsNoTransfer;
    // перевод индекса в десятичную систему
    return FS::FromNotToNot<kNotation, 10>(data_[step_]);
  }

  // может мы стоим на команде перехода?
  switch (IsTransfer(calc.GetRegisterBuffer().GetNumeratedBuffer()[0],
                     data_[step_])) {
    case TsUnconditionalTransfer:
      return FindEnd();
    case TsNoTransfer:
      return step_ + 2;
    case TsTransfer:
      transfer_status_ = TsTransfer;
      return step_ + 1;
    default:
      return {};
  }
}

}  // namespace CP