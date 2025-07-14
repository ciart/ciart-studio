#include "workspace.h"
#include "layer.h"

using namespace Ciart::Studio;

Workspace::Workspace() {
    this->layers.push_back(new BitmapLayer("Layer 1"));
}


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
