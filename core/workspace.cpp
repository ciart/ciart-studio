#include "workspace.h"

using namespace Ciart::Studio;

void Workspace::move(double dx, double dy) {
    offset.dx += dx;
    offset.dy += dy;
}

void Workspace::room(double magnification) {
    scale += magnification;
}
