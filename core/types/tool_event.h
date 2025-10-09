#pragma once

#include "offset.h"

namespace Ciart::Studio {
    enum class ToolEventType {
        Press,
        Move, 
        Release
    };
    
    struct ToolEvent {
        ToolEventType type;
        Offset position;
        float pressure = 1.0f;
        
        ToolEvent() : type(ToolEventType::Press), position(), pressure(1.0f) {}
        ToolEvent(ToolEventType eventType, const Offset& pos, float p = 1.0f)
            : type(eventType), position(pos), pressure(p) {}
    };
}
