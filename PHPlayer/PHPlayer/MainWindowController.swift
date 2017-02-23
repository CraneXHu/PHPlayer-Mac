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
    
    func windowWillClose(_ notification: Notification) {
        AppDelegate.player.stop()
        AppDelegate.audioController.stop()
    }
    
    override func keyDown(with event: NSEvent) {
        let key = Int(event.keyCode)
        switch key {
        case 123:
            AppDelegate.player.seek(-1000)
        case 124:
            AppDelegate.player.seek(1000)
        case 49:
            if AppDelegate.player.getState() != 2 {
                AppDelegate.player.pause()
                AppDelegate.audioController.pause()
            } else {
                AppDelegate.player.play()
                AppDelegate.audioController.play()
            }
        default:
            return
        }
    }
    
    override func mouseEntered(with event: NSEvent) {
        
    }
}
