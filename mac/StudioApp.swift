//
//  StudioApp.swift
//  Studio
//
//  Created by Pdom on 7/15/23.
//

import SwiftUI

@main
struct StudioApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView().environmentObject(DocumentStore())
                .toolbarBackgroundVisibility(.hidden, for: .windowToolbar)
        }
    }
}
