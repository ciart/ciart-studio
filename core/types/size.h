#pragma once

namespace Ciart::Studio {
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
