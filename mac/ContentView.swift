//
//  ContentView.swift
//  Studio
//
//  Created by Pdom on 7/15/23.
//

import SwiftUI

struct ContentView: View {
    @EnvironmentObject var toolStore: ToolStore
    @State private var selectedColor: Color = .blue
    @State private var statusMessage: String = "Ready"

    var body: some View {
        VStack(spacing: 0) {
            HStack(spacing: 0) {
                ToolbarView()
                HSplitView {
                    SidePanelView(title: "Tool Property") {
                        VStack(alignment: .leading, spacing: 16) {
                            VStack(alignment: .leading, spacing: 8) {
                                Text("선택된 도구")
                                    .foregroundColor(.white)
                                    .font(.headline)
                                
                                Text(toolStore.selectedIndex == 0 ? "연필" : "지우개")
                                    .foregroundColor(.gray)
                                    .font(.subheadline)
                            }
                            
                            Divider()
                                .background(Color.gray)
                            
                            VStack(alignment: .leading, spacing: 12) {
                                VStack(alignment: .leading, spacing: 4) {
                                    Text("도구 크기")
                                        .foregroundColor(.white)
                                        .font(.subheadline)
                                    
                                    HStack {
                                        Slider(value: Binding(
                                            get: { toolStore.toolSize },
                                            set: { toolStore.setToolSize($0) }
                                        ), in: 1...50)
                                        .accentColor(.blue)
                                        
                                        Text("\(Int(toolStore.toolSize))")
                                            .foregroundColor(.gray)
                                            .font(.caption)
                                            .frame(width: 25, alignment: .trailing)
                                    }
                                }
                                
                                VStack(alignment: .leading, spacing: 4) {
                                    Text("불투명도")
                                        .foregroundColor(.white)
                                        .font(.subheadline)
                                    
                                    HStack {
                                        Slider(value: Binding(
                                            get: { toolStore.toolOpacity },
                                            set: { toolStore.setToolOpacity($0) }
                                        ), in: 0.1...1.0)
                                        .accentColor(.blue)
                                        
                                        Text("\(Int(toolStore.toolOpacity * 100))%")
                                            .foregroundColor(.gray)
                                            .font(.caption)
                                            .frame(width: 35, alignment: .trailing)
                                    }
                                }
                            }
                            
                            Spacer()
                        }
                        .padding()
                        .frame(minWidth:200, idealWidth: 200, maxWidth: .infinity, maxHeight: .infinity)
                    }
                    WorkspaceView(statusMessage: $statusMessage)
                        .layoutPriority(1)
                        .frame(minWidth: 200, maxWidth: .infinity, minHeight: 200, maxHeight: .infinity)
                    SidePanelView(title: "Color Picker") {
                        VStack {
                            ColorPickerPanelView()
                            ColorPicker("색상 선택", selection: $selectedColor)
                        }.frame(minWidth: 200, idealWidth: 200, maxWidth: .infinity, maxHeight: .infinity)
                    }
                }
                .frame(maxHeight: .infinity)
            }
            StatusBar(message: statusMessage)
        }
        .frame(maxHeight: .infinity)
        .background(Color.black)
    }
}

#Preview {
    ContentView().environmentObject(ToolStore())
}

extension CGSize {
    static func + (lhs: Self, rhs: Self) -> Self {
        Self(width: lhs.width + rhs.width, height: lhs.height + rhs.height)
    }

    static func + (lhs: Self, rhs: CGFloat) -> Self {
        Self(width: lhs.width + rhs, height: lhs.height + rhs)
    }

    static func - (lhs: Self, rhs: Self) -> Self {
        Self(width: lhs.width - rhs.width, height: lhs.height - rhs.height)
    }

    static func - (lhs: Self, rhs: CGFloat) -> Self {
        Self(width: lhs.width - rhs, height: lhs.height - rhs)
    }

    static func * (lhs: Self, rhs: Self) -> Self {
        Self(width: lhs.width * rhs.width, height: lhs.height * rhs.height)
    }

    static func * (lhs: Self, rhs: CGFloat) -> Self {
        Self(width: lhs.width * rhs, height: lhs.height * rhs)
    }

    static func / (lhs: Self, rhs: Self) -> Self {
        Self(width: lhs.width / rhs.width, height: lhs.height / rhs.height)
    }

    static func / (lhs: Self, rhs: CGFloat) -> Self {
        Self(width: lhs.width / rhs, height: lhs.height / rhs)
    }

    var center: CGPoint {
        CGPoint(x: self.width / 2, y: self.height / 2)
    }
}

extension CGPoint {
    static func + (lhs: Self, rhs: Self) -> Self {
        Self(x: lhs.x + rhs.x, y: lhs.y + rhs.y)
    }

    static func + (lhs: Self, rhs: CGFloat) -> Self {
        Self(x: lhs.x + rhs, y: lhs.y + rhs)
    }

    static func += (lhs: inout Self, rhs: Self) {
        lhs = lhs + rhs
    }

    static func - (lhs: Self, rhs: Self) -> Self {
        Self(x: lhs.x - rhs.x, y: lhs.y - rhs.y)
    }

    static func - (lhs: Self, rhs: CGFloat) -> Self {
        Self(x: lhs.x - rhs, y: lhs.y - rhs)
    }

    static func * (lhs: Self, rhs: Self) -> Self {
        Self(x: lhs.x * rhs.x, y: lhs.y * rhs.y)
    }

    static func * (lhs: Self, rhs: CGFloat) -> Self {
        Self(x: lhs.x * rhs, y: lhs.y * rhs)
    }

    static func / (lhs: Self, rhs: Self) -> Self {
        Self(x: lhs.x / rhs.x, y: lhs.y / rhs.y)
    }

    static func / (lhs: Self, rhs: CGFloat) -> Self {
        Self(x: lhs.x / rhs, y: lhs.y / rhs)
    }

    func distance(to point: CGPoint) -> CGFloat {
        return hypot(point.x - x, point.y - y)
    }
}
