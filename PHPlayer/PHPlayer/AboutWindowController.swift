//
//  AboutWindowController.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/25.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Cocoa

class AboutWindowController: NSWindowController {

    @IBOutlet weak var iconImageView: NSImageView!
    override var windowNibName: String {
        return "AboutWindowController"
    }
    
    override func windowDidLoad() {
        super.windowDidLoad()

        // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
        iconImageView.image = NSApp.applicationIconImage
    }
    
}
