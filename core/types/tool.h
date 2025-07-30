#pragma once

#include <string>
#include <swift/bridging>
#include "tool_event.h"
#include "include/core/SkCanvas.h"

namespace Ciart::Studio {
    class ILayer;
    class Workspace;
    
    enum class ToolType {
        Pen,
        Eraser
    };
    
    class ITool {
    public:
        virtual ~ITool() = default;
        
        virtual ToolType getType() const = 0;
        virtual std::string getName() const = 0;
        
        // 도구 설정
        virtual void setSize(float size) = 0;
        virtual float getSize() const = 0;
        virtual void setOpacity(float opacity) = 0;
        virtual float getOpacity() const = 0;
        
        // 도구 이벤트 처리
        virtual void handleEvent(const ToolEvent& event, ILayer* activeLayer, Workspace* workspace) = 0;
        
        // 미리보기 렌더링 (실시간 커서 등)
        virtual void renderPreview(SkCanvas* canvas, const Offset& position, const float scale) const {}
        
    protected:
        float size = 10.0f;
        float opacity = 1.0f;
        bool isDrawing = false;
        Offset lastPosition;
    };
}
