#include "interface_def.hpp"

namespace ID {

TextBlockTable::TextBlockTable(ID::TableParameters parameters,
                               ID::TextBlock template_text_block) {
  table.resize(parameters.column_num * parameters.raw_num);

  for (int32_t i = 0; i < parameters.column_num; ++i) {
    for (int32_t j = 0; j < parameters.raw_num; ++j) {
      table[i * parameters.raw_num + j].location = {
          parameters.init_x +
              (static_cast<double>(i) * parameters.column_offset),
          parameters.init_y + (static_cast<double>(j) * parameters.raw_offset)};
      table[i * parameters.raw_num + j].font = template_text_block.font;
    }
  }
}

VisualisationTemplate::VisualisationTemplate() {
  const double kRawOffset = 29.65;
  const uint32_t kNumOfPrevOperations = 3;

  wxFontInfo font_info;
  font_info.FaceName("JetBrains Mono");

  wxColour col_tables_main(0, 102, 24);
  wxColour col_tables_add(128, 0, 0);
  wxColour col_screen(0, 204, 48);

  //--------STEP-----------+
  {
    wxFont font(font_info);
    font.SetPointSize(10);

    step.font.first = {font, col_screen};
    step.location = {130, 170};
  }

  //----------MAIN_NUMBER---------
  {
    wxFont font_num(font_info);
    font_num.SetPointSize(26);
    main_number.number.font.first = {font_num, col_screen};
    main_number.number.location = {170, 180};

    wxFont font_char(font_info);
    font_char.SetPointSize(10);
    main_number.characteristic.font.first = {font_char, col_screen};
    main_number.characteristic.location = {130, 195};
  };

  //-------------LAST_OPERATIONS-------------
  {
    wxFont font(font_info);
    font.SetPointSize(11);

    TextBlock text_block;
    text_block.font.first = {font, col_screen};

    const int kLORawOffset = 13;
    TableParameters parameters = {
        360, 178, kLORawOffset, 0, kNumOfPrevOperations, 1};

    last_operations = TextBlockTable(parameters, text_block);
  }

  //-------------MODE-------------
  {
    wxFont font(font_info);
    font.SetPointSize(kDefaultTextSize);
    font.MakeBold();

    mode.location = {544, 98};
    mode.font.first = {font, col_tables_main};
  }

  //------------FUNCTION_BUTTON------------
  {
    wxFont font(font_info);
    font.SetPointSize(kDefaultTextSize);
    font.MakeBold();

    function_button.location = {640, 70};
    function_button.font.first = {font, col_tables_main};
  }

  wxFont font(font_info);
  font.SetPointSize(kDefaultTextSize);

  TextBlock text_block;
  text_block.font.first = {font, col_tables_main};
  text_block.font.second = {font, col_tables_add};
  text_block.font.first.font.MakeBold();
  text_block.font.second.font.MakeBold();
  //---------PROGRAM------------
  {
    TableParameters table_parameters = {552, 134, kRawOffset, 145, 20, 3};
    program = TextBlockTable(table_parameters, text_block);
  }
  //--------------NUMERATED_BUFFER------------
  {
    TableParameters table_parameters_number = {1135, 135, kRawOffset, 0, 8, 1};
    numerated_buffer.number =
        TextBlockTable(table_parameters_number, text_block);

    TableParameters table_parameters_characteristic = {1055, 135, kRawOffset,
                                                       0,    8,   1};
    numerated_buffer.characteristic =
        TextBlockTable(table_parameters_characteristic, text_block);
  }
  //---------------ROUNDED_BUFFER-----------
  {
    TableParameters table_parameters_number = {
        1135, 400, kRawOffset, 0, CM::kRoundedBuffSize, 1};
    rounded_buffer.number = TextBlockTable(table_parameters_number, text_block);

    TableParameters table_parameters_characteristic = {
        1055, 400, kRawOffset, 0, CM::kRoundedBuffSize, 1};
    rounded_buffer.characteristic =
        TextBlockTable(table_parameters_characteristic, text_block);
  }
}

void TextBlockTable::SetPanel(wxWindow* panel) {
  for (auto& elem : table) {
    elem.panel = panel;
  }
}
void VisualisationTemplate::SetPanel(wxWindow* panel) {
  step.panel = panel;

  main_number.number.panel = panel;
  main_number.characteristic.panel = panel;

  last_operations.SetPanel(panel);
  mode.panel = panel;
  function_button.panel = panel;
  program.SetPanel(panel);

  numerated_buffer.number.SetPanel(panel);
  numerated_buffer.characteristic.SetPanel(panel);

  rounded_buffer.number.SetPanel(panel);
  rounded_buffer.characteristic.SetPanel(panel);
}
};  // namespace ID
