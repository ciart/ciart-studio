//
//  SkiaView.swift
//  Studio
//
//  Created by Pdom on 5/28/24.
//

import AppKit
import CiartStudioCore
import MetalKit
import SwiftUI

typealias Workspace = Ciart.Studio.Workspace
typealias WorkspaceRenderer = Ciart.Studio.WorkspaceRenderer
typealias ToolEvent = Ciart.Studio.ToolEvent
typealias ToolEventType = Ciart.Studio.ToolEventType
typealias Offset = Ciart.Studio.Offset

class CustomMTKView: MTKView {
    var onMouseMoved: ((NSEvent) -> Void)?
    var onMouseDown: ((NSEvent) -> Void)?
    var onMouseUp: ((NSEvent) -> Void)?
    var onMouseDragged: ((NSEvent) -> Void)?
    var onScrollWheel: ((NSEvent) -> Void)?
    var onMagnify: ((NSEvent) -> Void)?
    var onRotate: ((NSEvent) -> Void)?

    override func updateTrackingAreas() {
        super.updateTrackingAreas()

        for trackingArea in self.trackingAreas {
            self.removeTrackingArea(trackingArea)
        }

        let options: NSTrackingArea.Options = [
            .activeInKeyWindow, .mouseMoved, .mouseEnteredAndExited,
        ]
        let trackingArea = NSTrackingArea(
            rect: self.bounds, options: options, owner: self, userInfo: nil)
        self.addTrackingArea(trackingArea)
    }

    override func mouseMoved(with event: NSEvent) {
        onMouseMoved?(event)
    }

    override func mouseDown(with event: NSEvent) {
        onMouseDown?(event)
    }

    override func mouseUp(with event: NSEvent) {
        onMouseUp?(event)
    }

    override func mouseDragged(with event: NSEvent) {
        onMouseDragged?(event)
    }

    override func scrollWheel(with event: NSEvent) {
        onScrollWheel?(event)
    }

    override func magnify(with event: NSEvent) {
        onMagnify?(event)
    }

    override func rotate(with event: NSEvent) {
        onRotate?(event)
    }
}

struct WorkspaceView: NSViewRepresentable {
    typealias NSViewType = CustomMTKView
    @Binding var statusMessage: String
    @EnvironmentObject var toolStore: ToolStore
    
    private func screenToWorkspaceCoordinate(devicePixelScale: CGFloat, screenPoint: CGPoint, viewBounds: CGRect, workspace: borrowing Workspace) -> Offset {
        let flippedY = (viewBounds.height - screenPoint.y) * devicePixelScale

        // WorkspaceRenderer와 정확히 동일한 변수명과 계산 사용
        let scale = workspace.getScale()
        let size = workspace.getSize()
        let offset = workspace.getOffset()
        let screen_center_dx = viewBounds.width / 2 * devicePixelScale
        let screen_center_dy = viewBounds.height / 2 * devicePixelScale
        let workspace_center_dx = size.width / 2
        let workspace_center_dy = size.height / 2
        
        let workspace_offset_x = screen_center_dx - workspace_center_dx * scale + offset.dx
        let workspace_offset_y = screen_center_dy - workspace_center_dy * scale + offset.dy
        
        let localX = (screenPoint.x * devicePixelScale - workspace_offset_x) / scale
        let localY = (flippedY - workspace_offset_y) / scale
        
        var result = Offset()
        result.dx = localX
        result.dy = localY
        return result
    }

    class Coordinator: NSObject, MTKViewDelegate {
        var parent: WorkspaceView
        var device: MTLDevice!
        var commandQueue: MTLCommandQueue!
        var workspace: Workspace!
        var workspaceRenderer: UnsafeMutablePointer<WorkspaceRenderer>!

        init(_ parent: WorkspaceView) {
            self.parent = parent
            self.device = MTLCreateSystemDefaultDevice()
            self.commandQueue = self.device.makeCommandQueue()

            self.workspace = Workspace()
            self.workspaceRenderer = self.workspace.createRenderer(
                Unmanaged.passUnretained(device).toOpaque(),
                Unmanaged.passUnretained(commandQueue).toOpaque())

            super.init()
            
            // ToolStore와 ToolManager 연결
            let toolManager = workspace.getToolManager().pointee
            parent.toolStore.setToolManager(toolManager)
        }

        func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
            
        }

