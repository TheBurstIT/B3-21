#pragma once

#include <wx/stattext.h>
#include <wx/wx.h>

#include <string>
#include <vector>

#include "backend/calc_exec/calc_exec.hpp"

namespace ID {

const uint32_t kDefaultTextSize = 15;

struct Font {
  wxFont font;
  wxColour colour;
};

struct TableParameters {
  double init_x;
  double init_y;

  double raw_offset;
  double column_offset;

  uint32_t raw_num;
  uint32_t column_num;
};

struct TextBlock {
  wxWindow* panel = nullptr;
  int id = wxID_ANY;
  std::string text = " ";
  wxRealPoint location = wxRealPoint(0, 0);
  std::pair<Font, Font> font;
};

struct Number {
  TextBlock number;
  TextBlock characteristic;
};

struct TextBlockTable {
  TextBlockTable() = default;
  TextBlockTable(TableParameters table_parameters,
                 TextBlock template_text_block);

  void SetPanel(wxWindow*);

  std::vector<TextBlock> table;
};

struct NumberList {
  TextBlockTable number;
  TextBlockTable characteristic;
};

struct VisualisationTemplate {
  TextBlock step;
  Number main_number;
  TextBlockTable last_operations;
  TextBlock mode;
  TextBlock function_button;
  TextBlockTable program;
  NumberList numerated_buffer;
  NumberList rounded_buffer;

  VisualisationTemplate();
  void SetPanel(wxWindow*);
};

}  // namespace ID
