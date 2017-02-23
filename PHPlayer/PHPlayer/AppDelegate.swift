//
//  AppDelegate.swift
//  PHPlayer
//
//  Created by huhexiang on 17/1/16.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Cocoa

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    
    static let player = PlayerWrapper()
    let mainWindowController : MainWindowController = MainWindowController()
    let audioController : AudioController = AudioController()

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        
        mainWindowController.showWindow(nil);
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func application(_ sender: NSApplication, openFile filename: String) -> Bool {

        let path = (filename as NSString).utf8String
        AppDelegate.player.open(UnsafeMutablePointer<Int8>(mutating: path))
        AppDelegate.player.start()
        mainWindowController.videoView.initData()
        audioController.initData()
        audioController.play()
        return true
    }
}

