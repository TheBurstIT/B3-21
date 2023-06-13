#include "backend/number/number.hpp"

namespace Math {

const CN::Number kPi = (3.14159265359);

CN::Number Pow(CN::Number num, CN::Number power);
CN::Number Factorial(CN::Number n);
CN::Number Exp(CN::Number x);
CN::Number Sin(CN::Number x);
CN::Number Cos(CN::Number x);
CN::Number LnLimited(CN::Number x);
CN::Number Ln(CN::Number x);
CN::Number Root(CN::Number x);

}  // namespace Math