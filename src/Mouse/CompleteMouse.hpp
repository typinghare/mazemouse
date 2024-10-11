#ifndef COMPLETE_MOUSE_HPP
#define COMPLETE_MOUSE_HPP
#include "SemiFinishedMouse.hpp"

namespace Mazemouse {

auto constexpr MAZE_LENGTH_SIDE = 16;

class CompleteMouse final : public SemiFinishedMouse<MAZE_LENGTH_SIDE> {
 public:
    bool hardwareCheckWall(Dir4 dir) override;

    void hardwareMoveForward(int step) override;

    void hardwareTurn(Dir4 relative_dir) override;
};

inline bool CompleteMouse::hardwareCheckWall(Dir4 dir) { return true; }

inline void CompleteMouse::hardwareMoveForward(int step) {}

inline void CompleteMouse::hardwareTurn(Dir4 relative_dir) {}

}  // namespace Mazemouse

#endif
