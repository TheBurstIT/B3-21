#include <wx/wx.h>

#include <memory>

#include "backend/calc_exec/calc_exec.hpp"
#include "frontend/interface/frame/frame.hpp"
#include "frontend/visualization/updater.hpp"

class Updater : public wxThread {
 public:
  Updater(std::weak_ptr<CE::Calc> calc, wxFrame* frame)
      : wxThread(wxTHREAD_DETACHED),
        calc_(calc),
        frame_(frame) {}

  virtual void* Entry() {
    IU::Updater(calc_, frame_);
    return nullptr;
  }

 private:
  std::weak_ptr<CE::Calc> calc_;
  wxFrame* frame_;
};

class CalculatorApp : public wxApp {
 public:
  virtual bool OnInit() {  // Создание главного окна калькулятора
    std::shared_ptr<CE::Calc> calc(new CE::Calc());

    IF::CalculatorFrame* frame = new IF::CalculatorFrame("Calculator", calc);
    frame->Show(true);

    Updater* updater = new Updater(calc, frame);
    updater->Run();
    return true;
  }
};

wxIMPLEMENT_APP(CalculatorApp);