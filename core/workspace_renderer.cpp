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

#ifdef __APPLE__
#include "include/gpu/graphite/mtl/MtlBackendContext.h"
#include "include/gpu/graphite/mtl/MtlGraphiteTypes_cpp.h"
#elif defined(_WIN32)
#include "include/gpu/graphite/vk/VulkanGraphiteContext.h"
#include "include/gpu/graphite/vk/VulkanGraphiteTypes.h"
#include "include/gpu/vk/VulkanBackendContext.h"
#include <vulkan/vulkan.h>
#include <windows.h>
#include <vector>
#endif

#include "types/offset.h"
#include "types/size.h"

using namespace Ciart::Studio;

WorkspaceRenderer::WorkspaceRenderer(std::shared_ptr<WorkspaceContext> workspaceContext, void* device, void* commandQueue)
    : workspaceContext(workspaceContext) {
    initializeContext(device, commandQueue);
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
#ifdef __APPLE__
    return createMetalBackendSurface(texture, width, height, recorder);
#elif defined(_WIN32)
    return createVulkanBackendSurface(texture, width, height, recorder);
#else
    throw std::runtime_error("Unsupported platform");
#endif
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

void WorkspaceRenderer::initializeContext(void* device, void* commandQueue) {
#ifdef __APPLE__
    initializeMetalContext(device, commandQueue);
#elif defined(_WIN32)
    initializeVulkanContext((HWND)device);
#else
    throw std::runtime_error("Unsupported platform");
#endif
}

#ifdef __APPLE__
void WorkspaceRenderer::initializeMetalContext(void* device, void* commandQueue) {
    skgpu::graphite::MtlBackendContext backendContext = {};
    skgpu::graphite::ContextOptions options;

    backendContext.fDevice = (sk_cfp<CFTypeRef>)device;
    backendContext.fQueue = (sk_cfp<CFTypeRef>)commandQueue;

    context = skgpu::graphite::ContextFactory::MakeMetal(backendContext, options);
    if (!context) {
        throw std::runtime_error("Could not make Graphite Native Metal context");
    }
}

sk_sp<SkSurface> WorkspaceRenderer::createMetalBackendSurface(void* texture, double width, double height, skgpu::graphite::Recorder* recorder) {
    auto backendTexture = skgpu::graphite::BackendTextures::MakeMetal(SkISize::Make(width, height), (CFTypeRef)texture);

    sk_sp<SkSurface> surface = SkSurfaces::WrapBackendTexture(
        recorder, backendTexture, kBGRA_8888_SkColorType,
        SkColorSpace::MakeSRGB(), new SkSurfaceProps());

    if (!surface) {
        throw std::runtime_error("Could not make surface from Metal Recorder");
    }

    return surface;
}
#elif defined(_WIN32)
void WorkspaceRenderer::initializeVulkanContext(void* hwnd) {
    HWND window = (HWND)hwnd;
    
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Ciart Studio";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Ciart Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    
    const char* extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
    };
    createInfo.enabledExtensionCount = 2;
    createInfo.ppEnabledExtensionNames = extensions;

    if (vkCreateInstance(&createInfo, nullptr, &vulkanInstance) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan instance");
    }

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support");
    }
    
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());
    physicalDevice = devices[0];

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());
    
    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsQueueIndex = i;
            break;
        }
    }

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = graphicsQueueIndex;
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkDeviceCreateInfo deviceCreateInfo = {};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;

    if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &vulkanDevice) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create logical device");
    }

    vkGetDeviceQueue(vulkanDevice, graphicsQueueIndex, 0, &graphicsQueue);

    skgpu::VulkanBackendContext backendContext = {};
    skgpu::graphite::ContextOptions options;

    backendContext.fInstance = vulkanInstance;
    backendContext.fPhysicalDevice = physicalDevice;
    backendContext.fDevice = vulkanDevice;
    backendContext.fQueue = graphicsQueue;
    backendContext.fGraphicsQueueIndex = graphicsQueueIndex;

    context = skgpu::graphite::ContextFactory::MakeVulkan(backendContext, options);
    if (!context) {
        throw std::runtime_error("Could not make Graphite Native Vulkan context");
    }
}

sk_sp<SkSurface> WorkspaceRenderer::createVulkanBackendSurface(void* texture, double width, double height, skgpu::graphite::Recorder* recorder) {
    skgpu::graphite::VulkanTextureInfo textureInfo = {};
    textureInfo.fImageTiling = VK_IMAGE_TILING_OPTIMAL;
    textureInfo.fFormat = VK_FORMAT_B8G8R8A8_UNORM;
    textureInfo.fImageUsageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    textureInfo.fSampleCount = 1;
    
    skgpu::VulkanAlloc vulkanAlloc = {};
    skgpu::graphite::BackendTexture backendTexture = skgpu::graphite::BackendTextures::MakeVulkan(
        SkISize::Make(width, height), textureInfo, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 0, (VkImage)texture, vulkanAlloc);

    sk_sp<SkSurface> surface = SkSurfaces::WrapBackendTexture(
        recorder, backendTexture, kBGRA_8888_SkColorType,
        SkColorSpace::MakeSRGB(), new SkSurfaceProps());

    if (!surface) {
        throw std::runtime_error("Could not make surface from Vulkan Recorder");
    }

    return surface;
}
#endif
