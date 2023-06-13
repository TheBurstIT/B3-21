#include "visualization.hpp"

namespace IV {
std::string FilterTrash(CP::OperationCodes code, bool show = false) {
  return code == CP::OpTrash ? show ? "Trash" : "" : std::to_string(code);
}

Visualization::Visualization(std::shared_ptr<CE::Calc> calc,
                             const ID::VisualisationTemplate& vis_temp) {
  step_ = TextBlock(vis_temp.step);
  calc_ = calc;

  main_number_.first = TextBlock(vis_temp.main_number.number);
  main_number_.second = TextBlock(vis_temp.main_number.characteristic);

  last_operations_.resize(vis_temp.last_operations.table.size());
  for (size_t i = 0; i < last_operations_.size(); ++i) {
    last_operations_.at(i) = TextBlock(vis_temp.last_operations.table.at(i));
  }

  mode_ = TextBlock(vis_temp.mode);

  function_button_ = TextBlock(vis_temp.function_button);

  program_.resize(vis_temp.program.table.size());
  for (size_t i = 0; i < program_.size(); ++i) {
    program_.at(i) = TextBlock(vis_temp.program.table.at(i));
  }

  numerated_buffer_.first.resize(vis_temp.numerated_buffer.number.table.size());
  for (size_t i = 0; i < numerated_buffer_.first.size(); ++i) {
    numerated_buffer_.first.at(i) =
        TextBlock(vis_temp.numerated_buffer.number.table.at(i));
  }
  numerated_buffer_.second.resize(
      vis_temp.numerated_buffer.characteristic.table.size());
  for (size_t i = 0; i < numerated_buffer_.second.size(); ++i) {
    numerated_buffer_.second.at(i) =
        TextBlock(vis_temp.numerated_buffer.characteristic.table.at(i));
  }

  rounded_buffer_.first.resize(vis_temp.rounded_buffer.number.table.size());
  for (size_t i = 0; i < rounded_buffer_.first.size(); ++i) {
    rounded_buffer_.first.at(i) =
        TextBlock(vis_temp.rounded_buffer.number.table.at(i));
  }
  rounded_buffer_.second.resize(
      vis_temp.rounded_buffer.characteristic.table.size());
  for (size_t i = 0; i < rounded_buffer_.second.size(); ++i) {
    rounded_buffer_.second.at(i) =
        TextBlock(vis_temp.rounded_buffer.characteristic.table.at(i));
  }
  UpdateData();
}

void Visualization::UpdateData() {
  if (calc_->GetMode() == CE::TurnedOff) {
    Clear();
    return;
  }
  const auto& program_buf = calc_->GetProgram();
  const auto& num_buf = calc_->GetRegisterBuffer().GetNumeratedBuffer();
  auto rounded_buf = calc_->GetRegisterBuffer().GetRoundedBuffer();

  /*--------------------------------- step -----------------------------------*/
  step_.Update(std::to_string(FS::FromNotToNot<10, 6>(program_buf.GetStep())));

  /*-------------------------- step highlighting -----------------------------*/
  if (step_mem_ != -1) {
    if (step_mem_ != program_buf.GetStep()) {
      program_.at(step_mem_).SwitchFont();
      step_mem_ = program_buf.GetStep();
      program_.at(step_mem_).SwitchFont();
    }
  } else {
    step_mem_ = program_buf.GetStep();
    program_.at(step_mem_).SwitchFont();
  }

  /*----------------------------- main number --------------------------------*/
  {
    const auto& [characteristic, number] = num_buf.at(0).GetMainNumber();
    main_number_.first.Update(number);
    main_number_.second.Update(std::to_string(characteristic));
  }

  /*--------------------------- last operations ------------------------------*/

  for (size_t i = 0; i < last_operations_.size(); ++i) {
    if (static_cast<int32_t>(program_buf.GetStep()) - static_cast<int32_t>(i) <
        0) {
      last_operations_.at(i).Update("");
    } else {
      last_operations_.at(i).Update(
          FilterTrash(program_buf.GetProgram().at(program_buf.GetStep() - i)));
    }
  }

  /*--------------------------- function button ------------------------------*/
  {
    auto func_but = calc_->GetCurrFuncButton();
    function_button_.Update(
        func_but == CE::ButP ? "P" : (func_but == CE::ButF ? "F" : "NULL"));
  }

  /*--------------------------------- mode -----------------------------------*/
  {
    auto mode = calc_->GetMode();
    mode_.Update(
        mode == CE::Working
            ? "Working"
            : (mode == CE::Programming
                   ? "Programming"
                   : (mode == CE::ExecutingProg ? "Executing" : "TurnedOff")));
  }

  /*--------------------------- program buffer -------------------------------*/
  for (size_t i = 0; i < program_.size(); ++i) {
    program_.at(i).Update(FilterTrash(program_buf.GetProgram().at(i), true));
  }

  /*--------------------------- numerated buffer -----------------------------*/
  {
    for (int i = 0; i < numerated_buffer_.first.size(); ++i) {
      const auto& [characteristic, number] = num_buf.at(i).GetStaticNumber();
      numerated_buffer_.second.at(i).Update(std::to_string(characteristic));
      numerated_buffer_.first.at(i).Update(number);
    }
  }

  /*---------------------------- rounded buffer ------------------------------*/
  {
    {
      const auto& [characteristic, number] = num_buf.at(0).GetStaticNumber();
      rounded_buffer_.second.at(0).Update(std::to_string(characteristic));
      rounded_buffer_.first.at(0).Update(number);
    }

    for (int i = 0; i < rounded_buffer_.first.size() - 1; ++i) {
      const auto& [characteristic, number] =
          rounded_buf.at(i).GetStaticNumber();
      rounded_buffer_.second.at(i + 1).Update(std::to_string(characteristic));
      rounded_buffer_.first.at(i + 1).Update(number);
    }
  }
}

void Visualization::Clear() {
  /*--------------------------------- step -----------------------------------*/
  step_.Update("");

  /*----------------------------- main number --------------------------------*/
  {
    main_number_.first.Update("");
    main_number_.second.Update("");
  }

  /*--------------------------- last operations ------------------------------*/

  for (size_t i = 0; i < last_operations_.size(); ++i) {
    last_operations_.at(i).Update("");
  }

  /*--------------------------- function button ------------------------------*/
  { function_button_.Update(""); }

  /*--------------------------------- mode -----------------------------------*/
  {
    auto mode = calc_->GetMode();
    mode_.Update(
        mode == CE::Working
            ? "Working"
            : (mode == CE::Programming
                   ? "Programming"
                   : (mode == CE::ExecutingProg ? "Executing" : "TurnedOff")));
  }

  /*--------------------------- program buffer -------------------------------*/
  for (size_t i = 0; i < program_.size(); ++i) {
    program_.at(i).Update("");
  }

  /*--------------------------- numerated buffer -----------------------------*/
  {
    for (int i = 0; i < numerated_buffer_.first.size(); ++i) {
      numerated_buffer_.second.at(i).Update("");
      numerated_buffer_.first.at(i).Update("");
    }
  }

  /*---------------------------- rounded buffer ------------------------------*/
  {
    for (int i = 0; i < rounded_buffer_.first.size(); ++i) {
      rounded_buffer_.second.at(i).Update("");
      rounded_buffer_.first.at(i).Update("");
    }
  }
  wxYield();
}

TextBlock::TextBlock(const ID::TextBlock& raw) {
  curr_text_ = new wxStaticText(raw.panel, raw.id, raw.text, raw.location);
  curr_text_->SetFont(raw.font.first.font);
  curr_text_->SetForegroundColour(raw.font.first.colour);
  fonts_ = raw.font;
}
TextBlock::TextBlock(IV::TextBlock&& outer) {
  fonts_ = outer.fonts_;
  curr_text_ = outer.curr_text_;
  curr_font_ = outer.curr_font_;

  outer.curr_text_ = nullptr;
}

TextBlock& TextBlock::operator=(IV::TextBlock&& outer) {
  delete curr_text_;
  curr_text_ = outer.curr_text_;
  outer.curr_text_ = nullptr;

  curr_font_ = outer.curr_font_;
  fonts_ = outer.fonts_;
  return *this;
}

TextBlock::~TextBlock() { delete curr_text_; }

void TextBlock::Update(const std::string& str) {
  if (curr_text_->GetLabel() != str) {
    curr_text_->SetLabel(str);
  }
}

void TextBlock::SwitchFont() {
  curr_font_ = !(curr_font_);
  curr_text_->SetFont(curr_font_ ? fonts_.second.font : fonts_.first.font);
  curr_text_->SetForegroundColour(curr_font_ ? fonts_.second.colour
                                             : fonts_.first.colour);
}

}  // namespace IV