#include "layer.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPath.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkImage.h"

using namespace Ciart::Studio;

BitmapLayer::BitmapLayer(std::string name, int width, int height) 
    : width(width), height(height) {
    this->name = name;
    
    // SkBitmap 초기화
    SkImageInfo imageInfo = SkImageInfo::Make(width, height, kBGRA_8888_SkColorType, kPremul_SkAlphaType, SkColorSpace::MakeSRGB());
    bitmap.allocPixels(imageInfo);
    bitmap.eraseColor(SK_ColorTRANSPARENT);
    
    drawTestContent();
}

void BitmapLayer::drawTestContent() {
    if (bitmap.isNull()) return;
    
    SkCanvas canvas(bitmap);
    canvas.clear(SK_ColorTRANSPARENT);
    
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    
    // 그라데이션 배경
    paint.setColor(SkColorSetRGB(100, 150, 200));
    canvas.drawRect(SkRect::MakeWH(width, height), paint);
    
    // 테스트 도형들
    paint.setColor(SkColorSetRGB(255, 100, 100));
    canvas.drawCircle(width * 0.3f, height * 0.3f, 50, paint);
    
    paint.setColor(SkColorSetRGB(100, 255, 100));
    canvas.drawRect(SkRect::MakeXYWH(width * 0.5f, height * 0.2f, 80, 60), paint);
    
    paint.setColor(SkColorSetRGB(100, 100, 255));
    SkPath path;
    path.moveTo(width * 0.2f, height * 0.7f);
    path.lineTo(width * 0.4f, height * 0.5f);
    path.lineTo(width * 0.6f, height * 0.8f);
    path.close();
    canvas.drawPath(path, paint);
    
    // 텍스트 영역 표시
    paint.setColor(SkColorSetRGB(255, 255, 255));
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(2);
    canvas.drawRect(SkRect::MakeXYWH(10, 10, width - 20, height - 20), paint);
    
    isInitialized = true;
}

void BitmapLayer::render(SkCanvas* canvas) {
    if (bitmap.isNull() || !isInitialized) return;

    canvas->writePixels(bitmap, 0, 0);
}
