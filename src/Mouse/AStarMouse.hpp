#ifndef A_STAR_MOUSE_HPP
#define A_STAR_MOUSE_HPP

#include "FloodFillMouse.hpp"
#include "Mouse.hpp"

namespace Mazemouse {

template <int S, DerivedFromFloodFillCell C, DerivedFromEdge E>
struct AStarMouse : FloodFillMouse<S, C, E> {};

}  // namespace Mazemouse

#endif
