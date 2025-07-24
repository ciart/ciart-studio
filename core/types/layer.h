//
//  layer.h
//  CiartStudio
//
//  Created by Pdom on 6/29/25.
//

#pragma once

#include <string>

#include "include/core/SkCanvas.h"
#include "include/core/SkBitmap.h"

namespace Ciart::Studio {
    class ILayer {
    public:
        std::string name;
        virtual ~ILayer() = default;
        virtual void render(SkCanvas* canvas) = 0;
    };

    class BitmapLayer : public ILayer {
    public:
        BitmapLayer(std::string name, int width = 512, int height = 512);
        ~BitmapLayer() override = default;
        
        void render(SkCanvas* canvas) override;
        void drawTestContent();
        
        const SkBitmap& getBitmap() const { return bitmap; }
        SkBitmap& getBitmap() { return bitmap; }
        
    private:
        int width, height;
        SkBitmap bitmap;
        bool isInitialized = false;
    };
}
