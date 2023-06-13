#include "calc_exec.hpp"
#include "calc_math.hpp"

namespace CE {

void Calc::CP() {
  // подготовка
  if (exec_prog_thread_.has_value()) {
    exec_prog_thread_.value().join();
  }
  ChangeMode(ExecutingProg);

  // распараллеливание
  exec_prog_thread_ = std::thread([this]() {
    // исполняем, пока можно
    while (mode_ == ExecutingProg) {
      // выполняем команду
      CP::ProgramStatus executing_result = program_->ExecuteStep(*this);

      // проверяем результат выполнения и обновляем визуализацию
      if (executing_result == CP::ContinueUpdate) {
        SendSignal(UpdateData);
      } else if (executing_result == CP::Stop) {
        break;
      } else if (executing_result == CP::Error) {
        SendSignal(Error);
        break;
      }

      // здоровый сон
      std::this_thread::sleep_for(kWait);
    }

    ChangeMode(Working);
  });
}

void Calc::Neutral() {
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PArrowUp() {
  CN::Number& x_0 = buffer_.GetX0();
  try {
    x_0 = Math::Exp(x_0);
  } catch (...) {
    SendSignal(Error);
    return;
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::ArrowUp() {
  buffer_.PutFromXToZ(1);
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PXArrowY() {
  CN::Number& x_0 = buffer_.GetX0();
  try {
    x_0 = Math::Ln(x_0);
  } catch (...) {
    SendSignal(Error);
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::XArrowY() {
  CN::Number tmp = buffer_.GetNumeratedBuffer()[1];
  buffer_.PutFromXToZ(1);
  buffer_.GetX0() = tmp;
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PMultiply() {
  CN::Number& x_0 = buffer_.GetX0();
  try {
    x_0 = Math::kPi;
  } catch (...) {
    SendSignal(Error);
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::Multiply() {
  CN::Number y = buffer_.GetNumeratedBuffer()[1];
  CN::Number& x = buffer_.GetX0();
  try {
    x = x * y;
  } catch (...) {
    SendSignal(Error);
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PDivision() { PArrowUp(); }

void Calc::Division() {
  CN::Number y = buffer_.GetNumeratedBuffer()[1];
  CN::Number& x = buffer_.GetX0();
  try {
    x = x / y;
  } catch (...) {
    SendSignal(Error);
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::XExpY() {
  CN::Number y = buffer_.GetNumeratedBuffer()[1];
  CN::Number& x = buffer_.GetX0();
  try {
    x = Math::Pow(x, y);
  } catch (...) {
    SendSignal(Error);
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PComma() {
  buffer_.RoundBuffer(CM::ContrClockWise);
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::FComma() {
  CN::Number& x = buffer_.GetX0();
  try {
    x = CN::Number(1) / x;
  } catch (...) {
    SendSignal(Error);
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::Comma() {
  buffer_.GetX0().DotButton();
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::BO() {
  program_->StepToZero();
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PPrefixMinus() {
  buffer_.RoundBuffer(CM::ClockWise);
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::FPrefixMinus() {
  CN::Number& x = buffer_.GetX0();
  try {
    x = Math::Pow(x, 2);
  } catch (...) {
    SendSignal(Error);
    return;
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PrefixMinus() {
  buffer_.GetX0().SignButton();
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::FVP() {
  CN::Number& x = buffer_.GetX0();
  x = Math::Root(x);
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::VP() {
  buffer_.GetX0().CharacteristicButton();
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}
void Calc::Cx() {
  buffer_.GetX0().ClearButton();
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PMinus() {
  CN::Number& x = buffer_.GetX0();
  x = Math::Cos(x);
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::Minus() {
  CN::Number y = buffer_.GetNumeratedBuffer()[1];
  CN::Number& x = buffer_.GetX0();
  try {
    x = x - y;
  } catch (...) {
    SendSignal(Error);
    return;
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::PPlus() {
  CN::Number& x = buffer_.GetX0();
  x = Math::Sin(x);
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}

void Calc::Plus() {
  CN::Number y = buffer_.GetNumeratedBuffer()[1];
  CN::Number& x = buffer_.GetX0();
  try {
    x = x + y;
  } catch (...) {
    SendSignal(Error);
    return;
  }
  curr_func_button_ = ButNull;
  SendSignal(UpdateData);
}
};  // namespace CE
