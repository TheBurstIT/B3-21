#include <iostream>
#include <string>
#include <thread>

#include "backend/calc_exec/calc_exec.hpp"

void PrintCalc(const CE::Calc& calc) {
  std::cout << "Program:\n";
  for (int i = 0; i < CP::kProgBufferSize; ++i) {
    printf("%3d ", FS::FromNotToNot<10, 6>(i));
  }
  printf("\n");
  for (int i = 0; i < CP::kProgBufferSize; ++i) {
    printf("%3d ", calc.GetProgram().GetProgram()[i]);
  }
  std::cout << "\nSteps: "
            << FS::FromNotToNot<10, 6>(calc.GetProgram().GetStep());
  std::cout << "\nTransfer status: ";
  switch (calc.GetProgram().GetTransferStatus()) {
    case CP::TsTransfer:
      std::cout << "condition is not met - transfer\n";
      break;
    case CP::TsNoTransfer:
      std::cout << "condition is met - no transfer\n";
      break;
    case CP::TsUnconditionalTransfer:
      std::cout << "transfer\n";
    default:
      std::cout << "no command\n";
  }

  std::cout << "\nNumber buffer:\n";
  std::cout << "Numerated buffer:\n";
  for (int i = 0; i < CM::kNumeratedBuffSize; ++i) {
    auto [sign, characteristic, num] =
        calc.GetRegisterBuffer().GetNumeratedBuffer()[i].GetNumber();
    printf("%d. %d %c%s\n", i, characteristic, (sign ? '-' : ' '), num.c_str());
  }
  std::cout << "Rounded buffer\n";
  for (int i = 0; i < CM::kRoundedBuffSize - 1; ++i) {
    auto [sign, characteristic, num] =
        calc.GetRegisterBuffer().GetRoundedBuffer()[i].GetNumber();
    printf("%d. %d %c%s\n", i, characteristic, (sign ? '-' : ' '), num.c_str());
  }

  std::cout << "\nFunction Button: ";
  switch (calc.GetCurrFuncButton()) {
    case CE::ButP:
      std::cout << "P\n";
      break;
    case CE::ButF:
      std::cout << "F\n";
      break;
    default:
      std::cout << "NULL\n";
  }

  std::cout << "\nMode: ";
  switch (calc.GetMode()) {
    case CE::Working:
      std::cout << "working\n";
      break;
    case CE::Programming:
      std::cout << "programming\n";
      break;
    case CE::ExecutingProg:
      std::cout << "executing program\n";
      break;
    default:
      std::cout << "turned off\n";
  }
}

void Update(CE::Calc* calc) {
  MQ::MessageQueue queue = calc->GetDataUpdateMarker();
  PrintCalc(*calc);
  while (true) {
    switch (queue.Receive(1, MQ::Wait).value()) {
      case CE::UpdateData:
        PrintCalc(*calc);
        break;
      case CE::Error:
        std::cout << "Error!\n";
        break;
      default:
        std::cout << "GoodBie\n";
        return;
    }
  }
}

class Debug {
 public:
  Debug() { thread_ = std::thread(&Update, &calc_); }
  ~Debug() { thread_.detach(); }

  void EnterProgram(const std::vector<int>& buttons) {
    calc_.PressButton(CE::ButP);
    calc_.PressButton(CE::ButStepLeft);

    for (auto elem : buttons) {
      calc_.PressButton(static_cast<CE::Button>(elem));
      if (elem == CE::ButP || elem == CE::ButF) {
        continue ;
      }
      calc_.PressButton(CE::ButStepRight);
    }
  }

  void Work() {
    int input;
    while (true) {
      std::cin >> input;
      if (input == -2) {
        return;
      }
      if (input == -1) {
        calc_.TurnOnOff();
      } else if (input == -3) {
        int input;
        std::vector<int> program;
        while (true) {
          std::cin >> input;
          if (input < 0) {
            break;
          }
          program.push_back(input);
        }
        EnterProgram(program);
      } else {
        calc_.PressButton(static_cast<CE::Button>(input));
      }
    }
  }

 private:
  CE::Calc calc_;
  MQ::MessageQueue message_queue_ = calc_.GetDataUpdateMarker();
  std::thread thread_;
};

int main() {
  Debug debug;
  debug.Work();
}