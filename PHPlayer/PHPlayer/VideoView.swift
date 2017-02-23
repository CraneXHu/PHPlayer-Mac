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
    
    var texName: GLuint = 0
    var pbo: GLuint = 0
    
    var displayLink: CVDisplayLink?
    
    func setVideoCallback() {

        let voidSelf = UnsafeMutableRawPointer(Unmanaged.passRetained(self).toOpaque())
        
        AppDelegate.player.videoCallback({ (userData: UnsafeMutableRawPointer?, data: UnsafeMutablePointer<UInt8>?, width: Int32, height: Int32, linesize: UnsafeMutablePointer<Int32>?) -> Void in
            
            let videoView = unsafeBitCast(userData, to: VideoView.self)
            videoView.openGLContext?.lock()
            videoView.openGLContext?.makeCurrentContext()
            glBindTexture(GLenum(GL_TEXTURE_2D), videoView.texName)
            glBindBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), videoView.pbo)
//            glBufferSubData(GLenum(GL_PIXEL_UNPACK_BUFFER), 0, GLsizeiptr(width*height*4), data)
//            glBufferData(GLenum(GL_PIXEL_UNPACK_BUFFER), 1280*720*4, data, GLenum(GL_DYNAMIC_DRAW))
            let dst = glMapBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), GLenum(GL_READ_WRITE))
            memcpy(dst, data, Int(width*height*4))
            glUnmapBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER))
            glTexSubImage2D(GLenum(GL_TEXTURE_2D), 0, 0, 0, width,
                         height, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE),
                         nil)
            videoView.openGLContext?.unlock()
            }, userData: voidSelf)
    }
    
    func initData() {
        prepareTexture()
        setVideoCallback()
        setUpDisplayLink()
        startDisplayLink()
    }
    
    func prepareTexture() {
        
        let width = AppDelegate.player.getVideoWidth()
        let height = AppDelegate.player.getVideoHeight()
        
        openGLContext?.lock()
        openGLContext?.makeCurrentContext()
        
        glClearColor (0.0, 0.0, 0.0, 0.0)
        glShadeModel(GLenum(GL_FLAT))
        glEnable(GLenum(GL_DEPTH_TEST))
        
        if texName != 0 {
            glDeleteTextures(1, &texName)
        }
        if pbo != 0 {
            glDeleteBuffers(1, &pbo)
        }
        
        glGenTextures(1, &texName)
        glBindTexture(GLenum(GL_TEXTURE_2D), texName);
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MAG_FILTER), GL_LINEAR);
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MIN_FILTER), GL_LINEAR);
        
        glGenBuffers(1, &pbo)
        glBindBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), pbo)
//        glPixelStorei(GLenum(GL_UNPACK_ALIGNMENT), 1)
        let size = width*height*4
        glBufferData(GLenum(GL_PIXEL_UNPACK_BUFFER), GLsizeiptr(size), nil, GLenum(GL_DYNAMIC_COPY))
        glTexImage2D(GLenum(GL_TEXTURE_2D), 0, GL_RGBA, width,
                     height, 0, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE),
                     nil)
        glBindBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), 0)
        glBindTexture(GLenum(GL_TEXTURE_2D), 0)
        glEnable(GLenum(GL_TEXTURE_2D));
        openGLContext?.unlock()
    }
    
    func uninit() {
        glDeleteTextures(1, &texName)
        glDeleteBuffers(1, &pbo)
    }
    
    deinit {
        uninit()
    }
    
    func initShader() {
        
    }
    
    func drawVideo() {
        openGLContext?.lock()
        openGLContext?.makeCurrentContext()
        glClear(GLenum(GL_COLOR_BUFFER_BIT) | GLenum(GL_DEPTH_BUFFER_BIT))
        glBindTexture(GLenum(GL_TEXTURE_2D), texName);
        glBegin(GLenum(GL_QUADS))
        glTexCoord2f(0.0, 0.0)
        glVertex3f(-1.0, 1.0, 0.0)
        glTexCoord2f(0.0, 1.0)
        glVertex3f(-1.0, -1.0, 0.0)
        glTexCoord2f(1.0, 1.0)
        glVertex3f(1.0, -1.0, 0.0)
        glTexCoord2f(1.0, 0.0)
        glVertex3f(1.0, 1.0, 0.0)
        glEnd()
        glFlush()
        openGLContext?.flushBuffer()
        openGLContext?.unlock()
    }
    
    private func checkCVReturn(_ value: CVReturn) {
        assert(value == kCVReturnSuccess, "CVReturn not success: \(value)")
    }
    
    private func setUpDisplayLink(){
        func displayLinkCallback(
            _ displayLink: CVDisplayLink, _ inNow: UnsafePointer<CVTimeStamp>,
            _ inOutputTime: UnsafePointer<CVTimeStamp>,
            _ flagsIn: CVOptionFlags,
            _ flagsOut: UnsafeMutablePointer<CVOptionFlags>,
            _ context: UnsafeMutableRawPointer?) -> CVReturn {
            let videoView = unsafeBitCast(context, to: VideoView.self)
           videoView.drawVideo()
            return kCVReturnSuccess
        }
        
        CVDisplayLinkCreateWithActiveCGDisplays(&displayLink)
        if let link = displayLink {
            checkCVReturn(CVDisplayLinkSetOutputCallback(link, displayLinkCallback, UnsafeMutableRawPointer(Unmanaged.passRetained(self).toOpaque())))
        } else {
            fatalError("Failed to create display link")
        }
        if let context = self.openGLContext?.cglContextObj, let format = self.pixelFormat?.cglPixelFormatObj {
            checkCVReturn(CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink!, context, format))
        } else {
            fatalError("Failed to set display with nil opengl context")
        }
    }
    
    private func startDisplayLink() {
        CVDisplayLinkStart(displayLink!)
    }
}
