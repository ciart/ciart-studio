//
//  SkiaView.swift
//  Studio
//
//  Created by Pdom on 5/28/24.
//

import SwiftUI
import MetalKit

struct SkiaView: NSViewRepresentable {
    typealias NSViewType = MTKView
    
    class Coordinator: NSObject, MTKViewDelegate {
        var parent: SkiaView
        var device: MTLDevice!
        var commandQueue: MTLCommandQueue!
        var context: OpaquePointer!
        
        init(_ parent: SkiaView) {
            self.parent = parent
            self.device = MTLCreateSystemDefaultDevice()
            self.commandQueue = self.device.makeCommandQueue()
            
            let rustDevice = Unmanaged.passUnretained(device).toOpaque()
            let rustCommandQueue = Unmanaged.passUnretained(commandQueue).toOpaque()
            
//            context = create_metal_context(rustDevice, rustCommandQueue, nil)
            
            super.init()
        }
        
        func mtkView(_ view: MTKView, drawableSizeWillChange size: CGSize) {
            
        }
        
        func draw(in view: MTKView) {
            guard let drawable = view.currentDrawable else { return }

            let commandBuffer = commandQueue.makeCommandBuffer()
//
//            skia_draw(context, Unmanaged.passUnretained(drawable.texture).toOpaque(), Float(drawable.layer.drawableSize.width), Float(drawable.layer.drawableSize.height))
            
            let rpd = view.currentRenderPassDescriptor
            rpd?.colorAttachments[0].clearColor = MTLClearColor(red: 0.5, green: 1, blue: 0, alpha: 1)
            rpd?.colorAttachments[0].loadAction = .clear
            rpd?.colorAttachments[0].storeAction = .store
            let re = commandBuffer?.makeRenderCommandEncoder(descriptor: rpd!)
            re?.endEncoding()
            
            commandBuffer?.present(drawable)
            commandBuffer?.commit()
        }
    }
    
    func makeNSView(context: Context) -> NSViewType {
        let mtkView = MTKView()
        mtkView.delegate = context.coordinator
        mtkView.enableSetNeedsDisplay = true
        mtkView.presentsWithTransaction = false
        mtkView.framebufferOnly = false
        mtkView.colorPixelFormat = .bgra8Unorm
        mtkView.clearColor = MTLClearColorMake(0.0, 1, 1, 1.0)
        mtkView.device = MTLCreateSystemDefaultDevice()

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
