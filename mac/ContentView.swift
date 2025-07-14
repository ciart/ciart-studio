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

    var body: some View {

        HStack {
            Picker(selection: $toolStore.selectedIndex, label: Text("")) {
                Text("Pen").tag(0)
                Text("Eraser").tag(1)
            }
            .pickerStyle(.radioGroup)
            WorkspaceView()
            VStack {
                Button {

                } label: {
                    Text("Eraser")
                }
                ColorPicker("색상 선택", selection: $selectedColor)
            }
        }
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
