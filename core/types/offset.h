#pragma once

namespace Ciart::Studio {
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
}
