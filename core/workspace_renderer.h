//
// Created by Pdom on 10/9/24.
//
#pragma once

#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/Recorder.h"
#include "include/gpu/graphite/mtl/MtlGraphiteUtils.h"

#include "workspace.h"

namespace Ciart {
    namespace Studio {
        class WorkspaceRenderer {
        public:
            WorkspaceRenderer(void* device, void* commandQueue);
            void draw(void* texture, double width, double height);
            void resize(double width, double height);
            void setWorkspace(Workspace* workspace);

            std::unique_ptr<skgpu::graphite::Context> context;
        private:
            Workspace* workspace;
        };
    }
}
