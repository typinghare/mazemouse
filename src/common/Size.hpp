#ifndef SIZE_HPP
#define SIZE_HPP

namespace Mazemouse {

class Size {
    unsigned short width_;
    unsigned short height_;

 public:
    Size(unsigned width, unsigned height);

    [[nodiscard]] unsigned width() const;

    [[nodiscard]] unsigned height() const;
};

inline Size::Size(const unsigned width, const unsigned height) : width_(width), height_(height) {}

inline unsigned Size::width() const { return width_; }

inline unsigned Size::height() const { return height_; }

}  // namespace Mazemouse

#endif
