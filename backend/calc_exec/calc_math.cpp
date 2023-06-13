#include "calc_math.hpp"

namespace Math {

CN::Number Pow(CN::Number num, CN::Number power) {
  if (power == 0) {
    return 1;
  }
  auto res = num;
  auto number = num;
  for (CN::Number i = 0; i < power - 1; i = i + CN::Number(1)) {
    res *= number;
  }
  return res;
}

CN::Number Factorial(CN::Number n) {
  if (n == 0 || n == 1) {
    return CN::Number(1);
  }
  CN::Number result = 1;
  for (CN::Number i = 2; i <= n; i = i + CN::Number(1)) {
    result *= i;
  }
  return result;
}

CN::Number Exp(CN::Number x) {
  CN::Number result = 1;
  CN::Number term;
  int i = 1;
  CN::Number epsilon = 1e-8;

  do {
    auto term1 = Pow(x, i);
    auto term2 = Factorial(i);
    term = term1 / term2;
    result += term;
    ++i;
  } while (term > epsilon);

  return result;
}

CN::Number Sin(CN::Number x) {
  CN::Number result = x;
  CN::Number term;
  int i = 3;
  CN::Number epsilon = 1e-6;
  CN::Number one = 1;
  do {
    one = one * CN::Number(-1);
    term = one * (Pow(x, i) / Factorial(i));
    result += term;
    i += 2;
  } while ((term > 0 ? term : CN::Number(-1) * term) > epsilon);
  return result;
}

CN::Number Cos(CN::Number x) {
  CN::Number result = 0.000001;
  CN::Number term;
  CN::Number i = 2;
  CN::Number epsilon = 1e-8;
  CN::Number one = -1;

  do {
    one = one * CN::Number(-1);
    term = one * (Pow(x, i) / Factorial(i));
    result += term;
    i += CN::Number(2);
  } while ((term > 0 ? term : CN::Number(-1) * term) > epsilon);

  return CN::Number(1) - result;
}

CN::Number LnLimited(CN::Number x) {
  CN::Number ln2 = (0.693147181);
  if (x != 1) {
    CN::Number epsilon = 1e-8;
    CN::Number result = 0;
    CN::Number prev_result;
    CN::Number x_power = x;
    int i = 1;

    do {
      prev_result = result;
      CN::Number term = CN::Number(i % 2 == 1 ? 1 : -1) * Pow(x, i) / i;
      result += term;
      ++i;
    } while (((result - prev_result) < 0
                  ? CN::Number(-1) * (result - prev_result)
                  : result - prev_result) >= epsilon);

    return result;
  }
  return ln2;
}

CN::Number Ln(CN::Number x) {
  CN::Number result;
  if (x <= 1 && x > -1) {
    return LnLimited(x);
  }
  CN::Number y = x / 2;
  int count = 1;
  while (y > 2) {
    y = y / 2;
    ++count;
  }
  return LnLimited(y - 1) + (CN::Number(count) * LnLimited(1));
}

CN::Number Root(CN::Number x) {
  if (x == 0) {
    return 0;
  }
  CN::Number epsilon = 1e-8;
  CN::Number y = x;
  CN::Number y_next;
  do {
    y_next = y - (y * y - x) / (CN::Number(2) * y);
    if (((y - y_next) < 0 ? CN::Number(-1) * (y - y_next) : (y - y_next)) <
        epsilon) {
      break;
    }
    y = y_next;
  } while (true);

  {
    CN::Number integer_part = y_next.GetIntegerPart();
    if (integer_part * integer_part == x) {
      y_next = integer_part;
    }
  }

  return y_next;
}

}