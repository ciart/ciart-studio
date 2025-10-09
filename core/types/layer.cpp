#include "layer.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkPaint.h"
#include "include/core/SkImageInfo.h"
#include "include/core/SkImage.h"

using namespace Ciart::Studio;

BitmapLayer::BitmapLayer(std::string name, int width, int height) 
    : width(width), height(height) {
    this->name = name;
    
    initializeBitmap();
    drawTestContent();
}

void BitmapLayer::drawTestContent() {
    if (bitmap.isNull()) return;
    
    SkCanvas canvas(bitmap);
    canvas.clear(SK_ColorTRANSPARENT);
    
    drawBackground(&canvas);
    drawBorder(&canvas);
    
    isInitialized = true;
}

void BitmapLayer::render(SkCanvas* canvas) {
    if (bitmap.isNull() || !isInitialized) return;

    canvas->writePixels(bitmap, 0, 0);
}

void BitmapLayer::initializeBitmap() {
    SkImageInfo imageInfo = SkImageInfo::Make(width, height, kBGRA_8888_SkColorType, kPremul_SkAlphaType, SkColorSpace::MakeSRGB());
    bitmap.allocPixels(imageInfo);
    bitmap.eraseColor(SK_ColorTRANSPARENT);
}

void BitmapLayer::drawBackground(SkCanvas* canvas) {
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(SkColorSetRGB(100, 150, 200));
    canvas->drawRect(SkRect::MakeWH(width, height), paint);
}

void BitmapLayer::drawBorder(SkCanvas* canvas) {
    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SkColorSetRGB(255, 255, 255));
    paint.setStyle(SkPaint::kStroke_Style);
    paint.setStrokeWidth(2);
    canvas->drawRect(SkRect::MakeXYWH(10, 10, width - 20, height - 20), paint);
}
