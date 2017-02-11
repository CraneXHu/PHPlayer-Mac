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
    
    var mainWindowController : MainWindowController = MainWindowController()

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        mainWindowController.showWindow(nil);
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }


}

