#include "workspace.h"
#include "layer.h"
#include "workspace_context.h"
#include "../workspace_renderer.h"

using namespace Ciart::Studio;

Workspace::Workspace() {
    this->context = std::make_shared<WorkspaceContext>();
    std::vector<ILayer*> initialLayers;
    initialLayers.push_back(new BitmapLayer("Layer 1", this->context->getSize().width, this->context->getSize().height));
    this->context->setLayers(initialLayers);
}

WorkspaceRenderer& Workspace::createRenderer(void* device, void* commandQueue) {
    renderer = std::make_unique<WorkspaceRenderer>(context, device, commandQueue);
    return *renderer;
}


void Workspace::move(double dx, double dy) {
    Offset currentOffset = context->getOffset();
    currentOffset.dx += dx;
    currentOffset.dy += dy;
    context->setOffset(currentOffset);
}

void Workspace::zoom(double magnification) {
    double currentScale = context->getScale();
    currentScale += magnification;

    if (currentScale < MIN_SCALE) {
        currentScale = MIN_SCALE;
    }
    context->setScale(currentScale);
}
