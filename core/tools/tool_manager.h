#pragma once

#include "../types/tool.h"
#include "../types/tool_event.h"
#include <memory>
#include <vector>
#include <swift/bridging>

namespace Ciart::Studio {
    class ILayer;
    class Workspace;
    
    class ToolManager {
    public:
        ToolManager();
        ~ToolManager() = default;
        
        // 도구 관리
        void registerTool(ITool* tool);
        void selectTool(ToolType type);
        void selectTool(int index);
        
        // 현재 도구 정보
        ITool* getCurrentTool() const;
        int getCurrentToolIndex() const { return currentToolIndex; }
        ToolType getCurrentToolType() const;
        
        // 도구 설정
        void setToolSize(float size);
        float getToolSize() const;
        void setToolOpacity(float opacity);
        float getToolOpacity() const;
        
        // 이벤트 처리
        void handleToolEvent(const ToolEvent& event, ILayer* activeLayer, Workspace* workspace);
        
        // 미리보기 렌더링
        void renderToolPreview(SkCanvas* canvas, const Offset& position, const float scale) const;
        
        // 도구 목록
        const std::vector<ITool*>& getTools() const { return tools; }
        
    private:
        std::vector<ITool*> tools;
        int currentToolIndex = 0;
        
        void initializeDefaultTools();
    };
}
