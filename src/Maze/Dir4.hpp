#ifndef DIRECTION_HPP
#define DIRECTION_HPP

namespace Mazemouse {

/**
 * Represents four primary directions.
 */
enum class Dir4 : int { Up, Right, Down, Left };

/**
 * @brief Converts a direction to its corresponding vector.
 *
 * This function returns the 2D unit vector representation of the given
 * direction.
 * - Up: {0, -1}
 * - Right: {1, 0}
 * - Down: {0, 1}
 * - Left: {-1, 0}
 *
 * @param dir The direction to be converted.
 * @return A vector representing the direction as a vector.
 */
inline sf::Vector2i get_vector(const Dir4 dir) {
    switch (dir) {
        case Dir4::Up:
            return { 0, -1 };
        case Dir4::Right:
            return { 1, 0 };
        case Dir4::Down:
            return { 0, 1 };
        case Dir4::Left:
            return { -1, 0 };
    }
}

/**
 * @brief Converts a vector to its corresponding direction.
 *
 * This function returns the direction corresponding to the given 2D unit
 * vector.
 * - {0, -1} maps to Up.
 * - {1, 0} maps to Right.
 * - {0, 1} maps to Down.
 * - {-1, 0} maps to Left.
 *
 * @param vector The 2D vector to be converted to a direction.
 * @return A `Dir4` value representing the direction.
 * @throws std::invalid_argument if the vector is not a valid unit vector.
 */
inline Dir4 get_dir(const sf::Vector2i vector) {
    if (vector.x == 0) {
        return vector.y == -1 ? Dir4::Up : Dir4::Down;
    }
    if (vector.y == 0) {
        return vector.x == -1 ? Dir4::Left : Dir4::Right;
    }

    throw std::invalid_argument("Invalid vector");
}

/**
 * @brief Adds two directions and returns the resulting direction modulo 4.
 *
 * This function computes the sum of two directions, treating the directions as
 * integers. The result is wrapped around modulo 4 to ensure that the resulting
 * direction remains within the valid range of the Dir4 enum.
 *
 * @param dir1 The first direction.
 * @param dir2 The second direction.
 * @return The resulting direction after addition.
 */
inline Dir4 operator+(const Dir4 dir1, const Dir4 dir2) {
    return static_cast<Dir4>(
        (static_cast<int>(dir1) + static_cast<int>(dir2)) % 4);
}

/**
 * @brief Subtracts one direction from another and returns the resulting
 * direction modulo 4.
 *
 * This function computes the difference between two directions, treating the
 * directions as integers. The result is wrapped around modulo 4 to ensure that
 * the resulting direction remains within the valid range of the Dir4 enum.
 *
 * @param dir1 The first direction (minuend).
 * @param dir2 The second direction (subtrahend).
 * @return The resulting direction after subtraction.
 */
inline Dir4 operator-(const Dir4 dir1, const Dir4 dir2) {
    return static_cast<Dir4>(
        (static_cast<int>(dir1) - static_cast<int>(dir2) + 4) % 4);
}

}  // namespace Mazemouse

#endif
