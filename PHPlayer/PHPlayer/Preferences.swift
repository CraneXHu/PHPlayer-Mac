//
//  File.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/25.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Foundation

struct Preferences {
    
    struct Keys {
        static let enableHardwareDecoding = "enableHardwareDecoding"
        static let seekTime = "seekTime"
    }
    
    static let defaults: [String:Any] = [
        Keys.enableHardwareDecoding: true,
        Keys.seekTime: 5
        ]
}
