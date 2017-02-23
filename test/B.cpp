#include "B.hpp"

B::B()
    : s(static_cast<signed char>(std::rand())),
      t(static_cast<unsigned char>(std::rand())), u(std::rand()),
      v(std::rand()), w((float)std::rand() / std::rand()),
      x((double)std::rand() / std::rand())
{
}

bool B::operator==(const B& rhs) const
{
    return A::operator==(rhs) && s == rhs.s && t == rhs.t && u == rhs.u &&
           v == rhs.v && std::abs(boost::math::float_distance(w, rhs.w)) < 6 &&
           std::abs(boost::math::float_distance(x, rhs.x)) < 6;
}
