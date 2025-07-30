//
// Created by Pdom on 10/9/24.
//

#include "workspace_renderer.h"
#include "types/workspace_context.h"
#include "types/layer.h"
#include "tools/tool_manager.h"

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
    std::unique_ptr<skgpu::graphite::Recorder> recorder = context->makeRecorder();
    if (!recorder) {
        throw std::runtime_error("Could not make recorder");
    }

    sk_sp<SkSurface> surface = createBackendSurface(texture, width, height, recorder.get());
    SkCanvas* canvas = surface->getCanvas();
    canvas->clear(SK_ColorBLACK);

    Offset offset = workspaceContext->getOffset();
    double scale = workspaceContext->getScale();
    Size size = workspaceContext->getSize();
    Offset screen_center = {width / 2, height / 2};
    Offset workspace_center = {size.width / 2, size.height / 2};

    double workspace_offset_x = screen_center.dx - workspace_center.dx * scale + offset.dx;
    double workspace_offset_y = screen_center.dy - workspace_center.dy * scale + offset.dy;

    SkRect workspace_rect = SkRect::MakeXYWH(workspace_offset_x, workspace_offset_y, size.width * scale, size.height * scale);
    renderWorkspaceBackground(canvas, workspace_rect);

    renderLayers(canvas, workspace_rect);
    
    // Tool preview 렌더링
    renderToolPreview(canvas, workspace_rect);

    submitRecording(std::move(recorder));
}

sk_sp<SkSurface> WorkspaceRenderer::createBackendSurface(void* texture, double width, double height, skgpu::graphite::Recorder* recorder) {
    auto backendTexture = skgpu::graphite::BackendTextures::MakeMetal(SkISize::Make(width, height), (CFTypeRef)texture);

    sk_sp<SkSurface> surface = SkSurfaces::WrapBackendTexture(
        recorder, backendTexture, kBGRA_8888_SkColorType,
        SkColorSpace::MakeSRGB(), new SkSurfaceProps());

    if (!surface) {
        throw std::runtime_error("Could not make surface from Metal Recorder");
    }

    return surface;
}

void WorkspaceRenderer::renderWorkspaceBackground(SkCanvas* canvas, const SkRect& workspace_rect) {
    SkPaint bg_paint;
    bg_paint.setColor(SK_ColorWHITE);
    bg_paint.setAntiAlias(true);
    bg_paint.setStyle(SkPaint::kFill_Style);
    canvas->drawRect(workspace_rect, bg_paint);
}

void WorkspaceRenderer::renderLayers(SkCanvas* canvas, const SkRect& dest_rect) {
    Size workspace_size = workspaceContext->getSize();
    sk_sp<SkSurface> layer_surface = canvas->getSurface()->makeSurface(workspace_size.width, workspace_size.height);

    if (!layer_surface) {
        throw std::runtime_error("Could not create layer surface");
    }

    SkCanvas* layer_canvas = layer_surface->getCanvas();
    layer_canvas->clear(SK_ColorTRANSPARENT);

    const auto& layers = workspaceContext->getLayers();
    for (auto* layer : layers) {
        layer->render(layer_canvas);
    }

    sk_sp<SkImage> layer_image = layer_surface->makeImageSnapshot();
    canvas->drawImageRect(layer_image, dest_rect, SkSamplingOptions());
}

void WorkspaceRenderer::renderToolPreview(SkCanvas* canvas, const SkRect& workspace_rect) {
    if (!workspaceContext->hasMousePosition()) {
        return;
    }
    
    auto toolManager = workspaceContext->getToolManager();
    if (!toolManager) {
        return;
    }
    
    const Offset& mousePos = workspaceContext->getMousePosition();
    
    // 워크스페이스 좌표를 화면 좌표로 변환
    double scale = workspaceContext->getScale();
    double screenX = workspace_rect.fLeft + mousePos.dx * scale;
    double screenY = workspace_rect.fTop + mousePos.dy * scale;
    
    Offset screenPos = {screenX, screenY};
    toolManager->renderToolPreview(canvas, screenPos, scale);
}

void WorkspaceRenderer::submitRecording(std::unique_ptr<skgpu::graphite::Recorder> recorder) {
    std::unique_ptr<skgpu::graphite::Recording> recording = recorder->snap();
    if (!recording) {
        throw std::runtime_error("Could not create a recording");
    }

    skgpu::graphite::InsertRecordingInfo info;
    info.fRecording = recording.get();
    if (!context->insertRecording(info)) {
        throw std::runtime_error("Context::insertRecording failed");
    }

    context->submit(skgpu::graphite::SyncToCpu::kYes);
    if (context->hasUnfinishedGpuWork()) {
        throw std::runtime_error("Sync with GPU completion failed");
    }
}
