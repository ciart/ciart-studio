//
// Created by Pdom on 10/9/24.
//
#pragma once

#include "include/gpu/graphite/Context.h"
#include "include/gpu/graphite/Recorder.h"

#ifdef __APPLE__
#include "include/gpu/graphite/mtl/MtlGraphiteUtils.h"
#elif defined(_WIN32)
#include "include/gpu/graphite/vk/VulkanGraphiteTypes.h"
#include "include/gpu/graphite/vk/VulkanGraphiteUtils.h"
#endif

namespace Ciart::Studio {
    class WorkspaceContext;
    class WorkspaceRenderer {
    public:
        WorkspaceRenderer(std::shared_ptr<WorkspaceContext> context, void* device, void* commandQueue);
        void draw(void* texture, double width, double height);

    private:
        std::shared_ptr<WorkspaceContext> workspaceContext;
        std::unique_ptr<skgpu::graphite::Context> context = nullptr;
        
        void initializeContext(void* device, void* commandQueue);
        sk_sp<SkSurface> createSurface(std::unique_ptr<skgpu::graphite::Recorder>& recorder, int width, int height);
        sk_sp<SkSurface> createBackendSurface(void* texture, double width, double height, skgpu::graphite::Recorder* recorder);
        void renderWorkspaceBackground(SkCanvas* canvas, const SkRect& workspace_rect);
        void renderLayers(SkCanvas* canvas, const SkRect& dest_rect);
        void renderToolPreview(SkCanvas* canvas, const SkRect& workspace_rect);
        void submitRecording(std::unique_ptr<skgpu::graphite::Recorder> recorder);
        
#if defined(__APPLE__)
        void initializeMetalContext(void* device, void* commandQueue);
        sk_sp<SkSurface> createMetalBackendSurface(void* texture, double width, double height, skgpu::graphite::Recorder* recorder);
#elif defined(_WIN32)
        void initializeVulkanContext(void* hwnd);
        sk_sp<SkSurface> createVulkanBackendSurface(void* texture, double width, double height, skgpu::graphite::Recorder* recorder);
        
        VkInstance vulkanInstance = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkDevice vulkanDevice = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        uint32_t graphicsQueueIndex = 0;
#endif
    };
}
