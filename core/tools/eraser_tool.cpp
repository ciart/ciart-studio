#include "eraser_tool.h"
#include "../types/layer.h"
#include "../types/workspace.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkBlendMode.h"
#include "include/core/SkPathEffect.h"
#include "include/effects/SkDashPathEffect.h"
#include <cmath>

namespace Ciart::Studio {
    
    EraserTool::EraserTool() {
        erasePaint.setAntiAlias(true);
        erasePaint.setColor(SK_ColorTRANSPARENT);
        erasePaint.setBlendMode(SkBlendMode::kClear); // 픽셀을 지우는 블렌드 모드
        erasePaint.setStyle(SkPaint::kFill_Style);
        size = 10.0f;
        opacity = 1.0f;
    }
    
    void EraserTool::setSize(float newSize) {
        size = std::max(1.0f, std::min(100.0f, newSize)); // 1-100 픽셀 범위로 제한
    }
    
    void EraserTool::setOpacity(float newOpacity) {
        opacity = std::max(0.0f, std::min(1.0f, newOpacity));
        // 지우개의 opacity는 alpha 값으로 조절
        U8CPU alpha = static_cast<U8CPU>(255 * (1.0f - opacity));
        erasePaint.setAlpha(alpha);
    }
    
    void EraserTool::handleEvent(const ToolEvent& event, ILayer* activeLayer, Workspace* workspace) {
        if (!activeLayer) return;
        
        switch (event.type) {
            case ToolEventType::Press:
                startErase(event.position);
                break;
                
            case ToolEventType::Move:
                if (isDrawing) {
                    continueErase(event.position);
                }
                break;
                
            case ToolEventType::Release:
                if (isDrawing) {
                    endErase(activeLayer);
                }
                break;
        }
    }
    
    void EraserTool::startErase(const Offset& position) {
        isDrawing = true;
        lastPosition = position;
        currentStroke.reset();
        currentStroke.moveTo(position.dx, position.dy);
    }
    
    void EraserTool::continueErase(const Offset& position) {
        if (!isDrawing) return;
        
        // 부드러운 지우개 스트로크를 위한 quadTo 사용
        float midX = (lastPosition.dx + position.dx) / 2.0f;
        float midY = (lastPosition.dy + position.dy) / 2.0f;
        
        currentStroke.quadTo(lastPosition.dx, lastPosition.dy, midX, midY);
        lastPosition = position;
    }
    
    void EraserTool::endErase(ILayer* activeLayer) {
        if (!isDrawing) return;
        
        isDrawing = false;
        
        // BitmapLayer에서 픽셀 지우기
        BitmapLayer* bitmapLayer = dynamic_cast<BitmapLayer*>(activeLayer);
        if (bitmapLayer) {
            SkBitmap& bitmap = bitmapLayer->getBitmap();
            SkCanvas canvas(bitmap);
            
            // 지우개 스타일 설정
            SkPaint strokePaint = erasePaint;
            strokePaint.setStyle(SkPaint::kStroke_Style);
            strokePaint.setStrokeWidth(size);
            strokePaint.setStrokeCap(SkPaint::kRound_Cap);
            strokePaint.setStrokeJoin(SkPaint::kRound_Join);
            strokePaint.setBlendMode(SkBlendMode::kClear);
            
            // 경로 지우기
            canvas.drawPath(currentStroke, strokePaint);
            
            // 단일 점인 경우 원형 영역 지우기
            if (currentStroke.countPoints() <= 1) {
                erasePixelBrush(&canvas, lastPosition, size);
            }
        }
        
        currentStroke.reset();
    }
    
    void EraserTool::erasePixelBrush(SkCanvas* canvas, const Offset& position, float brushSize) {
        // 픽셀 단위 원형 지우개
        SkPaint clearPaint;
        clearPaint.setAntiAlias(true);
        clearPaint.setBlendMode(SkBlendMode::kClear);
        clearPaint.setStyle(SkPaint::kFill_Style);
        
        float radius = brushSize / 2.0f;
        canvas->drawCircle(position.dx, position.dy, radius, clearPaint);
    }
    
    void EraserTool::renderPreview(SkCanvas* canvas, const Offset& position, const float scale) const {
        // 지우개 커서 미리보기 (점선 원)
        SkPaint previewPaint;
        previewPaint.setColor(SkColorSetA(SK_ColorRED, 150));
        previewPaint.setStyle(SkPaint::kStroke_Style);
        previewPaint.setStrokeWidth(2.0f);
        previewPaint.setAntiAlias(true);
        
        // 점선 효과
        float intervals[] = {4.0f, 4.0f};
        previewPaint.setPathEffect(SkDashPathEffect::Make(intervals, 2));
        
        float radius = size / 2.0f;
        canvas->drawCircle(position.dx, position.dy, radius, previewPaint);
    }
}
