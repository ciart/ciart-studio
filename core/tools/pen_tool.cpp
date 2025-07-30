#include "pen_tool.h"
#include "../types/layer.h"
#include "../types/workspace.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkBitmap.h"
#include <cmath>

namespace Ciart::Studio {
    
    PenTool::PenTool() {
        paint.setAntiAlias(true);
        paint.setColor(SK_ColorWHITE);
        paint.setStyle(SkPaint::kFill_Style);
        size = 5.0f;
        opacity = 1.0f;
        color = SK_ColorWHITE;
    }
    
    void PenTool::setSize(float newSize) {
        size = std::max(1.0f, std::min(100.0f, newSize)); // 1-100 픽셀 범위로 제한
    }
    
    void PenTool::setOpacity(float newOpacity) {
        opacity = std::max(0.0f, std::min(1.0f, newOpacity));
        SkColor colorWithAlpha = SkColorSetA(color, static_cast<U8CPU>(255 * opacity));
        paint.setColor(colorWithAlpha);
    }
    
    void PenTool::setColor(SkColor newColor) {
        color = newColor;
        SkColor colorWithAlpha = SkColorSetA(color, static_cast<U8CPU>(255 * opacity));
        paint.setColor(colorWithAlpha);
    }
    
    SkColor PenTool::getColor() const {
        return color;
    }
    
    void PenTool::handleEvent(const ToolEvent& event, ILayer* activeLayer, Workspace* workspace) {
        if (!activeLayer) return;
        
        switch (event.type) {
            case ToolEventType::Press:
                startStroke(event.position);
                break;
                
            case ToolEventType::Move:
                if (isDrawing) {
                    continueStroke(event.position);
                }
                break;
                
            case ToolEventType::Release:
                if (isDrawing) {
                    endStroke(activeLayer);
                }
                break;
        }
    }
    
    void PenTool::startStroke(const Offset& position) {
        isDrawing = true;
        lastPosition = position;
        currentStroke.reset();
        currentStroke.moveTo(position.dx, position.dy);
    }
    
    void PenTool::continueStroke(const Offset& position) {
        if (!isDrawing) return;
        
        // 부드러운 라인을 위한 quadTo 사용
        float midX = (lastPosition.dx + position.dx) / 2.0f;
        float midY = (lastPosition.dy + position.dy) / 2.0f;
        
        currentStroke.quadTo(lastPosition.dx, lastPosition.dy, midX, midY);
        lastPosition = position;
    }
    
    void PenTool::endStroke(ILayer* activeLayer) {
        if (!isDrawing) return;
        
        isDrawing = false;
        
        // BitmapLayer에 스트로크 그리기
        BitmapLayer* bitmapLayer = dynamic_cast<BitmapLayer*>(activeLayer);
        if (bitmapLayer) {
            SkBitmap& bitmap = bitmapLayer->getBitmap();
            SkCanvas canvas(bitmap);
            
            // 픽셀 단위 브러시 스타일 설정
            SkPaint strokePaint = paint;
            strokePaint.setStyle(SkPaint::kStroke_Style);
            strokePaint.setStrokeWidth(size);
            strokePaint.setStrokeCap(SkPaint::kRound_Cap);
            strokePaint.setStrokeJoin(SkPaint::kRound_Join);
            
            // 경로 그리기
            canvas.drawPath(currentStroke, strokePaint);
            
            // 단일 점인 경우 원 그리기
            if (currentStroke.countPoints() <= 1) {
                drawPixelBrush(&canvas, lastPosition, size, paint);
            }
        }
        
        currentStroke.reset();
    }
    
    void PenTool::drawPixelBrush(SkCanvas* canvas, const Offset& position, float brushSize, const SkPaint& brushPaint) {
        // 픽셀 단위 원형 브러시
        SkPaint fillPaint = brushPaint;
        fillPaint.setStyle(SkPaint::kFill_Style);
        
        float radius = brushSize / 2.0f;
        canvas->drawCircle(position.dx, position.dy, radius, fillPaint);
    }
    
    void PenTool::drawLine(SkCanvas* canvas, const Offset& from, const Offset& to, float brushSize, const SkPaint& brushPaint) {
        // 두 점 사이의 픽셀 단위 라인 그리기
        SkPaint strokePaint = brushPaint;
        strokePaint.setStyle(SkPaint::kStroke_Style);
        strokePaint.setStrokeWidth(brushSize);
        strokePaint.setStrokeCap(SkPaint::kRound_Cap);
        
        canvas->drawLine(from.dx, from.dy, to.dx, to.dy, strokePaint);
    }
    
    void PenTool::renderPreview(SkCanvas* canvas, const Offset& position, const float scale) const {
        // 커서 미리보기 (반투명 원)
        SkPaint previewPaint;
        previewPaint.setColor(SkColorSetA(SK_ColorBLACK, 100));
        previewPaint.setStyle(SkPaint::kStroke_Style);
        previewPaint.setStrokeWidth(1.0f);
        previewPaint.setAntiAlias(true);
        
        float radius = size / 2.0f * scale;
        canvas->drawCircle(position.dx, position.dy, radius, previewPaint);
    }
}
