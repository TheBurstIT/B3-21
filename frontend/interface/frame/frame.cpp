#include "frame.hpp"

const wxEventType IF::CalculatorFrame::kEventUpdate = wxNewEventType();

BEGIN_EVENT_TABLE(IF::CalculatorFrame, wxFrame)
EVT_COMMAND(wxID_ANY, kEventUpdate, IF::CalculatorFrame::UpdateCalc)
END_EVENT_TABLE()

enum WxIds { IdMakeBackUp = 1, IdLoadFromBackUp, IdSaveProgram, IdLoadProgram };

IF::CalculatorFrame::CalculatorFrame(const std::string& title,
                                     std::shared_ptr<CE::Calc> calc)
    : wxFrame(NULL, CE::ButNum2, title, wxDefaultPosition, wxSize(1361, 800)) {
  wxPanel* panel = new wxPanel(this, wxID_ANY);

  wxStaticBitmap* background;

  wxInitAllImageHandlers();
  {
    wxLogNull ignore_warnings;
    wxImage pict1(wxT("images/interface.png"), wxBITMAP_TYPE_PNG);
    wxBitmap backgroundBitmap(pict1);
    background = new wxStaticBitmap(this, wxID_ANY, backgroundBitmap,
                                    wxDefaultPosition, wxSize(1361, 800));
  }

  wxMenuBar* menu_bar = new wxMenuBar();
  wxMenu* backup_menu = new wxMenu();
  wxMenu* program_menu = new wxMenu();

  backup_menu->Append(IdMakeBackUp, _("&Создать резервную копию"));
  backup_menu->Append(IdLoadFromBackUp, _("&Восстановить из резервной копии"));

  program_menu->Append(IdSaveProgram, _("&Сохранить программу"));
  program_menu->Append(IdLoadProgram, _("&Загрузить программу"));

  menu_bar->Append(backup_menu, _("&Резервная копия"));
  menu_bar->Append(program_menu, _("&Программа"));

  SetMenuBar(menu_bar);
  Bind(wxEVT_MENU, &CalculatorFrame::MakeBackUp, this, IdMakeBackUp);
  Bind(wxEVT_MENU, &CalculatorFrame::LoadFromBackUp, this, IdLoadFromBackUp);
  Bind(wxEVT_MENU, &CalculatorFrame::SaveProgram, this, IdSaveProgram);
  Bind(wxEVT_MENU, &CalculatorFrame::LoadProgram, this, IdLoadProgram);

  const wxSize BUTTON_SIZE(39, 36);

  wxButton* button0 = new wxButton(background, CE::ButNum0, "",
                                   wxPoint(133, 638), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button1 = new wxButton(background, CE::ButNum1, "",
                                   wxPoint(133, 586), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button2 = new wxButton(background, CE::ButNum2, "",
                                   wxPoint(182, 586), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button3 = new wxButton(background, CE::ButNum3, "",
                                   wxPoint(232, 586), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button4 = new wxButton(background, CE::ButNum4, "",
                                   wxPoint(133, 533), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button5 = new wxButton(background, CE::ButNum5, "",
                                   wxPoint(182, 533), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button6 = new wxButton(background, CE::ButNum6, "",
                                   wxPoint(232, 533), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button7 = new wxButton(background, CE::ButNum7, "",
                                   wxPoint(133, 480), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button8 = new wxButton(background, CE::ButNum8, "",
                                   wxPoint(182, 480), BUTTON_SIZE, wxNO_BORDER);
  wxButton* button9 = new wxButton(background, CE::ButNum9, "",
                                   wxPoint(232, 480), BUTTON_SIZE, wxNO_BORDER);

  wxButton* buttonPlus = new wxButton(
      background, CE::ButPlus, "", wxPoint(329, 533), BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonMin =
      new wxButton(background, CE::ButMinus, "", wxPoint(329, 586), BUTTON_SIZE,
                   wxNO_BORDER);
  wxButton* buttonMul =
      new wxButton(background, CE::ButMultiply, "", wxPoint(282, 533),
                   BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonDiv =
      new wxButton(background, CE::ButDivision, "", wxPoint(282, 586),
                   BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonCx = new wxButton(
      background, CE::ButCx, "", wxPoint(133, 427), BUTTON_SIZE, wxNO_BORDER);

  wxButton* buttonP = new wxButton(background, CE::ButP, "", wxPoint(182, 427),
                                   BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonNull = new wxButton(
      background, CE::ButNull, "", wxPoint(133, 267), BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonArrowUp =
      new wxButton(background, CE::ButArrowUp, "", wxPoint(329, 479),
                   BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonXArrowY =
      new wxButton(background, CE::ButXArrowY, "", wxPoint(282, 479),
                   BUTTON_SIZE, wxNO_BORDER);

  wxButton* buttonCP = new wxButton(
      background, CE::ButCP, "", wxPoint(329, 374), BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonBvP = new wxButton(
      background, CE::ButBvP, "", wxPoint(282, 638), BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonXExpY =
      new wxButton(background, CE::ButXExpY, "", wxPoint(232, 427), BUTTON_SIZE,
                   wxNO_BORDER);
  wxButton* buttonF = new wxButton(background, CE::ButF, "", wxPoint(329, 638),
                                   BUTTON_SIZE, wxNO_BORDER);

  wxButton* buttonComma =
      new wxButton(background, CE::ButComma, "", wxPoint(182, 638), BUTTON_SIZE,
                   wxNO_BORDER);
  wxButton* buttonBO = new wxButton(
      background, CE::ButBO, "", wxPoint(282, 374), BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonPrefixMinus =
      new wxButton(background, CE::ButPrefixMinus, "", wxPoint(232, 638),
                   BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonBbP = new wxButton(
      background, CE::ButBbP, "", wxPoint(282, 427), BUTTON_SIZE, wxNO_BORDER);

  wxButton* buttonPP = new wxButton(
      background, CE::ButPP, "", wxPoint(329, 427), BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonStepLeft =
      new wxButton(background, CE::ButStepLeft, "", wxPoint(232, 374),
                   BUTTON_SIZE, wxNO_BORDER);
  wxButton* buttonStepRight =
      new wxButton(background, CE::ButStepRight, "", wxPoint(182, 374),
                   BUTTON_SIZE, wxNO_BORDER);

  button0->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button1->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button2->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button3->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button4->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button5->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button6->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button7->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button8->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  button9->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);

  buttonPlus->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonMin->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonMul->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonDiv->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonCx->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);

  buttonP->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonNull->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonArrowUp->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonXArrowY->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);

  buttonCP->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonBvP->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonXExpY->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonF->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);

  buttonComma->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonBO->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonPrefixMinus->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonBbP->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);

  buttonPP->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonStepLeft->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);
  buttonStepRight->Bind(wxEVT_BUTTON, &CalculatorFrame::ButtonClick, this);

  visual_template_.SetPanel(background);
  visualization_ = IV::Visualization(calc, visual_template_);

  calc_ = calc;
}

void IF::CalculatorFrame::UpdateCalc(wxCommandEvent& event) {
  visualization_.UpdateData();
}