//
//  MainWindowController.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/9.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Foundation
import Cocoa

class MainWindowController: NSWindowController, NSWindowDelegate {
    
    override var windowNibName: String {
        return "MainWindowController"
    }
    @IBOutlet weak var videoView: VideoView!
    
    override func windowDidLoad() {
        super.windowDidLoad()
        
        self.window?.titleVisibility = .hidden
        self.window?.styleMask.insert(NSFullSizeContentViewWindowMask)
        self.window?.titlebarAppearsTransparent = true
    }
    
    override func keyDown(with event: NSEvent) {
        let key = Int(event.keyCode)
        if key == NSLeftArrowFunctionKey {
            
        }
    }
    
    override func mouseEntered(with event: NSEvent) {
        
    }
}
