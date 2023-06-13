#include "backend/backup/backup.hpp"
#include "frame.hpp"

namespace IF {
void CalculatorFrame::ButtonClick(wxCommandEvent& event) {
  if (event.GetId() == CE::ButNull) {
    calc_->TurnOnOff();
  } else {
    calc_->PressButton(static_cast<CE::Button>(event.GetId()));
  }
}

void CalculatorFrame::MakeBackUp(wxCommandEvent& event) {
  wxFileDialog file_dialog(this, _("Make backup"), "", "",
                           "CALC files (*.calc)|*.calc",
                           wxFD_SAVE | wxFD_FILE_MUST_EXIST);
  if (file_dialog.ShowModal() != wxID_CANCEL) {
    try {
      BU::MakeBackup(*calc_, file_dialog.GetPath().ToStdString());
    } catch (std::invalid_argument& argument) {
      wxMessageBox(argument.what(), "File error", wxOK | wxICON_ERROR);
    }
  }
}

void CalculatorFrame::LoadFromBackUp(wxCommandEvent& event) {
  wxFileDialog file_dialog(this, _("Load from backup"), "", "",
                           "CALC files (*.calc)|*.calc",
                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (file_dialog.ShowModal() != wxID_CANCEL) {
    try {
      *calc_ = BU::RestoreFromBackup(file_dialog.GetPath().ToStdString());
    } catch (std::invalid_argument& argument) {
      wxMessageBox(argument.what(), "File error", wxOK | wxICON_ERROR);
    }
  }
}

void CalculatorFrame::SaveProgram(wxCommandEvent& event) {
  wxFileDialog file_dialog(this, _("Load from backup"), "", "",
                           "PROG files (*.prog)|*.prog",
                           wxFD_SAVE | wxFD_FILE_MUST_EXIST);
  if (file_dialog.ShowModal() != wxID_CANCEL) {
    try {
      BU::SaveProgram(calc_->GetProgram().GetProgram(),
                      file_dialog.GetPath().ToStdString());
    } catch (std::invalid_argument& argument) {
      wxMessageBox(argument.what(), "File error", wxOK | wxICON_ERROR);
    }
  }
}

void CalculatorFrame::LoadProgram(wxCommandEvent& event) {
  wxFileDialog file_dialog(this, _("Load from backup"), "", "",
                           "PROG files (*.prog)|*.prog",
                           wxFD_OPEN | wxFD_FILE_MUST_EXIST);
  if (file_dialog.ShowModal() != wxID_CANCEL) {
    try {
      calc_->SetProgram(BU::LoadProgram(file_dialog.GetPath().ToStdString()));
    } catch (std::invalid_argument& argument) {
      wxMessageBox(argument.what(), "File error", wxOK | wxICON_ERROR);
    }
  }
}

}  // namespace IF