#include "tool_manager.h"
#include "pen_tool.h"
#include "eraser_tool.h"
#include "../types/layer.h"
#include "../types/workspace.h"
#include <stdexcept>

namespace Ciart::Studio {
    
    ToolManager::ToolManager() {
        initializeDefaultTools();
    }
    
    void ToolManager::initializeDefaultTools() {
        // 기본 도구들 등록
        registerTool(new PenTool());
        registerTool(new EraserTool());
        
        currentToolIndex = 0; // 기본적으로 펜 도구 선택
    }
    
    void ToolManager::registerTool(ITool* tool) {
        if (tool) {
            tools.push_back(tool);
        }
    }
    
    void ToolManager::selectTool(ToolType type) {
        for (size_t i = 0; i < tools.size(); ++i) {
            if (tools[i]->getType() == type) {
                currentToolIndex = static_cast<int>(i);
                return;
            }
        }
        throw std::runtime_error("Tool type not found");
    }
    
    void ToolManager::selectTool(int index) {
        if (index >= 0 && index < static_cast<int>(tools.size())) {
            currentToolIndex = index;
        } else {
            throw std::out_of_range("Tool index out of range");
        }
    }
    
    ITool* ToolManager::getCurrentTool() const {
        if (currentToolIndex >= 0 && currentToolIndex < static_cast<int>(tools.size())) {
            return tools[currentToolIndex];
        }
        return nullptr;
    }
    
    ToolType ToolManager::getCurrentToolType() const {
        ITool* tool = getCurrentTool();
        return tool ? tool->getType() : ToolType::Pen;
    }
    
    void ToolManager::setToolSize(float size) {
        ITool* tool = getCurrentTool();
        if (tool) {
            tool->setSize(size);
        }
    }
    
    float ToolManager::getToolSize() const {
        ITool* tool = getCurrentTool();
        return tool ? tool->getSize() : 10.0f;
    }
    
    void ToolManager::setToolOpacity(float opacity) {
        ITool* tool = getCurrentTool();
        if (tool) {
            tool->setOpacity(opacity);
        }
    }
    
    float ToolManager::getToolOpacity() const {
        ITool* tool = getCurrentTool();
        return tool ? tool->getOpacity() : 1.0f;
    }
    
    void ToolManager::handleToolEvent(const ToolEvent& event, ILayer* activeLayer, Workspace* workspace) {
        ITool* tool = getCurrentTool();
        if (tool && activeLayer && workspace) {
            tool->handleEvent(event, activeLayer, workspace);
        }
    }
    
    void ToolManager::renderToolPreview(SkCanvas* canvas, const Offset& position, const float scale) const {
        ITool* tool = getCurrentTool();
        if (tool && canvas) {
            tool->renderPreview(canvas, position, scale);
        }
    }
}
