//
// Created by Pdom on 10/9/24.
//
#pragma once

#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/Recorder.h"
#include "include/gpu/graphite/mtl/MtlGraphiteUtils.h"

namespace Ciart::Studio {
    class WorkspaceContext;
    class WorkspaceRenderer {
    public:
        WorkspaceRenderer(std::shared_ptr<WorkspaceContext> context, void* device, void* commandQueue);
        void draw(void* texture, double width, double height);

    private:
        std::shared_ptr<WorkspaceContext> workspaceContext;
        std::unique_ptr<skgpu::graphite::Context> context = nullptr;
    };
}
