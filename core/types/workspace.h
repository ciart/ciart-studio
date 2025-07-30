#pragma once

#include <vector>
#include <memory>

#include <swift/bridging>

#include "size.h"
#include "offset.h"
#include "layer.h"
#include "workspace_context.h"
#include "tool_event.h"
#include "tool.h"

#define MIN_SCALE 0.5

namespace Ciart::Studio {
    class WorkspaceRenderer;
    class ToolManager;
    
    class Workspace {
    public:
        Workspace();
        
        const Size getSize() const { return context->getSize(); }
        const Offset getOffset() const { return context->getOffset(); }
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
        
        // 활성 레이어 관리
        void setActiveLayerIndex(int index);
        int getActiveLayerIndex() const { return activeLayerIndex; }
        ILayer* getActiveLayer() const;
        
        // 도구 관리
        ToolManager& getToolManager() SWIFT_RETURNS_INDEPENDENT_VALUE { return *toolManager; }
        const ToolManager& getToolManager() const SWIFT_RETURNS_INDEPENDENT_VALUE { return *toolManager; }
        
        // 도구 이벤트 처리
        void handleToolEvent(const ToolEvent& event);
        
        // 마우스 위치 관리 (context에 위임)
        void setMousePosition(const Offset& position) { context->setMousePosition(position); }
        void clearMousePosition() { context->clearMousePosition(); }
        const Offset& getMousePosition() const { return context->getMousePosition(); }
        bool hasMousePosition() const { return context->hasMousePosition(); }
        
        WorkspaceRenderer& createRenderer(void* device, void* commandQueue) SWIFT_RETURNS_INDEPENDENT_VALUE;
        
    private:
        std::unique_ptr<WorkspaceRenderer> renderer;
        std::shared_ptr<WorkspaceContext> context;
        std::unique_ptr<ToolManager> toolManager;
        int activeLayerIndex = 0;
    };
}
