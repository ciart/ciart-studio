//
//  ToolState.swift
//  Studio
//
//  Created by Pdom on 5/5/24.
//

import Foundation
import CiartStudioCore

typealias ToolManager = Ciart.Studio.ToolManager
typealias ToolType = Ciart.Studio.ToolType

class ToolStore: ObservableObject {
    @Published var selectedIndex = 0
    @Published var toolSize: Float = 10.0
    @Published var toolOpacity: Float = 1.0
    
    private var toolManager: ToolManager?
    
    init() {
        // ToolManager는 WorkspaceView에서 설정됨
    }
    
    func setToolManager(_ toolManager: ToolManager) {
        self.toolManager = toolManager
        updateFromToolManager()
    }
    
    func selectTool(index: Int) {
        selectedIndex = index
        toolManager?.selectTool(Int32(index))
        updateFromToolManager()
    }
    
    func selectTool(type: ToolType) {
        toolManager?.selectTool(type)
        selectedIndex = Int(toolManager?.getCurrentToolIndex() ?? 0)
        updateFromToolManager()
    }
    
    func setToolSize(_ size: Float) {
        toolSize = size
        toolManager?.setToolSize(size)
    }
    
    func setToolOpacity(_ opacity: Float) {
        toolOpacity = opacity
        toolManager?.setToolOpacity(opacity)
    }
    
    func getToolSize() -> Float {
        return toolManager?.getToolSize() ?? 10.0
    }
    
    func getToolOpacity() -> Float {
        return toolManager?.getToolOpacity() ?? 1.0
    }
    
    func getCurrentToolType() -> ToolType {
        return toolManager?.getCurrentToolType() ?? .Pen
    }
    
    private func updateFromToolManager() {
        if let toolManager = toolManager {
            toolSize = toolManager.getToolSize()
            toolOpacity = toolManager.getToolOpacity()
        }
    }
}
