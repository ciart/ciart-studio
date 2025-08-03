#pragma once

#include <vector>
#include <memory>

#ifdef __APPLE__
#include <swift/bridging>
#endif

#include "size.h"
#include "offset.h"
#include "layer.h"
#include "workspace_context.h"

#define MIN_SCALE 0.5

namespace Ciart::Studio {
    class WorkspaceRenderer;
    
    class Workspace {
    public:
        Workspace();
        
        const Size& getSize() const { return context->getSize(); }
        const Offset& getOffset() const { return context->getOffset(); }
        double getAngle() const { return context->getAngle(); }
        double getScale() const { return context->getScale(); }
        const std::vector<ILayer*>& getLayers() const { return context->getLayers(); }
        
        void setSize(const Size& size) { context->setSize(size); }
        void setOffset(const Offset& offset) { context->setOffset(offset); }
        void setAngle(double angle) { context->setAngle(angle); }
        void setScale(double scale) { context->setScale(scale); }
        void setLayers(const std::vector<ILayer*>& layers) { context->setLayers(layers); }

        void move(double dx, double dy);
        void zoom(double magnification);
        
    
#ifdef _WIN32
        WorkspaceRenderer& createRenderer(void* device, void* commandQueue);
#endif

#ifdef __APPLE__
        WorkspaceRenderer& createRenderer(void* device, void* commandQueue) SWIFT_RETURNS_INDEPENDENT_VALUE;
#endif

    private:
        std::unique_ptr<WorkspaceRenderer> renderer;
        std::shared_ptr<WorkspaceContext> context;
    };
}
