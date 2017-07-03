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
    let preferenceWindowController = PreferencesWindowController()
    let aboutWindowController = AboutWindowController()

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        
        UserDefaults.standard.register(defaults: Preferences.defaults)
//        mainWindowController.showWindow(nil);
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }

    func application(_ sender: NSApplication, openFile filename: String) -> Bool {
        mainWindowController.showWindow(nil)
        let path = (filename as NSString).utf8String
        AppDelegate.player.open(UnsafeMutablePointer<Int8>(mutating: path))
        AppDelegate.player.start()
        mainWindowController.initData()
        return true
    }
    

    @IBAction func showAboutWindow(_ sender: Any) {
        aboutWindowController.showWindow(nil)
    }
    
    @IBAction func showPreferenceWindow(_ sender: Any) {
        preferenceWindowController.showWindow(nil)
    }
}

