//
// Created by Pdom on 10/9/24.
//
#pragma once

#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/ContextOptions.h"
#include "include/gpu/graphite/GraphiteTypes.h"
#include "include/gpu/graphite/Recorder.h"
#include "include/gpu/graphite/Surface.h"
#include "include/gpu/graphite/mtl/MtlBackendContext.h"
#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkRRect.h"
#include "include/core/SkRect.h"
#include "include/core/SkSize.h"
#include "include/core/SkStream.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColorSpace.h"
#include "include/gpu/graphite/mtl/MtlGraphiteUtils.h"
#include "include/gpu/graphite/mtl/MtlGraphiteTypes_cpp.h"

namespace Ciart {
    namespace Studio {
        class Workspace {
        public:
            Workspace(void* device, void* commandQueue);
            void draw(void* texture, double width, double height);
            void resize(double width, double height);
            std::unique_ptr<skgpu::graphite::Context> context;
        };
    }
}
