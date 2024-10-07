#ifndef EDGE_HPP
#define EDGE_HPP

namespace Mazemouse {

struct Edge {
    mutable bool hasWall{ true };
    mutable bool visited{ false };
};

}  // namespace Mazemouse

#endif
