//
//  ControlbarView.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/28.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Cocoa

class ControlbarView: NSVisualEffectView {

    override func awakeFromNib() {
        self.layer?.cornerRadius = 5
    }
    
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
    }
    
}
