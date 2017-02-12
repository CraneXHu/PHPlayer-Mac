//
//  VideoView.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/9.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Cocoa
import OpenGL.GL
import OpenGL.GL3

class VideoView: NSOpenGLView {
    
    
    func setUpCallback(obj: CPPWrapper) {

        var voidSelf = UnsafeMutableRawPointer(Unmanaged.passRetained(self).toOpaque())
        
        obj.setCallback({ (ctx: UnsafeMutableRawPointer?, data: UnsafeMutablePointer<UInt8>?, width: Int32, height: Int32, linesize: UnsafeMutablePointer<Int32>?) -> Void in
            let videoView = unsafeBitCast(ctx, to: VideoView.self)
            
            }, context: voidSelf);
    }
    
    func initShader() {
        
    }
    
    func bindTexture() {
        
    }
    
    override func draw(_ dirtyRect: NSRect) {
        
        glClearColor(0, 0, 0, 0);
        glClear(GLbitfield(GL_COLOR_BUFFER_BIT));
        glColor3f(1.0, 0.85, 0.35);
        glBegin(GLenum(GL_TRIANGLES));
        glVertex3f(-1.0, -1.0, 0.0);
        glVertex3f(-1.0, 1.0, 0.0);
        glVertex3f(1.0, 1.0, 0.0);
        glEnd();
        glFlush();
    }
}
