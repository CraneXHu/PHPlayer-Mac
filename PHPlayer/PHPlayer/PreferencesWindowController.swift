//
//  PreferencesWindowController.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/24.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Cocoa

class PreferencesWindowController: NSWindowController {

    @IBOutlet weak var toolbar: NSToolbar!
    
    var prefsGeneralViewController: PrefsGeneralViewController = PrefsGeneralViewController()
    
    var prefsShortcutKeyViewController: PrefsShortcutKeyViewController = PrefsShortcutKeyViewController()
    
    var currentViewController: NSViewController?

    override var windowNibName: String {
        return "PreferencesWindowController"
    }
    
    override func windowDidLoad() {

        super.windowDidLoad()

        // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
        
    }
    
//    override func awakeFromNib() {
//        initView()
//    }
    
    func initView() {
        
        let newView = prefsGeneralViewController.view
        let windowRect = self.window?.contentView?.frame
        let currentViewRect = newView.frame
        let yPos = windowRect!.origin.y + (windowRect!.size.height - currentViewRect.size.height)
        let newFrame = NSMakeRect(windowRect!.origin.x, yPos, currentViewRect.size.width, currentViewRect.size.height)
        self.window?.setFrame(newFrame, display: true, animate: true)
        self.window?.contentView = newView
    }
    
    @IBAction func generalSetting(_ sender: Any) {
        currentViewController = prefsGeneralViewController
        changeView()
    }
    
    @IBAction func shortcutKeySetting(_ sender: Any) {
        currentViewController = prefsShortcutKeyViewController
        changeView()
    }
    
    func changeView() {
//        currentViewController?.view.removeFromSuperview()
//        self.window?.contentView?.addSubview((currentViewController?.view)!)
        
        let newView = currentViewController?.view
        var windowRect = self.window?.frame
        let currentViewRect = newView?.frame
        windowRect!.origin.y = windowRect!.origin.y + (windowRect!.size.height - currentViewRect!.size.height)
        windowRect!.size.height = currentViewRect!.size.height
        windowRect!.size.width = currentViewRect!.size.width
        self.window?.contentView = newView
        self.window?.setFrame(windowRect!, display: true, animate: true)
    }
}
