#include <three/math/math.h>

namespace three {

namespace Math {

std::string generateUUID() {

  char chars[63] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

  std::stringstream uuid;

  int rnd = 0, r;

  for (auto i = 0; i < 36; i++) {

    if (i == 8 || i == 13 || i == 18 || i == 23) {

      uuid << "-";

    }
    else if (i == 14) {

      uuid << "4";

    }
    else {

      if (rnd <= 0x02) rnd = 0x2000000 + ( (std::rand() * 0x1000000) | 0);
      r = rnd & 0xf;
      rnd = rnd >> 4;
      uuid << chars[(i == 19) ? (r & 0x3) | 0x8 : r];

    }
  }

  return uuid.str();

};

} // namespace Math

} // namespace three