        func draw(in view: MTKView) {
            guard let drawable = view.currentDrawable else { return }

            let commandBuffer = commandQueue.makeCommandBuffer()

            workspaceRenderer?.pointee.draw(
                Unmanaged.passUnretained(drawable.texture).toOpaque(),
                drawable.layer.drawableSize.width, drawable.layer.drawableSize.height)

            // let rpd = view.currentRenderPassDescriptor
            // rpd?.colorAttachments[0].clearColor = MTLClearColor(red: 0, green: 0, blue: 0, alpha: 1)
            // rpd?.colorAttachments[0].loadAction = .clear
            // rpd?.colorAttachments[0].storeAction = .store
            // let re = commandBuffer?.makeRenderCommandEncoder(descriptor: rpd!)
            // re?.endEncoding()

            commandBuffer?.present(drawable)
            commandBuffer?.commit()
        }
    }

    func makeNSView(context: Context) -> NSViewType {
        let mtkView = CustomMTKView()
        mtkView.delegate = context.coordinator
        mtkView.enableSetNeedsDisplay = true
        mtkView.presentsWithTransaction = false
        mtkView.framebufferOnly = false
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.clearColor = MTLClearColorMake(0, 0, 0, 1)
        mtkView.device = MTLCreateSystemDefaultDevice()

        // Set up event handlers
        mtkView.onMouseMoved = { event in
            let devicePixelScale = event.window?.backingScaleFactor ?? 1
            let location = mtkView.convert(event.locationInWindow, from: nil)
            let offset = self.screenToWorkspaceCoordinate(devicePixelScale: devicePixelScale, screenPoint: location, viewBounds: mtkView.bounds, workspace: context.coordinator.workspace)
            context.coordinator.workspace.setMousePosition(offset)
            mtkView.setNeedsDisplay(mtkView.bounds)
            self.statusMessage = "Mouse at: \(location) -> workspace: (\(offset.dx), \(offset.dy))"
        }

        mtkView.onMouseDown = { event in
            let devicePixelScale = event.window?.backingScaleFactor ?? 1
            let location = mtkView.convert(event.locationInWindow, from: nil)
            let offset = self.screenToWorkspaceCoordinate(devicePixelScale: devicePixelScale, screenPoint: location, viewBounds: mtkView.bounds, workspace: context.coordinator.workspace)
            var toolEvent = ToolEvent()
            toolEvent.type = ToolEventType.Press
            toolEvent.position = offset
            toolEvent.pressure = 1.0
            context.coordinator.workspace.handleToolEvent(toolEvent)
            mtkView.setNeedsDisplay(mtkView.bounds)
            self.statusMessage = "Drawing at screen: \(location) -> workspace: (\(offset.dx), \(offset.dy))"
        }

        mtkView.onMouseUp = { event in
            let devicePixelScale = event.window?.backingScaleFactor ?? 1
            let location = mtkView.convert(event.locationInWindow, from: nil)
            let offset = self.screenToWorkspaceCoordinate(devicePixelScale: devicePixelScale, screenPoint: location, viewBounds: mtkView.bounds, workspace: context.coordinator.workspace)
            var toolEvent = ToolEvent()
            toolEvent.type = ToolEventType.Release
            toolEvent.position = offset
            toolEvent.pressure = 1.0
            context.coordinator.workspace.handleToolEvent(toolEvent)
            mtkView.setNeedsDisplay(mtkView.bounds)
            self.statusMessage = "Drawing ended at: \(location)"
        }

        mtkView.onMouseDragged = { event in
            let devicePixelScale = event.window?.backingScaleFactor ?? 1
            let location = mtkView.convert(event.locationInWindow, from: nil)
            let offset = self.screenToWorkspaceCoordinate(devicePixelScale: devicePixelScale, screenPoint: location, viewBounds: mtkView.bounds, workspace: context.coordinator.workspace)
            context.coordinator.workspace.setMousePosition(offset)
            var toolEvent = ToolEvent()
            toolEvent.type = ToolEventType.Move
            toolEvent.position = offset
            toolEvent.pressure = 1.0
            context.coordinator.workspace.handleToolEvent(toolEvent)
            mtkView.setNeedsDisplay(mtkView.bounds)
            self.statusMessage = "Drawing at: \(location)"
        }

        mtkView.onScrollWheel = { event in
            if event.hasPreciseScrollingDeltas {
                context.coordinator.workspace.move(event.scrollingDeltaX, event.scrollingDeltaY)
            } else {
                context.coordinator.workspace.zoom(event.scrollingDeltaY)
            }

            mtkView.setNeedsDisplay(mtkView.bounds)
            self.statusMessage = "Scroll: precise=\(event.hasPreciseScrollingDeltas), deltaX=\(event.scrollingDeltaX), deltaY=\(event.scrollingDeltaY)"
        }

        mtkView.onMagnify = { event in
            context.coordinator.workspace.zoom(event.magnification)
            mtkView.setNeedsDisplay(mtkView.bounds)
            self.statusMessage = "Magnification: \(event.magnification)"
        }

        mtkView.onRotate = { event in
            self.statusMessage = "Rotation: \(event.rotation)"
        }

        return mtkView
    }

    func updateNSView(_ nsView: NSViewType, context: Context) {
        
    }

    func makeCoordinator() -> Coordinator {
        Coordinator(self)
    }
}

#Preview {
    WorkspaceView(statusMessage: .constant("Preview message")).frame(width: 400, height: 300)
}
