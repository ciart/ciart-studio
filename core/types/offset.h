#pragma once

namespace Ciart::Studio {
    struct Offset {
        double dx;
        double dy;

        Offset() : dx(0.0), dy(0.0) {}
        Offset(double dx, double dy) : dx(dx), dy(dy) {}

        Offset operator+(const Offset& other) const {
            return Offset(dx + other.dx, dy + other.dy);
        }

        Offset operator-(const Offset& other) const {
            return Offset(dx - other.dx, dy - other.dy);
        }
    };
}
