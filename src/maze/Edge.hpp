#ifndef EDGE_HPP
#define EDGE_HPP

namespace Mazemouse {

class Edge {
    bool existed_{ false };

 public:
    [[nodiscard]] bool existed() const;

    void setExisted();
};

inline bool Edge::existed() const { return existed_; }

inline void Edge::setExisted() { existed_ = true; }

}  // namespace Mazemouse

#endif
