//
// Created by Pdom on 10/9/24.
//

#include "workspace.h"

using namespace Ciart::Studio;

Workspace::Workspace(void* device, void* commandQueue) {
    skgpu::graphite::MtlBackendContext backendContext = {};
    skgpu::graphite::ContextOptions options;

    backendContext.fDevice = (sk_cfp<CFTypeRef>)device;
    backendContext.fQueue = (sk_cfp<CFTypeRef>)commandQueue;

    context =
        skgpu::graphite::ContextFactory::MakeMetal(backendContext, options);
    if (!context) {
        throw std::runtime_error("Could not make Graphite Native Metal context");
    }
}

void Workspace::draw(void* texture, double width, double height) {
    auto backendTexture = skgpu::graphite::BackendTextures::MakeMetal(SkISize::Make(width, height), (CFTypeRef)texture);

    std::unique_ptr<skgpu::graphite::Recorder> recorder = context->makeRecorder();
    if (!recorder) {
        throw std::runtime_error("Could not make recorder");
    }

    sk_sp<SkSurface> surface =
            SkSurfaces::WrapBackendTexture(recorder.get(), backendTexture, kBGRA_8888_SkColorType, SkColorSpace::MakeSRGB(), new SkSurfaceProps());
    if (!surface) {
        throw std::runtime_error("Could not make surface from Metal Recorder");
    }

    SkCanvas* canvas = surface->getCanvas();
    canvas->clear(SK_ColorBLACK);
    SkRect rect = SkRect::MakeLTRB(100, 100, width - 100, height - 100);

    SkPaint paint;
    paint.setColor(SK_ColorGREEN);
    paint.setAntiAlias(true);
    paint.setStyle(SkPaint::kFill_Style);
    paint.setStrokeWidth(5);

    canvas->drawRect(rect, paint);

    // printf("ready to snap the GPU calls\n");
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

    // printf("Submitting work to GPU and waiting for it to be done\n");
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

void Workspace::resize(double width, double height) {
    // TODO: Implement resize logic
}

