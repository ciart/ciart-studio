#pragma once

namespace Ciart {
    namespace Studio {
        struct Offset {
            double dx;
            double dy;

            Offset(double dx, double dy) : dx(dx), dy(dy) {}

            Offset operator+(const Offset& other) const {
                return Offset(dx + other.dx, dy + other.dy);
            }

            Offset operator-(const Offset& other) const {
                return Offset(dx - other.dx, dy - other.dy);
            }
        };

        struct Size {
            double width;
            double height;

            Size(double width, double height) : width(width), height(height) {}

            Size operator+(const Size& other) const {
                return Size(width + other.width, height + other.height);
            }

            Size operator-(const Size& other) const {
                return Size(width - other.width, height - other.height);
            }

            Size operator*(double scale) const {
                return Size(width * scale, height * scale);
            }

            Size operator/(double scale) const {
                return Size(width / scale, height / scale);
            }
        };
    }
}
