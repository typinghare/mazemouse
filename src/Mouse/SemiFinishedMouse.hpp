#ifndef SEMI_FINISHED_MOUSE_HPP
#define SEMI_FINISHED_MOUSE_HPP

#include "AStarMouse.hpp"

namespace Mazemouse {

template <int S>
class SemiFinishedMouse : public AStarMouse<S, FloodFillCell, Edge> {
 public:
    SemiFinishedMouse() :
        AStarMouse<S, FloodFillCell, Edge>({ 0, S - 1 }, Dir4::Up){};
};

}  // namespace Mazemouse

#endif
