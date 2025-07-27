import SwiftUI

struct StatusBar: View {
    let message: String
    
    var body: some View {
        HStack {
            Text(message)
                .font(.system(size: 12, design: .monospaced))
                .foregroundColor(.secondary)
                .padding(.horizontal, 8)
            Spacer()
        }
        .frame(height: 24)
        .background(Color(NSColor.controlBackgroundColor))
        .border(Color(NSColor.separatorColor), width: 0.5)
    }
}

#Preview {
    StatusBar(message: "Mouse moved to: (100.0, 200.0)")
}