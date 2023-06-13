#include "updater.hpp"

namespace IU {
void Updater(std::weak_ptr<CE::Calc> calc, wxFrame* window) {
  auto queue_checker = calc.lock()->GetDataUpdateMarker();
  while (true) {
    auto received_msg = queue_checker.Receive(1, MQ::Wait);
    if (received_msg.value() == CE::Error) {
      wxMessageBox("An error occurred while using the calculator",
                   "EXECUTING ERROR", wxOK | wxICON_ERROR);
    } else if (received_msg.value() == CE::UpdateData) {
      wxCommandEvent update_event(IF::CalculatorFrame::kEventUpdate, wxID_ANY);
      wxPostEvent(window, update_event);
    } else {
      break;
    }
  }
}

}  // namespace IU