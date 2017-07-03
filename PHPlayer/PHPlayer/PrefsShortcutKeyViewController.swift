//
//  PrefsShortcutKeyViewController.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/25.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Cocoa

class PrefsShortcutKeyViewController: NSViewController, NSTableViewDelegate, NSTableViewDataSource {

    override var nibName: String? {
        return "PrefsShortcutKeyViewController"
    }
    
    var keyMaps: [(cmd:String,key:String)] = [];
    
    @IBOutlet weak var tableView: NSTableView!
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        
        tableView.dataSource = self
        tableView.delegate = self
        
        initData()

    }
    
    func numberOfRows(in tableView: NSTableView) -> Int {
        return keyMaps.count
    }
    
    func tableView(_ tableView: NSTableView, objectValueFor tableColumn: NSTableColumn?, row: Int) -> Any? {
        let identifier = tableColumn?.identifier
        
        if identifier == "Cmd" {
            return keyMaps[row].cmd;
        } else {
            return keyMaps[row].key;
        }
    }
    
    func initData() {
        keyMaps.append((cmd:"Pause/Play", "Space"))
        tableView.reloadData()
    }
    
}
