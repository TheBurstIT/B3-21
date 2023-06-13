#include "number.hpp"

#include <vector>

namespace CN {

Number::Number(const Number& other) {
  other.ExitEnterMode();

  sign_ = other.sign_;
  number_ = other.number_;
  characteristic_ = other.characteristic_;
}

Number::Number(Number&& other)
    : sign_(std::move(other.sign_)),
      number_(std::move(other.number_)),
      characteristic_(std::move(other.characteristic_)) {
  RepairNumber();
}

Number::Number(int number)
    : number_(abs(number)),
      sign_(number < 0),
      characteristic_(std::to_string(abs(number)).size() - 1) {
  RepairNumber();
}

Number::Number(double number) : sign_(number < 0) {
  if (number == 0) {
    number_ = 0;
    return;
  }

  if (number < 0) {
    number *= -1;
  }

  std::vector<char> tmp_str_num(100, 0);
  sprintf(tmp_str_num.data(), "%.15f", number);
  std::string str_num = tmp_str_num.data();

  auto begin = str_num.begin();
  if (abs(number) >= 1) {
    characteristic_ = str_num.find('.') - 1;
  } else {
    characteristic_ =
        -(static_cast<int>(str_num.find_first_not_of('0', 2)) - 1);
    begin += str_num.find_first_not_of('0', 2);
  }

  for (auto iter = begin; iter < str_num.end(); ++iter) {
    if (*iter == '.') {
      continue;
    }
    AddDigit(number_, *iter - '0');
  }

  RepairNumber();
}

Number& Number::operator=(const Number& other) {
  other.ExitEnterMode();

  sign_ = other.sign_;
  number_ = other.number_;
  characteristic_ = other.characteristic_;

  new_characteristic_ = 0;
  mode_ = Mantissa;
  return *this;
}

Number& Number::operator=(CN::Number&& other) {
  sign_ = std::move(other.sign_);
  number_ = std::move(other.number_);
  characteristic_ = std::move(other.characteristic_);

  new_characteristic_ = 0;
  mode_ = Mantissa;

  RepairNumber();
  return *this;
}

/*---------------------------- для визуализации ------------------------------*/
std::tuple<int, std::string> Number::GetStaticNumber() const noexcept {
  // может ли число быть представлено полностью
  auto [sign, characteristic, number] = IsFullView() ? FullView() : PartView();

  if (sign) {
    number.insert(number.begin(), '-');
  }

  if (new_characteristic_ != 0) {
    characteristic = new_characteristic_;
  }
  if (number.back() == '.') {
    number.pop_back();
  }
  return {characteristic, number};
}

std::tuple<int, std::string> Number::GetMainNumber() const noexcept {
  auto [sign, characteristic, number] = GetNumberPrivate();
  if (sign) {
    number.insert(number.begin(), '-');
  } else {
    number.insert(number.begin(), ' ');
  }
  return {characteristic, number};
}

/*------------------------ интерфейс взаимодействия --------------------------*/
void Number::SignButton() noexcept {
  if (mode_ == Characteristic) {
    new_characteristic_ *= -1;
  } else if (number_ != 0) {
    sign_ = !sign_;
  }
}

void Number::DotButton() noexcept {
  auto curr_num = GetNumberPrivate();
  // проверка наличия требуемого режима, полноты представления и возможности
  // использования кнопки в дальнейшем
  if (mode_ == Mantissa && std::get<1>(curr_num) == 0 &&
      std::get<2>(curr_num).size() + 1 < kNumOfDigits) {
    if (std::get<2>(curr_num) == "0") {
      characteristic_ -= 1;
    }
    mode_ = AfterDot;
  }
}

void Number::CharacteristicButton() {
  if (mode_ == Characteristic) {
    ExitEnterMode();
    return;
  }
  RepairNumber();
  if (!IsFullView()) {
    new_characteristic_ = characteristic_;
  }
  mode_ = Characteristic;
}

void Number::NumberButton(char digit) {
  auto curr_number = GetNumberPrivate();
  if (mode_ == Characteristic) {
    if (std::get<2>(curr_number).size() < kNumOfCharacteristic) {
      AddDigit(new_characteristic_, digit);
    }
  } else if (std::get<2>(curr_number).size() < kNumOfDigits) {
    // проверка полноты представления
    if (std::get<1>(curr_number) == 0) {
      if (digit != 0 || mode_ == AfterDot) {
        // дополнение числа нулями до соответствия характеристике
        while (number_ != 0 &&
               characteristic_ -
                       static_cast<int>(std::to_string(number_).size()) + 1 >
                   0) {
          AddDigit(number_, 0);
        }

        AddDigit(number_, digit);
      }
      if (number_ == 0 && mode_ == AfterDot) {
        characteristic_ -= 1;
      }

      // увеличение характеристики, если это произошло
      if (std::get<2>(curr_number) != "0" &&
          !IsThereDot(std::get<2>(curr_number))) {
        characteristic_ += 1;
      }
    } else {
      AddDigit(number_, digit);
    }
  }
}

void Number::ClearButton() noexcept { *this = Number(); }

/*--------------------------- операторы сравнения ----------------------------*/
bool Number::operator<(Number other) const {
  Number this_num = *this;
  if (this_num.sign_ != other.sign_) {
    return this_num.sign_;
  }

  if (this_num == 0 || other == 0) {
    return this_num == 0 && other != 0;
  }

  auto compare = this_num.sign_ ? std::cmp_greater<int64_t, int64_t>
                                : std::cmp_less<int64_t, int64_t>;
  if (this_num.characteristic_ != other.characteristic_) {
    return compare(this_num.characteristic_, other.characteristic_);
  }
  ToEqualDigits(this_num.number_, other.number_);
  return compare(this_num.number_, other.number_);
}

bool Number::operator<=(Number other) const {
  Number this_num = *this;
  if (this_num.sign_ != other.sign_) {
    return this_num.sign_;
  }

  if (this_num == 0 || other == 0) {
    return this_num == 0;
  }

  auto compare = this_num.sign_ ? std::cmp_greater_equal<int64_t, int64_t>
                                : std::cmp_less_equal<int64_t, int64_t>;
  if (this_num.characteristic_ != other.characteristic_) {
    return compare(this_num.characteristic_, other.characteristic_);
  }
  ToEqualDigits(this_num.number_, other.number_);
  return compare(this_num.number_, other.number_);
}

bool Number::operator>(Number other) const {
  Number this_num = *this;
  if (this_num.sign_ != other.sign_) {
    return other.sign_;
  }

  if (this_num == 0 || other == 0) {
    return other == 0 && this_num != 0;
  }

  auto compare = this_num.sign_ ? std::cmp_less<int64_t, int64_t>
                                : std::cmp_greater<int64_t, int64_t>;
  if (this_num.characteristic_ != other.characteristic_) {
    return compare(this_num.characteristic_, other.characteristic_);
  }
  ToEqualDigits(this_num.number_, other.number_);
  return compare(this_num.number_, other.number_);
}

bool Number::operator>=(Number other) const {
  Number this_num = *this;
  if (this_num.sign_ != other.sign_) {
    return other.sign_;
  }

  if (this_num == 0 || other == 0) {
    return other == 0;
  }

  auto compare = this_num.sign_ ? std::cmp_less_equal<int64_t, int64_t>
                                : std::cmp_greater_equal<int64_t, int64_t>;
  if (this_num.characteristic_ != other.characteristic_) {
    return compare(this_num.characteristic_, other.characteristic_);
  }
  ToEqualDigits(this_num.number_, other.number_);
  return compare(this_num.number_, other.number_);
}

bool Number::operator==(Number other) const {
  Number this_num = *this;
  return (this_num.sign_ == other.sign_) &&
         (this_num.characteristic_ == other.characteristic_) &&
         (this_num.number_ == other.number_);
}

bool Number::operator!=(Number other) const {
  Number this_num = *this;
  return this_num.sign_ != other.sign_ ||
         this_num.characteristic_ != other.characteristic_ ||
         this_num.number_ != other.number_;
}

/*------------------------- арифметические операции --------------------------*/
Number Number::operator-() const {
  Number result = *this;
  result.sign_ = !sign_;
  return result;
}

Number& Number::operator+=(const Number& other) {
  ExitEnterMode();

  if (abs(characteristic_ - other.characteristic_) >= kNumOfDigits) {
    return *this = Number::Abs(*this) > Number::Abs(other) ? *this : other;
  }

  Number min = Number::Abs(*this) < Number::Abs(other) ? *this : other;
  Number max = Number::Abs(*this) >= Number::Abs(other) ? *this : other;

  Number::ToEqualDigits(max, min);

  Number result;
  result.sign_ = max.sign_;
  result.number_ =
      max.number_ + (static_cast<int>(min.number_) *
                     (-1 + (2 * static_cast<int>(max.sign_ == min.sign_))));
  result.characteristic_ = max.characteristic_ +
                           std::to_string(result.number_).size() -
                           std::to_string(max.number_).size();
  IsOverflow(result);

  return *this = result;
}
Number Number::operator+(const Number& other) const {
  Number result = *this;
  return result += other;
}

Number& Number::operator-=(const Number& other) {
  ExitEnterMode();

  if (abs(characteristic_ - other.characteristic_) >= kNumOfDigits) {
    return *this = Number::Abs(*this) > Number::Abs(other) ? *this : other;
  }

  return operator+=(-other);
}
Number Number::operator-(const Number& other) const {
  Number result = *this;
  return result -= other;
}

Number& Number::operator*=(Number other) {
  ExitEnterMode();

  Number result;
  result.sign_ = sign_ ^ other.sign_;
  result.number_ = number_ * other.number_;
  result.characteristic_ = 1 + std::to_string(result.number_).size() -
                           std::to_string(number_).size() -
                           std::to_string(other.number_).size() +
                           characteristic_ + other.characteristic_;

  IsOverflow(result);

  return *this = result;
}
Number Number::operator*(const Number& other) const {
  Number result = *this;
  return result *= other;
}

Number& Number::operator/=(Number other) {
  ExitEnterMode();

  if (other == 0) {
    throw std::invalid_argument("dividing by zero");
  }

  Number tmp_this = *this;

  ToEqualDigits(tmp_this.number_, other.number_);

  Number result = static_cast<double>(tmp_this.number_) /
                  static_cast<double>(other.number_);
  if (result == 0) {
    return *this = 0;
  }

  result.characteristic_ += characteristic_ - other.characteristic_;
  IsOverflow(result);

  return *this = result;
}
Number Number::operator/(const Number& other) const {
  Number result = *this;
  return result /= other;
}

Number& Number::operator%=(Number other) {
  return *this = *this - ((*this / other) * other);
}
Number Number::operator%(const Number& other) const {
  Number number = *this;
  return number %= other;
}

Number Number::GetIntegerPart() const {
  ExitEnterMode();

  if (characteristic_ < 0 || number_ == 0) {
    return 0;
  }

  auto [sign, characteristic, number] = GetNumberPrivate();
  if (characteristic > 0) {
    return *this;
  }

  Number result = *this;

  int dot = number.find('.') + 1;
  for (int i = 0; i < number.size() - dot - 1; ++i) {
    result.number_ /= 10;
  }
  return result;
}

/*-------------------------- for backup / restore ----------------------------*/
Number::Number(bool sign, uint64_t number, int characteristic,
               int new_characteristic, EnterMode mode) noexcept
    : sign_(sign),
      number_(number),
      characteristic_(characteristic),
      new_characteristic_(new_characteristic),
      mode_(mode) {}

Number::BackUpIng Number::GetClass() const noexcept {
  return {sign_, number_, characteristic_, new_characteristic_, mode_};
}

/*---------------------------- приватные методы ------------------------------*/
void Number::ExitEnterMode() const noexcept {
  if (mode_ == Characteristic) {
    if (IsFullView()) {
      characteristic_ += new_characteristic_;
    } else {
      characteristic_ = new_characteristic_;
    }
    new_characteristic_ = 0;
  }
  RepairNumber();

  mode_ = Mantissa;
}

void Number::RepairNumber() const noexcept {
  auto old_mode = mode_;
  mode_ = Mantissa;
  auto curr_number = GetNumberPrivate();

  // удаление чисел, не помещающихся на экран
  while (std::get<2>(curr_number).size() > kNumOfDigits) {
    if (!std::get<2>(curr_number).ends_with(".")) {
      number_ /= 10;
    }
    std::get<2>(curr_number).pop_back();
  }

  // удаление лишних нулей
  if (IsThereDot(std::get<2>(curr_number))) {
    while (*(std::get<2>(curr_number).end() - 1) == '0') {
      std::get<2>(curr_number).pop_back();
      number_ /= 10;
    }
  }

  if (number_ == 0) {
    sign_ = false;
    characteristic_ = 0;
  }

  mode_ = old_mode;
}

template <typename T>
void Number::AddDigit(T& number, char digit) noexcept {
  number *= 10;
  number += digit;
}

std::tuple<bool, int, std::string> Number::FullView() const noexcept {
  auto str_number = std::to_string(number_);

  if (characteristic_ >= 0) {
    // дополнение нулями для соответствия характеристике
    while (characteristic_ - static_cast<int>(str_number.size()) + 1 > 0) {
      str_number.push_back('0');
    }
    if (str_number.size() > characteristic_ + 1) {
      str_number.insert(str_number.begin() + characteristic_ + 1, '.');
    }
  } else {
    // добавление префиксных нулей
    for (int i = 0; i < abs(characteristic_) - static_cast<int>(number_ == 0);
         ++i) {
      str_number.insert(str_number.begin() + i, '0');
    }
    str_number.insert(str_number.begin() + 1, '.');
  }

  return {sign_, 0, str_number};
}

std::tuple<bool, int, std::string> Number::PartView() const noexcept {
  auto str_number = std::to_string(number_);
  auto dot_point = str_number.begin() + 1;
  if (dot_point != str_number.cend()) {
    str_number.insert(dot_point, '.');
  }

  return {sign_, characteristic_, str_number};
}

Number Number::Abs(Number number) noexcept {
  return number >= 0 ? number : -number;
}

std::tuple<bool, int, std::string> Number::GetNumberPrivate() const noexcept {
  if (mode_ == Characteristic) {
    return {new_characteristic_ < 0, 0,
            std::to_string(abs(new_characteristic_))};
  }

  // может ли число быть представлено полностью
  auto curr_num = IsFullView() ? FullView() : PartView();

  // Вставка точки в конец после нажатия клавиши "."
  if (mode_ == AfterDot && !IsThereDot(std::get<2>(curr_num))) {
    std::get<2>(curr_num).insert(std::get<2>(curr_num).end(), '.');
  }

  return curr_num;
}

/*--------------------------- проверочные методы -----------------------------*/
bool Number::IsThereDot(const std::string& string) noexcept {
  return std::find(string.begin(), string.end(), '.') != string.end();
}

bool Number::IsFullView() const noexcept {
  if (number_ == 0) {
    return true;
  }
  return characteristic_ + 1 <= kNumOfDigits &&
         -characteristic_ + 1 < kNumOfDigits;
}

void Number::IsOverflow(CN::Number& number) {
  if (std::to_string(number.characteristic_).size() > kNumOfCharacteristic) {
    if (number.characteristic_ < 0) {
      number = 0;
    } else {
      throw std::bad_array_new_length();
    }
  }
}

void Number::ToEqualDigits(CN::Number& abs_max, CN::Number& abs_min) {
  int dist = static_cast<int>(std::to_string(abs_min.number_).size()) -
             static_cast<int>(std::to_string(abs_max.number_).size()) +
             abs_max.characteristic_ - abs_min.characteristic_;

  if (dist > 0) {
    for (int i = 0; i < dist; ++i) {
      abs_max.number_ *= 10;
    }
  } else if (dist < 0) {
    for (int i = 0; i < -dist; ++i) {
      abs_min.number_ *= 10;
    }
  }
}

void Number::ToEqualDigits(uint64_t& first, uint64_t& second) {
  std::string str_first = std::to_string(first);
  std::string str_second = std::to_string(second);

  uint64_t& min = str_first.size() > str_second.size() ? second : first;
  int min_size = std::min(str_first.size(), str_second.size());
  int max_size = std::max(str_first.size(), str_second.size());

  for (int i = min_size; i < max_size; ++i) {
    AddDigit(min, 0);
  }
}

}  // namespace CN