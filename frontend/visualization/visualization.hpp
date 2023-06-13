#pragma once

#include <wx/stattext.h>
#include <wx/wx.h>

#include <memory>
#include <string>

#include "backend/calc_exec/calc_exec.hpp"
#include "frontend/interface/definition/interface_def.hpp"

namespace IV {
class TextBlock {
 public:
  TextBlock() = default;
  explicit TextBlock(const ID::TextBlock&);
  TextBlock(const TextBlock&) = delete;
  TextBlock(TextBlock&&);

  ~TextBlock();

  TextBlock& operator=(const TextBlock&) = delete;
  TextBlock& operator=(TextBlock&&);

  void Update(const std::string&);
  void SwitchFont();

 private:
  bool curr_font_ = false;
  std::pair<ID::Font, ID::Font> fonts_;
  wxStaticText* curr_text_ = nullptr;
};

// класс - визуализатор
class Visualization {
 public:
  // конструкторы и деструкторы
  Visualization() = default;
  Visualization(std::shared_ptr<CE::Calc>,
                const ID::VisualisationTemplate& vis_temp);
  Visualization(const Visualization&) = delete;
  Visualization(Visualization&&) = delete;

  ~Visualization() = default;

  // операторы присваивания
  Visualization& operator=(const Visualization&) = delete;
  Visualization& operator=(Visualization&&) = default;

  // интерфейс взаимодействия
  void UpdateData();

 private:
  std::shared_ptr<CE::Calc> calc_;

  void Clear();
  TextBlock step_;
  int step_mem_ = -1;
  std::pair<TextBlock, TextBlock> main_number_;
  std::vector<TextBlock> last_operations_;
  TextBlock mode_;
  TextBlock function_button_;
  std::vector<TextBlock> program_;
  std::pair<std::vector<TextBlock>, std::vector<TextBlock>>
      numerated_buffer_;
  std::pair<std::vector<TextBlock>, std::vector<TextBlock>> rounded_buffer_;
};

}  // namespace IV