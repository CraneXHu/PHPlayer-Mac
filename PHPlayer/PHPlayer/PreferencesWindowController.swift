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
    
    var toolbarHeight: CGFloat?
    
    var prefsGeneralViewController: PrefsGeneralViewController = PrefsGeneralViewController()
    
    var prefsShortcutKeyViewController: PrefsShortcutKeyViewController = PrefsShortcutKeyViewController()
    
    var currentViewController: NSViewController?

    override var windowNibName: String {
        return "PreferencesWindowController"
    }
    
    override func windowDidLoad() {

        super.windowDidLoad()

        // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.

        toolbarHeight = getToolbarHeight()
    }
    
    func getToolbarHeight() -> CGFloat {
        return NSHeight(self.window!.frame) - NSHeight(self.window!.contentView!.frame)
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
        
        let newView = currentViewController?.view
        var windowRect = self.window?.frame
        let oldViewRect = self.window?.contentView?.frame
        let currentViewRect = newView?.frame
        windowRect!.origin.y = windowRect!.origin.y + (oldViewRect!.size.height - currentViewRect!.size.height)
        windowRect!.size.height = currentViewRect!.size.height + toolbarHeight!
        windowRect!.size.width = currentViewRect!.size.width
        self.window?.contentView = newView
        self.window?.setFrame(windowRect!, display: true, animate: true)
    }
}
