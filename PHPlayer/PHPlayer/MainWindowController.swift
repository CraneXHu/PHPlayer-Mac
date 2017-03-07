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
    
    @IBOutlet weak var controlbar: ControlbarView!
    
    @IBOutlet weak var currentDuration: NSTextField!
    
    @IBOutlet weak var duration: NSTextField!
    
    @IBOutlet weak var videoSlider: NSSliderCell!
    
    @IBOutlet weak var audioSlider: NSSlider!
    
    @IBOutlet weak var windowTitle: NSTextField!
    
    var syncPlayTimeTimer: Timer?
    
    override func windowDidLoad() {
        super.windowDidLoad()
        
        self.window?.titleVisibility = .hidden
        self.window?.styleMask.insert(NSFullSizeContentViewWindowMask)
        self.window?.titlebarAppearsTransparent = true
        
    }
    
    func windowWillClose(_ notification: Notification) {
        syncPlayTimeTimer?.invalidate()
        AppDelegate.player.stop()
        AppDelegate.audioController.stop()
    }
    
    override func keyDown(with event: NSEvent) {
        let key = Int(event.keyCode)
        switch key {
        case 123:
            AppDelegate.player.backward(5)
        case 124:
            AppDelegate.player.forward(5)
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
    
    func initData() {
        videoView.initData()
        setTitle()
        setDurtion()
        initTimer()
        
        audioSlider.doubleValue = 1.0
    }
    
    func setDurtion() {
        duration.stringValue = String(AppDelegate.player.getDuration())
    }
    
    func initTimer() {
        syncPlayTimeTimer = Timer.scheduledTimer(timeInterval: 0.5, target: self, selector: #selector(self.updateTime), userInfo: nil, repeats: true)
    }
    
    func updateTime() {
        currentDuration.stringValue = String(AppDelegate.player.getCurrentTime())
        videoSlider.doubleValue = 100*AppDelegate.player.getCurrentTime()/AppDelegate.player.getDuration()
    }
    
    @IBAction func videoSliderChanged(_ sender: NSSlider) {
        let seekPostion = Double(AppDelegate.player.getDuration())*sender.doubleValue/sender.maxValue
        AppDelegate.player.seek(seekPostion)
    }
    
    @IBAction func volumeSliderChanged(_ sender: NSSlider) {
        AppDelegate.audioController.setPlaybackVolume(playbackVolume: Float(sender.doubleValue))
    }
    
    @IBAction func playAction(_ sender: NSButton) {
        
        if sender.state == NSOffState {
            AppDelegate.player.pause()
            AppDelegate.audioController.pause()
        } else {
            AppDelegate.player.play()
            AppDelegate.audioController.play()
        }
    }

    func setTitle() {
        windowTitle.stringValue = String(cString: AppDelegate.player.getFileName())
    }
}
