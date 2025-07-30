#include "workspace.h"
#include "layer.h"
#include "workspace_context.h"
#include "../workspace_renderer.h"
#include "../tools/tool_manager.h"

using namespace Ciart::Studio;

Workspace::Workspace() {
    this->context = std::make_shared<WorkspaceContext>();
    std::vector<ILayer*> initialLayers;
    initialLayers.push_back(new BitmapLayer("Layer 1", this->context->getSize().width, this->context->getSize().height));
    this->context->setLayers(initialLayers);
    
    // ToolManager 초기화
    this->toolManager = std::make_unique<ToolManager>();
    this->context->setToolManager(std::shared_ptr<ToolManager>(toolManager.get(), [](ToolManager*){})); // non-owning shared_ptr
    this->activeLayerIndex = 0;
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

void Workspace::setActiveLayerIndex(int index) {
    const auto& layers = context->getLayers();
    if (index >= 0 && index < static_cast<int>(layers.size())) {
        activeLayerIndex = index;
    }
}

ILayer* Workspace::getActiveLayer() const {
    const auto& layers = context->getLayers();
    if (activeLayerIndex >= 0 && activeLayerIndex < static_cast<int>(layers.size())) {
        return layers[activeLayerIndex];
    }
    return nullptr;
}

void Workspace::handleToolEvent(const ToolEvent& event) {
    if (toolManager) {
        ILayer* activeLayer = getActiveLayer();
        toolManager->handleToolEvent(event, activeLayer, this);
    }
}

