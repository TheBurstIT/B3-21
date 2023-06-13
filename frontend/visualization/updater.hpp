#pragma once

#include <wx/stattext.h>
#include <wx/wx.h>

#include <memory>
#include <string>

#include "backend/calc_exec/calc_exec.hpp"
#include "frontend/interface/definition/interface_def.hpp"
#include "frontend/interface/frame/frame.hpp"

namespace IU {
// функция, следящая за изменениями в калькуляторе (отдельный поток)
void Updater(std::weak_ptr<CE::Calc> calc, wxFrame* window);
}  // namespace IU
