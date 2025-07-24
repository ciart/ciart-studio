//
// Created by Pdom on 10/9/24.
//

#include "workspace_renderer.h"
#include "types/workspace_context.h"

#include "include/core/SkBitmap.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkImage.h"
#include "include/core/SkRect.h"
#include "include/core/SkSize.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColorSpace.h"
#include "include/gpu/graphite/ContextOptions.h"
#include "include/gpu/graphite/GraphiteTypes.h"
#include "include/gpu/graphite/Surface.h"
#include "include/gpu/graphite/mtl/MtlBackendContext.h"
#include "include/gpu/graphite/mtl/MtlGraphiteTypes_cpp.h"

#include "types/offset.h"
#include "types/size.h"

using namespace Ciart::Studio;

WorkspaceRenderer::WorkspaceRenderer(std::shared_ptr<WorkspaceContext> workspaceContext, void* device, void* commandQueue)
    : workspaceContext(workspaceContext) {
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

void WorkspaceRenderer::draw(void* texture, double width, double height) {
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

    Offset offset = workspaceContext->getOffset();
    double scale = workspaceContext->getScale() * workspaceContext->getScale();
    Size size = workspaceContext->getSize();
    Offset screen_center = {width / 2, height / 2};
    Offset workspace_center = {size.width / 2, size.height / 2};

    SkRect rect = SkRect::MakeXYWH( screen_center.dx - workspace_center.dx * scale + offset.dx, screen_center.dy - workspace_center.dy * scale + offset.dy, size.width * scale, size.height * scale);

    SkPaint paint;
    paint.setColor(SK_ColorWHITE);
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
