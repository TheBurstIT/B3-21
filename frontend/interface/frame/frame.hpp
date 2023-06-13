#pragma once

#include <wx/stattext.h>
#include <wx/wx.h>

#include <memory>
#include <vector>

#include "backend/calc_exec/calc_exec.hpp"
#include "frontend/interface/definition/interface_def.hpp"
#include "frontend/visualization/visualization.hpp"

namespace IF {

class CalculatorFrame : public wxFrame {
 public:
  CalculatorFrame(const std::string& title, std::shared_ptr<CE::Calc> calc);
  ~CalculatorFrame() = default;

  static const wxEventType kEventUpdate;
  void UpdateCalc(wxCommandEvent& event);


 private:
  std::shared_ptr<CE::Calc> calc_;
  ID::VisualisationTemplate visual_template_;
  IV::Visualization visualization_;

  // Обработчики событий для кнопок
  void ButtonClick(wxCommandEvent& event);

  void MakeBackUp(wxCommandEvent& event);
  void LoadFromBackUp(wxCommandEvent& event);

  void SaveProgram(wxCommandEvent& event);
  void LoadProgram(wxCommandEvent& event);

  DECLARE_EVENT_TABLE();
};

}  // namespace IF