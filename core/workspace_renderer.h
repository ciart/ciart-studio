//
// Created by Pdom on 10/9/24.
//
#pragma once

#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/Recorder.h"
#include "include/gpu/graphite/mtl/MtlGraphiteUtils.h"

#include "types/workspace.h"

namespace Ciart::Studio {
    class WorkspaceRenderer {
    public:
        WorkspaceRenderer(void* device, void* commandQueue);
        void draw(Workspace& workspace, void* texture, double width, double height);

    private:
        std::unique_ptr<skgpu::graphite::Context> context = nullptr;
    };
}
