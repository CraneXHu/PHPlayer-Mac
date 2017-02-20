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
    var pboId: GLuint = 0
    var obj: CPPWrapper?
    
//    override init(frame: CGRect) {
//        // init context
//        let attributes: [NSOpenGLPixelFormatAttribute] = [
//            UInt32(NSOpenGLPFADoubleBuffer),
//            UInt32(NSOpenGLPFAAccelerated),
//            UInt32(NSOpenGLPFAOpenGLProfile), UInt32(NSOpenGLProfileVersion3_2Core),
//            0
//        ]
//        let desentAttributes: [NSOpenGLPixelFormatAttribute] = [
//            UInt32(NSOpenGLPFADoubleBuffer),
//            UInt32(NSOpenGLPFAOpenGLProfile), UInt32(NSOpenGLProfileVersion3_2Core),
//            0
//        ]
//        
//        let pixelFormat = NSOpenGLPixelFormat(attributes: attributes) ?? NSOpenGLPixelFormat(attributes: desentAttributes)
//        
//        super.init(frame: frame, pixelFormat: pixelFormat!)!
//        
//        guard openGLContext != nil else {
//            return
//        }
//        
//        initTexture()
//    }
//    
//    required init?(coder: NSCoder) {
//        fatalError("init(coder:) has not been implemented")
//    }
    
    var displayLink: CVDisplayLink?
    
    var lastTime: Int64 = 0
    
    func setObj(obj: CPPWrapper) {
        self.obj = obj
    }
    
    func setUpCallback(obj: CPPWrapper) {

        let voidSelf = UnsafeMutableRawPointer(Unmanaged.passRetained(self).toOpaque())
        
        obj.setCallback({ (ctx: UnsafeMutableRawPointer?, data: UnsafeMutablePointer<UInt8>?, width: Int32, height: Int32, linesize: UnsafeMutablePointer<Int32>?) -> Void in
            
            let videoView = unsafeBitCast(ctx, to: VideoView.self)
            videoView.openGLContext?.lock()
            videoView.openGLContext?.makeCurrentContext()
            glBindTexture(GLenum(GL_TEXTURE_2D), videoView.texName)
            glBindBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), videoView.pboId)
//            glBufferSubData(GLenum(GL_PIXEL_UNPACK_BUFFER), 0, GLsizeiptr(width*height*4), data)
//            glBufferData(GLenum(GL_PIXEL_UNPACK_BUFFER), 1280*720*4, data, GLenum(GL_DYNAMIC_DRAW))
            let dst = glMapBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), GLenum(GL_READ_WRITE))
            memcpy(dst, data, Int(width*height*4))
            glUnmapBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER))
            glTexSubImage2D(GLenum(GL_TEXTURE_2D), 0, 0, 0, width,
                         height, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE),
                         nil)
            videoView.openGLContext?.unlock()
//            let interval: TimeInterval = NSDate.timeIntervalSinceReferenceDate
//            let millisecond = CLongLong(round(interval*1000))
//            Swift.print(millisecond)
//            if videoView.lastTime == 0 {
//                videoView.lastTime = millisecond
//            } else {
//                if millisecond - videoView.lastTime > 100{
//                    videoView.lastTime = millisecond
//                } else {
//                    videoView.lastTime = millisecond
//                }
//            }
            
            }, context: voidSelf)
    }
    
    func initTexture() {
        
        glClearColor (0.0, 0.0, 0.0, 0.0)
        glShadeModel(GLenum(GL_FLAT))
        glEnable(GLenum(GL_DEPTH_TEST))
        glGenTextures(1, &texName)
        glBindTexture(GLenum(GL_TEXTURE_2D), texName);
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MAG_FILTER), GL_LINEAR);
        glTexParameteri(GLenum(GL_TEXTURE_2D), GLenum(GL_TEXTURE_MIN_FILTER), GL_LINEAR);
        
        glGenBuffers(1, &pboId)
        glBindBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), pboId)
//        glPixelStorei(GLenum(GL_UNPACK_ALIGNMENT), 1)
        glBufferData(GLenum(GL_PIXEL_UNPACK_BUFFER), 1280*720*4, nil, GLenum(GL_DYNAMIC_COPY))
        glTexImage2D(GLenum(GL_TEXTURE_2D), 0, GL_RGBA, 1280,
                     720, 0, GLenum(GL_RGBA), GLenum(GL_UNSIGNED_BYTE),
                     nil)
        glBindBuffer(GLenum(GL_PIXEL_UNPACK_BUFFER), 0)
        glBindTexture(GLenum(GL_TEXTURE_2D), 0)
        glEnable(GLenum(GL_TEXTURE_2D));
    }
    
    func initShader() {
        
    }
    
    func bindTexture() {
        
    }
    
    override func prepareOpenGL() {
        super.prepareOpenGL()
//        openGLContext?.makeCurrentContext()
        initTexture()
        setUpCallback(obj: self.obj!)
        setUpDisplayLink()
        startDisplayLink()
    }
    
//    override func draw(_ dirtyRect: NSRect) {
//        
//        openGLContext?.lock()
//        super.draw(dirtyRect)
//        glColor3f(1.0, 0.85, 0.35);
//        glClear(GLenum(GL_COLOR_BUFFER_BIT) | GLenum(GL_DEPTH_BUFFER_BIT))
//        glBindTexture(GLenum(GL_TEXTURE_2D), texName);
//        glBegin(GLenum(GL_QUADS))
//        glTexCoord2f(0.0, 0.0)
//        glVertex3f(-1.0, 1.0, 0.0)
//        glTexCoord2f(0.0, 1.0)
//        glVertex3f(-1.0, -1.0, 0.0)
//        glTexCoord2f(1.0, 1.0)
//        glVertex3f(1.0, -1.0, 0.0)
//        glTexCoord2f(1.0, 0.0)
//        glVertex3f(1.0, 1.0, 0.0)
//        glEnd();
//        glFlush();
//        openGLContext?.unlock()
//    }
    
    func drawVideo() {
        openGLContext?.lock()
        openGLContext?.makeCurrentContext()
        glColor3f(1.0, 0.85, 0.35);
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
