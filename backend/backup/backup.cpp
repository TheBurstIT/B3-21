#include "backup.hpp"

#include <fstream>
#include <string>
#include <vector>

void BU::MakeBackup(const CE::Calc& to_backup, const std::string& file_path) {
  CP::Program program = to_backup.GetProgram();
  CM::Buffer buffer = to_backup.GetRegisterBuffer();
  int button = to_backup.GetCurrFuncButton();
  int mode = to_backup.GetMode();

  std::vector<CP::OperationCodes> program_data = program.GetProgram();
  int program_step = program.GetStep();
  int program_transfer = program.GetTransferStatus();

  std::vector<CN::Number> buffer_body = buffer.GetNumeratedBuffer();
  std::vector<CN::Number> round_body = buffer.GetRoundedBuffer();

  std::ofstream output_file(file_path,
                            std::ofstream::out | std::ofstream::trunc);
  if (!output_file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    throw errno;
  }

  output_file << button << std::endl;
  output_file << mode << std::endl;

  for (size_t i = 0; i < program_data.size(); ++i) {
    output_file << program_data[i] << std::endl;
  }

  output_file << program_step << std::endl;
  output_file << program_transfer << std::endl;

  for (size_t i = 0; i < buffer_body.size(); ++i) {
    CN::Number::BackUpIng num = buffer_body[i].GetClass();
    output_file << num.sign << std::endl;
    output_file << num.number << std::endl;
    output_file << num.characteristic << std::endl;
    output_file << num.new_characteristic << std::endl;
    output_file << num.mode << std::endl;
  }

  for (size_t i = 0; i < round_body.size(); ++i) {
    CN::Number::BackUpIng num = round_body[i].GetClass();
    output_file << num.sign << std::endl;
    output_file << num.number << std::endl;
    output_file << num.characteristic << std::endl;
    output_file << num.new_characteristic << std::endl;
    output_file << num.mode << std::endl;
  }

  output_file.close();
}

CE::Calc BU::RestoreFromBackup(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file) {
    throw std::invalid_argument("Failed to open file");
  }

  int button_cp;
  file >> button_cp;
  int mode_cp;
  file >> mode_cp;

  std::vector<CP::OperationCodes> program_data_cp(CP::kProgBufferSize);
  for (int i = 0; i < CP::kProgBufferSize; ++i) {
    int operation_code;
    file >> operation_code;
    if (!CP::IsOperationCode(operation_code)) {
      throw std::invalid_argument("Is not an operation code");
    }
    program_data_cp[i] = static_cast<CP::OperationCodes>(operation_code);
  }

  int program_step_cp;
  file >> program_step_cp;
  int program_transfer_cp;
  file >> program_transfer_cp;

  std::vector<CN::Number::BackUpIng> backUping_cp(CM::kNumeratedBuffSize);
  for (int i = 0; i < backUping_cp.size(); ++i) {
    file >> backUping_cp[i].sign;
    file >> backUping_cp[i].number;
    file >> backUping_cp[i].characteristic;
    file >> backUping_cp[i].new_characteristic;
    int mode;
    file >> mode;
    backUping_cp[i].mode = static_cast<CN::EnterMode>(mode);
  }

  std::vector<CN::Number::BackUpIng> backUping_cp_round(CM::kRoundedBuffSize);
  for (int i = 0; i < backUping_cp_round.size(); ++i) {
    file >> backUping_cp_round[i].sign;
    file >> backUping_cp_round[i].number;
    file >> backUping_cp_round[i].characteristic;
    file >> backUping_cp_round[i].new_characteristic;
    int mode;
    file >> mode;
    backUping_cp_round[i].mode = static_cast<CN::EnterMode>(mode);
  }

  CP::Program program(program_data_cp, program_step_cp,
                      static_cast<CP::TransferStatus>(program_transfer_cp));

  std::vector<CN::Number> numerated_buffer;
  numerated_buffer.reserve(backUping_cp.size());
  for (int i = 0; i < numerated_buffer.capacity(); ++i) {
    numerated_buffer.emplace_back(backUping_cp[i].sign, backUping_cp[i].number,
                                  backUping_cp[i].characteristic,
                                  backUping_cp[i].new_characteristic,
                                  backUping_cp[i].mode);
  }
  std::vector<CN::Number> rounded_buffer;
  rounded_buffer.reserve(backUping_cp_round.size());
  for (int i = 0; i < rounded_buffer.capacity(); ++i) {
    rounded_buffer.emplace_back(
        backUping_cp_round[i].sign, backUping_cp_round[i].number,
        backUping_cp_round[i].characteristic,
        backUping_cp_round[i].new_characteristic, backUping_cp_round[i].mode);
  }
  CM::Buffer buffer(numerated_buffer, rounded_buffer);
  return CE::Calc(program, buffer, static_cast<CE::Button>(button_cp),
                  static_cast<CE::Mode>(mode_cp));
}

void BU::SaveProgram(const std::vector<CP::OperationCodes>& to_save, const std::string& file_path) {
  std::ofstream output_file(file_path,
                            std::ofstream::out | std::ofstream::trunc);
  if (!output_file.is_open()) {
    std::cerr << "Failed to open file" << std::endl;
    throw std::invalid_argument("Failed to open file");
  }
  for(int i = 0; i < to_save.size(); ++i) {
    output_file << static_cast<int>(to_save[i]) << "\n";
  }
  output_file.close();
}

std::vector<CP::OperationCodes> BU::LoadProgram(const std::string& file_path) {
  std::ifstream file(file_path);
  if (!file) {
    throw std::invalid_argument("Failed to open file");
  }
  std::vector<CP::OperationCodes> result(CP::kProgBufferSize);
  for(int i = 0; i < CP::kProgBufferSize; ++i) {
    int operation_code;
    file >> operation_code;
    if (!CP::IsOperationCode(operation_code)) {
      throw std::invalid_argument("Is not an operation code");
    }
    result[i] = static_cast<CP::OperationCodes>(operation_code);
  }
  return result;
}
