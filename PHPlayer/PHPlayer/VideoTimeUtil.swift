//
//  VideoTimeUtil.swift
//  PHPlayer
//
//  Created by huhexiang on 17/3/17.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Foundation

class VideoTimeUtil {
    
    static func convert(second: Int) -> String {
        let h = second / 3600
        let m = (second % 3600) / 60
        let s = (second % 3600) % 60
        let ms = (m < 10 ? "0\(m)" : "\(m)")
        let ss = (s < 10 ? "0\(s)" : "\(s)")
        let hs = (h > 0 ? "\(h):" : "")
        return "\(hs)\(ms):\(ss)"
    }
}
