//
//  ToolState.swift
//  Studio
//
//  Created by Pdom on 5/5/24.
//

import Foundation

class ToolStore: ObservableObject {
    let tools: [Tool]
    
    @Published var selectedIndex = 0
    
    init() {
        tools = [
            Pen(),
            Eraser()
        ]
    }
    
    func changeTool(id: ToolId) {
        selectedIndex = tools.firstIndex(where: { tool in
            tool.id == id
        }) ?? 0
    }
}
