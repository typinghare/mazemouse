#ifndef COORD_HPP
#define COORD_HPP

namespace Mazemouse {

struct Vector2 {
    int x;
    int y;
};

inline Vector2 operator+(const Vector2& v1, const Vector2& v2) {
    return { v1.x + v2.x, v1.y + v2.y };
}

inline Vector2 operator-(const Vector2& v1, const Vector2& v2) {
    return { v1.x - v2.x, v1.y - v2.y };
}

inline Vector2 operator*(const int k, const Vector2& vector) {
    return { k * vector.x, k * vector.y };
}

inline bool operator==(const Vector2& v1, const Vector2& v2) {
    return v1.x == v2.x && v1.y == v2.y;
}

}  // namespace Mazemouse

#endif
