//
//  WorkspaceView.swift
//  Studio
//
//  Created by Pdom on 5/5/24.
//

import AppKit
import SwiftUI

class NSWorkspaceView: NSView {
    var offset: CGPoint = .zero
    var angle: Angle = .zero
    var scale: CGFloat = 1
    var pageSize = CGSize(width: 200, height: 200)
    
    func computeOrigin() -> CGPoint {
        let center = bounds.size.center
        let pageCenter = pageSize.center
        
        return CGPoint(x: center.x - pageCenter.x * scale + offset.x, y: center.y - pageCenter.y * scale + offset.y)
    }
    
    func computeRotateOffset(localLocation: CGPoint, deltaAngle: Angle) -> CGPoint {
        let origin = computeOrigin()
        let distance = localLocation.distance(to: origin)
        
        let a = atan2(localLocation.y - origin.y, localLocation.x - origin.x) + deltaAngle.radians
        
        let x = cos(a)
        let y = sin(a)
        let center = bounds.size.center
        let pageCenter = pageSize.center
        
        return CGPoint(x: x * distance * scale - center.x - pageCenter.x * scale, y: y * distance * scale - center.y - pageCenter.y * scale)
    }
    
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)
        
        self.drawCanvas()
    }
    
    override func scrollWheel(with event: NSEvent) {
        offset.x += event.scrollingDeltaX
        offset.y -= event.scrollingDeltaY
        
        setNeedsDisplay(self.bounds)
    }
    
    override func rotate(with event: NSEvent) {
        let deltaAngle = Angle(degrees: Double(event.rotation))
        let locationInWindow = self.convert(event.locationInWindow, from: nil)
        
        angle += deltaAngle
        offset += computeRotateOffset(localLocation: CGPoint(x: locationInWindow.x, y: locationInWindow.y), deltaAngle: deltaAngle)
        
        setNeedsDisplay(self.bounds)
    }
    
    override func magnify(with event: NSEvent) {
        scale += event.magnification
        
        setNeedsDisplay(self.bounds)
    }
    
    override func mouseDown(with event: NSEvent) {
        print(self.convert(event.locationInWindow, from: nil))
    }
    
    override func rightMouseDown(with event: NSEvent) {
//        print(add_rust(30, 40))
        print(event)
    }
    
    func drawCanvas(){
        NSGraphicsContext.saveGraphicsState()
        
        guard let context = NSGraphicsContext.current?.cgContext else { return }
        
        let origin = computeOrigin()
        
        context.translateBy(x: origin.x, y: origin.y)
        context.scaleBy(x: scale, y: scale)
        context.rotate(by: angle.radians)
        
        context.setFillColor(.white)
        
        context.fill(CGRect(x: 0, y: 0, width: pageSize.width, height: pageSize.height))
        
        NSGraphicsContext.restoreGraphicsState()
    }
}


struct WorkspaceView: NSViewRepresentable {
    typealias NSViewType = NSWorkspaceView
    
    func makeNSView(context: Context) -> NSWorkspaceView {
        NSWorkspaceView()
    }
    
    func updateNSView(_ nsView: NSWorkspaceView, context: Context) {
    }
}

#Preview {
    WorkspaceView().frame(width: 400, height: 300)
}
