#include "workspace.h"

using namespace Ciart::Studio;

void Workspace::move(double dx, double dy) {
    offset.dx += dx;
    offset.dy += dy;
}

void Workspace::zoom(double magnification) {
    scale += magnification;

    if (scale < MIN_SCALE) {
        scale = MIN_SCALE;
    }
}
