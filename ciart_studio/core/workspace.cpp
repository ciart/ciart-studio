//
// Created by Pdom on 10/9/24.
//

#include "workspace.h"

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
#include "include/gpu/graphite/mtl/MtlGraphiteUtils.h"

#define WIDTH 200
#define HEIGHT 400

using namespace Ciart::Studio;

skgpu::graphite::MtlBackendContext GetMetalContext();

Workspace::Workspace(int x) {
    skgpu::graphite::MtlBackendContext backendContext = GetMetalContext();
    skgpu::graphite::ContextOptions options;

    std::unique_ptr<skgpu::graphite::Context> context =
        skgpu::graphite::ContextFactory::MakeMetal(backendContext, options);
    if (!context) {
        throw std::runtime_error("Could not make Graphite Native Metal context");
    }
    printf("Context made, now to make the surface\n");

    SkImageInfo imageInfo =
            SkImageInfo::Make(WIDTH, HEIGHT, kRGBA_8888_SkColorType, kPremul_SkAlphaType);

    std::unique_ptr<skgpu::graphite::Recorder> recorder = context->makeRecorder();
    if (!recorder) {
        throw std::runtime_error("Could not make recorder");
    }
    sk_sp<SkSurface> surface =
            SkSurfaces::RenderTarget(recorder.get(), imageInfo);
    if (!surface) {
        throw std::runtime_error("Could not make surface from Metal Recorder");
    }

    SkCanvas* canvas = surface->getCanvas();
    canvas->clear(SK_ColorCYAN);
    SkRRect rrect = SkRRect::MakeRectXY(SkRect::MakeLTRB(10, 20, 50, 70), 10, 10);

    SkPaint paint;
    paint.setColor(SK_ColorYELLOW);
    paint.setAntiAlias(true);

    canvas->drawRRect(rrect, paint);

    printf("ready to snap the GPU calls\n");
    // Now to send the draws to the GPU
    std::unique_ptr<skgpu::graphite::Recording> recording = recorder->snap();
    if (!recording) {
        throw std::runtime_error("Could not create a recording");
    }
    skgpu::graphite::InsertRecordingInfo info;
    info.fRecording = recording.get();
    if (!context->insertRecording(info)) {
        throw std::runtime_error("Context::insertRecording failed");
    }

    sk_sp<SkImage> img;
    auto callback = [](SkImage::ReadPixelsContext ctx,
                       std::unique_ptr<const SkImage::AsyncReadResult> result) {
        if (result->count() != 1) {
            printf("Didn't load exactly one plane\n");
            return;
        }
        auto ii = SkImageInfo::Make(WIDTH, HEIGHT, kRGBA_8888_SkColorType, kPremul_SkAlphaType);
        sk_sp<SkImage>* output = reinterpret_cast<sk_sp<SkImage>*>(ctx);

        SkPixmap pm(ii, result->data(0), result->rowBytes(0));
        *output = SkImages::RasterFromPixmapCopy(pm);
    };
    printf("GPU operations modifying surface have been inserted in the command buffer, scheduling pixel readback\n");
    context->asyncRescaleAndReadPixels(surface.get(), imageInfo, SkIRect::MakeSize({WIDTH, HEIGHT}),
                                       SkImage::RescaleGamma::kSrc,
                                       SkImage::RescaleMode::kRepeatedCubic,
                                       callback, &img);

    printf("Submitting work to GPU and waiting for it to be done\n");
    // Note this doesn't work on all backend types, e.g. Dawn.
    context->submit(skgpu::graphite::SyncToCpu::kYes);
    if (context->hasUnfinishedGpuWork()) {
        throw std::runtime_error("Sync with GPU completion failed");
    }

    // sk_sp<SkData> jpeg = SkJpegEncoder::Encode(nullptr, img.get(), {});
    // if (!jpeg) {
    //     throw std::runtime_error("Encoding failed");
    // }
}

int Workspace::print() { return this->x; }
