//
//  Tool.swift
//  Studio
//
//  Created by Pdom on 5/5/24.
//

enum ToolId {
    case pen
    case eraser
}

protocol Tool {
    var id: ToolId { get }
    
    func onPress(document: Document)
    
    func onMove(document: Document)
    
    func onRelease(document: Document)
}
