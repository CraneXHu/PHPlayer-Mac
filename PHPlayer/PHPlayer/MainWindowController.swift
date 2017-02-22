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
    
//    let obj: CPPWrapper = CPPWrapper()
//    var audioPlayer : AudioController?
    
    override func windowDidLoad() {
        super.windowDidLoad()
        
        self.window?.titleVisibility = .hidden
        self.window?.styleMask.insert(NSFullSizeContentViewWindowMask)
        self.window?.titlebarAppearsTransparent = true
        
//        audioPlayer = AudioController(player: obj)
//        
////        videoView.initTexture()
//        videoView.setObj(obj: obj)
//        let path:String = "/Users/pkhope/Downloads/WW.S01E09.720p.FIX字幕侠/WW.S01E09.720p.FIX字幕侠.mkv"
//        let pathCStr = (path as NSString).utf8String
//        obj.open(UnsafeMutablePointer<Int8>(mutating: pathCStr))
//        obj.start()
//
//        audioPlayer?.initWithAudioSpec(sampleRate: 48000, channels: 2)
//        audioPlayer?.play();
    }
    
    override func keyDown(with event: NSEvent) {
        let key = Int(event.keyCode)
        if key == NSLeftArrowFunctionKey {
            
        }
    }
    
    override func mouseEntered(with event: NSEvent) {
        
    }
}
