//
//  ToolbarView.swift
//  Studio
//
//  Created by Pdom on 7/28/25.
//

import SwiftUI

struct ToolbarView: View {
    @EnvironmentObject var toolStore: ToolStore
    
    var body: some View {
        VStack(spacing: 0) {
            Button(action: { toolStore.selectTool(index: 0) }) {
                Image(systemName: "pencil")
                    .foregroundColor(toolStore.selectedIndex == 0 ? .white : .gray)
                    .frame(width: 32, height: 32)
            }
            .buttonStyle(.plain)
            .background(toolStore.selectedIndex == 0 ? Color.blue : Color.clear)
            .cornerRadius(4)
            
            Button(action: { toolStore.selectTool(index: 1) }) {
                Image(systemName: "eraser")
                    .foregroundColor(toolStore.selectedIndex == 1 ? .white : .gray)
                    .frame(width: 32, height: 32)
            }
            .buttonStyle(.plain)
            .background(toolStore.selectedIndex == 1 ? Color.blue : Color.clear)
            .cornerRadius(4)
        }
    }
}

#Preview {
    ToolbarView().environmentObject(ToolStore())
}