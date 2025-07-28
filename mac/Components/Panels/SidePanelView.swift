//
//  SidePanelView.swift
//  CiartStudio
//
//  Created by Pdom on 7/28/25.
//

import SwiftUI

struct SidePanelView<Content: View>: View {
    let title: String
    
    @ViewBuilder let content: Content
    
    var body: some View {
        VStack {
            Text(title)
                .font(.system(size: 12))
                .padding(.vertical, 8)
                .padding(.horizontal, 12)
                .frame(maxWidth: .infinity, alignment: .leading)
            content
        }
        .background(in: RoundedRectangle(cornerSize: .init(width: 10, height: 10)))
        .backgroundStyle(Color(red: 0.15, green: 0.15, blue: 0.17).shadow(.inner(color: .white.opacity(0.1), radius: 0, x: 0, y: 1)))
    }
}
