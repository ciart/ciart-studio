//
//  ColorPicker.swift
//  CiartStudio
//
//  Created by Pdom on 7/28/25.
//

import SwiftUI

struct ColorPickerPanelView: View {
    var body: some View {
        ZStack {
            Circle()
                .strokeBorder(AngularGradient(
                    gradient: Gradient(colors: [
                        Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [1, 0, 0, 1])!),
                        Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [1, 1, 0, 1])!),
                        Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [0, 1, 0, 1])!),
                        Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [0, 1, 1, 1])!),
                        Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [0, 0, 1, 1])!),
                        Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [1, 0, 1, 1])!),
                        Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [1, 0, 0, 1])!),
                    ]),
                    center: .center
                ), lineWidth: 10)
                .frame(width: 268, height: 268)
            
            Rectangle()
                .fill(LinearGradient(colors: [
                    Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [1, 1, 1, 1])!),
                    Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [1, 0, 0, 1])!),
                ], startPoint: .init(x: 0, y: 0), endPoint: .init(x: 1, y: 0)))
                .frame(width: 159, height: 159)
            
            Rectangle()
                .fill(LinearGradient(colors: [
                    Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [1, 1, 1, 1])!),
                    Color(cgColor: CGColor(colorSpace: CGColorSpace(name: CGColorSpace.displayP3)!, components: [0, 0, 0, 1])!),
                ], startPoint: .init(x: 0, y: 0), endPoint: .init(x: 0, y: 1)))
                .frame(width: 159, height: 159)
                .blendMode(.multiply)
        }
        .frame(height: 268)
        .padding(.top, 12)
    }
}

#Preview {
    ColorPickerPanelView()
}
