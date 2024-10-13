import SwiftUI
import CiartStudioCore

@main
struct MyApp: App {
    @State private var a = Ciart.Studio.Workspace(123)

    var body: some Scene {
        WindowGroup {
            ContentView()
            Text("\(a.print())")
        }
    }
}

struct ContentView: View {
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundStyle(.tint)
            Text("Hello, world!")
        }
        .padding()
    }
}
