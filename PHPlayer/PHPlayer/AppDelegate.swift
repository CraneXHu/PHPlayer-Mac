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

    @IBOutlet weak var window: NSWindow!


    @IBOutlet weak var resultLabel: NSTextField!

    func applicationDidFinishLaunching(_ aNotification: Notification) {
        // Insert code here to initialize your application
        let obj: CPPWrapper = CPPWrapper()
        let result: Int = Int(obj.add(1, and: 2))
        resultLabel.stringValue = String(result)
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }


}

