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

struct SkiaView: NSViewRepresentable {
    typealias NSViewType = CustomMTKView

    class Coordinator: NSObject, MTKViewDelegate {
        var parent: SkiaView
        var device: MTLDevice!
        var commandQueue: MTLCommandQueue!
        var workspace: Workspace!
        var workspaceRenderer: WorkspaceRenderer!

        init(_ parent: SkiaView) {
            self.parent = parent
            self.device = MTLCreateSystemDefaultDevice()
            self.commandQueue = self.device.makeCommandQueue()

            self.workspace = Workspace()
            self.workspaceRenderer = WorkspaceRenderer(
                Unmanaged.passUnretained(device).toOpaque(),
                Unmanaged.passUnretained(commandQueue).toOpaque())

            super.init()

            withUnsafeMutablePointer(to: &workspace!) { workspacePtr in
                self.workspaceRenderer.setWorkspace(
                    workspacePtr,
                )
            }
        }

        func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
            workspaceRenderer?.resize(size.width, size.height)
        }

        func draw(in view: MTKView) {
            guard let drawable = view.currentDrawable else { return }

            let commandBuffer = commandQueue.makeCommandBuffer()

            workspaceRenderer?.draw(
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
            let location = mtkView.convert(event.locationInWindow, from: nil)
            print("Mouse moved to: \(location)")
        }

        mtkView.onMouseDown = { event in
            let location = mtkView.convert(event.locationInWindow, from: nil)
            print("Mouse down at: \(location)")
        }

        mtkView.onMouseUp = { event in
            let location = mtkView.convert(event.locationInWindow, from: nil)
            print("Mouse up at: \(location)")
        }

        mtkView.onMouseDragged = { event in
            let location = mtkView.convert(event.locationInWindow, from: nil)
            print("Mouse dragged to: \(location)")
        }

        mtkView.onScrollWheel = { event in
            if event.hasPreciseScrollingDeltas {
                context.coordinator.workspace.move(event.scrollingDeltaX, event.scrollingDeltaY)
            } else {
                context.coordinator.workspace.zoom(event.scrollingDeltaY)
            }
            mtkView.setNeedsDisplay(mtkView.bounds)
            print("Scroll: precise=\(event.hasPreciseScrollingDeltas), deltaX=\(event.scrollingDeltaX), deltaY=\(event.scrollingDeltaY)")
        }

        mtkView.onMagnify = { event in
            context.coordinator.workspace.zoom(event.magnification)
            mtkView.setNeedsDisplay(mtkView.bounds)
            print("Magnification: \(event.magnification)")
        }

        mtkView.onRotate = { event in
            print("Rotation: \(event.rotation)")
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
    SkiaView()
}
