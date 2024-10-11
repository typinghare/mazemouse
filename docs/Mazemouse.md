# Mazemouse

## Mouse

To extend the `SemiFinishedMouse` and implement the `MouseHardwareInterface`, refer to the `CompletedMouse` class as an example for guidance.

In the `Mouse` class, there is an important `state` attribute that dictates the behavior of the mouse. This `state` can take on one of the following values:

- `MouseState::Stopped`: The mouse is no longer moving.
- `MouseState::Exploring`: The mouse is actively exploring the maze, searching for the goal area.
- `MouseState::ReturningToStart`: The mouse navigates back to the starting cell.
- `MouseState::RushingToFinish`: The mouse moves quickly towards the goal area after exploration is complete.
- 
The Mouse class also includes two key methods: `nextExploringCycle()` and `nextRushingCycle()`. These methods should be invoked according to the mouse's current state:

- `nextExploringCycle()` should be called when the mouse is in either the Exploring or ReturningToStart states.
- `nextRushingCycle()` should be called when the mouse is in the RushingToFinish state.
Below is an example of how to handle the state transitions and invoke the appropriate methods:

```c++
int main() {
    mouse.state = MouseState::Exploring;
    while (mouse.state != MouseState.Stopped) {
        if (mouse.state == MouseState.Exploring || mouse.state == ReturningToStart) {
            mouse.nextExploringCycle();
        } else if (mouse.state == MouseState.RushingToFinish) {
            mouse.nextRushingCycle();
        }
    }
}
```

Once the mouse has finished exploring the maze, the following method can be used to reset the mouse to prepare it for a rush to the goal:

```c++
mouse.resetRushingState();
```

After calling `resetRushingState()`, the mouse will be repositioned at the starting cell but will retain its memory of the maze. From this state, the mouse will begin rushing towards the goal area, starting fresh from the beginning.