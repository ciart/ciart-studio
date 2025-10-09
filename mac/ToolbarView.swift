//
//  ToolbarView.swift
//  Studio
//
//  Created by Pdom on 7/28/25.
//

import SwiftUI

struct ToolbarView: View {
    @State private var selectedIndex = 0

    var body: some View {
        VStack(spacing: 0) {
            Button(action: { selectedIndex = 0 }) {
                Image(systemName: "pencil")
                    .foregroundColor(selectedIndex == 0 ? .white : .gray)
                    .frame(width: 32, height: 32)
            }
            .buttonStyle(.plain)
            .background(selectedIndex == 0 ? Color.blue : Color.clear)
            .cornerRadius(4)

            Button(action: { selectedIndex = 1 }) {
                Image(systemName: "eraser")
                    .foregroundColor(selectedIndex == 1 ? .white : .gray)
                    .frame(width: 32, height: 32)
            }
            .buttonStyle(.plain)
            .background(selectedIndex == 1 ? Color.blue : Color.clear)
            .cornerRadius(4)
        }
    }
}

#Preview {
    ToolbarView()
}
