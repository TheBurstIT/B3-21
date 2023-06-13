#include "calc_exec.hpp"

namespace CE {
/*------------------------ вспомогательные функции ---------------------------*/
uint32_t Calc::number_of_class_objects = 1;

/*------------ конструкторы / деструктор / оператор присваивания -------------*/
Calc::Calc() {
  program_ = new CP::Program();
  number_of_class_objects += 1;
}

Calc::Calc(const CE::Calc& other)
    : program_(other.program_),
      buffer_(other.buffer_),
      curr_func_button_(other.curr_func_button_),
      mode_(other.mode_) {
  program_ = new CP::Program();
  number_of_class_objects += 1;
}

Calc::~Calc() {
  delete program_;
  if (exec_prog_thread_.has_value()) {
    mode_ = Working;
    exec_prog_thread_.value().join();
  }
  try {
    SendSignal(GoodBie);
  } catch (...) {
  }
}

Calc& Calc::operator=(const CE::Calc& other) {
  if (exec_prog_thread_.has_value()) {
    mode_ = Working;
    exec_prog_thread_.value().join();
    exec_prog_thread_ = {};
  }

  *program_ = *other.program_;
  buffer_ = other.buffer_;
  curr_func_button_ = other.curr_func_button_;
  mode_ = other.mode_;

  SendSignal(UpdateData);

  return *this;
}

Calc& Calc::operator=(CE::Calc&& other) {
  if (exec_prog_thread_.has_value()) {
    mode_ = Working;
    exec_prog_thread_.value().join();
    exec_prog_thread_ = {};
  }

  program_ = other.program_;
  other.program_ = nullptr;

  buffer_ = std::move(other.buffer_);
  curr_func_button_ = other.curr_func_button_;
  mode_ = other.mode_;

  SendSignal(UpdateData);

  return *this;
}

/*---------------------------- для визуализации ------------------------------*/
const CP::Program& Calc::GetProgram() const noexcept { return *program_; }

const CM::Buffer& Calc::GetRegisterBuffer() const noexcept { return buffer_; }

const Button& Calc::GetCurrFuncButton() const noexcept {
  return curr_func_button_;
}

const Mode& Calc::GetMode() const noexcept { return mode_; }

MQ::MessageQueue Calc::GetDataUpdateMarker() const noexcept {
  return update_signal_;
}

/*------------------------ интерфейс взаимодействия --------------------------*/
void Calc::PressButton(Button button) {
  if (mode_ == TurnedOff) {
    return;
  }

  if (mode_ == ExecutingProg) {
    PressedButtonExecutingProg(button);
    return;
  }

  if (button == ButP || button == ButF) {
    PressedFuncButton(button);
    return;
  }

  // команда смены режима
  auto new_mode = IsChangingModeCommand(button);
  if (new_mode.has_value()) {
    ChangeMode(new_mode.value());
    return;
  }

  if (mode_ == Working) {
    PressedButtonWorking(button);
    return;
  }

  if (mode_ == Programming) {
    PressedButtonProgramming(button);
    return;
  }
}

void Calc::TurnOnOff() noexcept {
  if (mode_ == TurnedOff) {
    mode_ = Working;
  } else {
    *this = Calc();
  }

  SendSignal(UpdateData);
}

/*------------------------------ for restore ---------------------------------*/
Calc::Calc(const CP::Program& program_buffer, const CM::Buffer& register_buffer,
           CE::Button curr_func_button, CE::Mode mode)
    : program_(new CP::Program(program_buffer)),
      buffer_(register_buffer),
      curr_func_button_(curr_func_button),
      mode_(mode) {
  number_of_class_objects += 1;
}

void Calc::SetProgram(const std::vector<CP::OperationCodes>& program) {
  if (mode_ == TurnedOff) {
    return;
  }

  if (mode_ == ExecutingProg) {
    mode_ = Working;
  }
  program_->SetProgram(program);

  SendSignal(UpdateData);
}

/*---------------------------- приватные методы ------------------------------*/
void Calc::ChangeMode(Mode new_mode) {
  curr_func_button_ = ButNull;
  mode_ = new_mode;
  SendSignal(UpdateData);
}

void Calc::PressedButtonWorking(Button button) {
  if (button == ButStepRight) {
    CP::ProgramStatus status = program_->ExecuteStep(*this);
    switch (status) {
      case CP::ContinueUpdate:
        SendSignal(UpdateData);
        break;
      case CP::Stop:
        SendSignal(UpdateData);
        break;
      case CP::Error:
        SendSignal(Error);
        break;
    }
  } else if (button == ButStepLeft) {
    program_->MakeStep(CP::DirLeft);
    SendSignal(UpdateData);
  } else {
    ExecuteCommand(GetOperationCode(button));
  }
}

void Calc::PressedButtonProgramming(Button button) {
  if (button == ButStepRight) {
    program_->MakeStep(CP::DirRight);
  } else if (button == ButStepLeft) {
    program_->MakeStep(CP::DirLeft);
  } else {
    program_->EnterCode(GetOperationCode(button));
    curr_func_button_ = ButNull;
  }
  SendSignal(UpdateData);
}

void Calc::PressedButtonExecutingProg(CE::Button button) {
  if (button == ButStepLeft) {
    ChangeMode(Working);
  }
}

void Calc::PressedFuncButton(Button button) {
  curr_func_button_ = button;
  SendSignal(UpdateData);
}

void Calc::ExecuteCommand(CP::OperationCodes operation) {
  auto num = IsNum(operation);
  if (num.has_value()) {
    Num(num.value());
    return;
  }

  auto pf_num = IsPFNum(operation);
  if (pf_num.has_value()) {
    pf_num.value().first == ButP ? PNum(pf_num.value().second)
                                 : FNum(pf_num.value().second);
    return;
  }

  (this->*method_ptr_[operation])();
}
void Calc::ExecuteCommandInProgram(CP::OperationCodes operation) {
  auto num = IsNum(operation);
  if (num.has_value()) {
    buffer_.GetX0().ClearButton();
  }

  ExecuteCommand(operation);
}

void Calc::SendSignal(MessageToVisualize message) {
  update_signal_.Send(message, 1);
}

/*--------------------------- проверочные методы -----------------------------*/
CP::OperationCodes Calc::GetOperationCode(CE::Button button) const noexcept {
  return static_cast<CP::OperationCodes>(
      (button / 1000) * 10 + FS::GetDigit(button, curr_func_button_));
}

std::optional<Mode> Calc::IsChangingModeCommand(
    CE::Button button) const noexcept {
  if (curr_func_button_ == ButP) {
    if (button == ButStepRight) {
      return Working;
    }
    if (button == ButStepLeft) {
      return Programming;
    }
  }

  return {};
}

std::optional<std::pair<CE::Button, uint32_t>> Calc::IsPFNum(
    CP::OperationCodes operation) noexcept {
  if (FS::GetDigit(operation, 0) == 1) {
    return {{ButP, FS::GetDigit(operation, 1)}};
  }
  if (FS::GetDigit(operation, 0) == 2) {
    return {{ButF, FS::GetDigit(operation, 1)}};
  }
  return {};
}

std::optional<uint32_t> Calc::IsNum(CE::Button button) noexcept {
  if (FS::GetDigit(button, 2) == 4) {
    return FS::GetDigit(button, 3);
  }

  return {};
}

std::optional<uint32_t> Calc::IsNum(CP::OperationCodes operation) noexcept {
  if (FS::GetDigit(operation, 0) == 4) {
    return FS::GetDigit(operation, 1);
  }

  return {};
}

/*-------------------------- элементарные функции ----------------------------*/
void Calc::PNum(uint32_t num) {
  try {
    buffer_.PutFromXToZ(num);
  } catch (...) {
    SendSignal(Error);
    return;
  }

  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::FNum(uint32_t num) {
  try {
    buffer_.PutFromZToX(num);
  } catch (...) {
    SendSignal(Error);
    return;
  }

  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::Num(uint32_t digit) {
  buffer_.GetX0().NumberButton(digit);
  SendSignal(UpdateData);
}

}  // namespace CE