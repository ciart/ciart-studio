#pragma once

#include "../types/tool.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include <vector>

namespace Ciart::Studio {
    class PenTool : public ITool {
    public:
        PenTool();
        ~PenTool() override = default;
        
        ToolType getType() const override { return ToolType::Pen; }
        std::string getName() const override { return "Pen"; }
        
        void setSize(float size) override;
        float getSize() const override { return size; }
        void setOpacity(float opacity) override;
        float getOpacity() const override { return opacity; }
        
        void setColor(SkColor color);
        SkColor getColor() const;
        
        void handleEvent(const ToolEvent& event, ILayer* activeLayer, Workspace* workspace) override;
        void renderPreview(SkCanvas* canvas, const Offset& position, const float scale) const override;
        
    private:
        SkPaint paint;
        SkPath currentStroke;
        SkColor color = SK_ColorBLACK;
        
        void startStroke(const Offset& position);
        void continueStroke(const Offset& position);
        void endStroke(ILayer* activeLayer);
        
        void drawPixelBrush(SkCanvas* canvas, const Offset& position, float size, const SkPaint& paint);
        void drawLine(SkCanvas* canvas, const Offset& from, const Offset& to, float size, const SkPaint& paint);
    };
}
