#pragma once

#include "../types/tool.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"

namespace Ciart::Studio {
    class EraserTool : public ITool {
    public:
        EraserTool();
        ~EraserTool() override = default;
        
        ToolType getType() const override { return ToolType::Eraser; }
        std::string getName() const override { return "Eraser"; }
        
        void setSize(float size) override;
        float getSize() const override { return size; }
        void setOpacity(float opacity) override;
        float getOpacity() const override { return opacity; }
        
        void handleEvent(const ToolEvent& event, ILayer* activeLayer, Workspace* workspace) override;
        void renderPreview(SkCanvas* canvas, const Offset& position, const float scale) const override;
        
    private:
        SkPaint erasePaint;
        SkPath currentStroke;
        
        void startErase(const Offset& position);
        void continueErase(const Offset& position);
        void endErase(ILayer* activeLayer);
        
        void erasePixelBrush(SkCanvas* canvas, const Offset& position, float size);
    };
}
