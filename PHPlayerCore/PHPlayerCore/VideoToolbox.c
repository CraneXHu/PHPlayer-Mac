//
//  VideoToolbox.c
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/8.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "VideoToolbox.h"

int videotoolbox_retrieve_data(AVCodecContext *pAVCodecContext, AVFrame *pFrame)
{
    CVPixelBufferRef pixbuf = (CVPixelBufferRef)pFrame->data[3];
    OSType pixel_format = CVPixelBufferGetPixelFormatType(pixbuf);
    size_t width  = CVPixelBufferGetWidth(pixbuf);
    size_t height = CVPixelBufferGetHeight(pixbuf);
    CVReturn err;
    uint8_t *data[4] = { 0 };
    int linesize[4] = { 0 };
    int planes, ret, i;
    char codec_str[32];
    
    AVFrame *tempFrame = av_frame_alloc();
    
    switch (pixel_format) {
        case kCVPixelFormatType_420YpCbCr8Planar: tempFrame->format = AV_PIX_FMT_YUV420P; break;
        case kCVPixelFormatType_422YpCbCr8:       tempFrame->format = AV_PIX_FMT_UYVY422; break;
        case kCVPixelFormatType_32BGRA:           tempFrame->format = AV_PIX_FMT_BGRA; break;
        case kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange: tempFrame->format = AV_PIX_FMT_NV12; break;
        default:
            return AVERROR(ENOSYS);
    }
    
    tempFrame->width  = width;
    tempFrame->height = height;
    ret = av_frame_get_buffer(tempFrame, 32);
    if (ret < 0)
        return ret;
    
    err = CVPixelBufferLockBaseAddress(pixbuf, kCVPixelBufferLock_ReadOnly);
    if (err != kCVReturnSuccess) {
        av_log(NULL, AV_LOG_ERROR, "Error locking the pixel buffer.\n");
        return AVERROR_UNKNOWN;
    }
    
    if (CVPixelBufferIsPlanar(pixbuf)) {
        
        planes = CVPixelBufferGetPlaneCount(pixbuf);
        for (i = 0; i < planes; i++) {
            data[i]     = CVPixelBufferGetBaseAddressOfPlane(pixbuf, i);
            linesize[i] = CVPixelBufferGetBytesPerRowOfPlane(pixbuf, i);
        }
    } else {
        data[0] = CVPixelBufferGetBaseAddress(pixbuf);
        linesize[0] = CVPixelBufferGetBytesPerRow(pixbuf);
    }
    
    av_image_copy(tempFrame->data, tempFrame->linesize,
                  (const uint8_t **)data, linesize, tempFrame->format,
                  width, height);
    
    ret = av_frame_copy_props(tempFrame, pFrame);
    CVPixelBufferUnlockBaseAddress(pixbuf, kCVPixelBufferLock_ReadOnly);
    if (ret < 0)
        return ret;
    
    av_frame_unref(pFrame);
    av_frame_move_ref(pFrame, tempFrame);
    av_frame_free(tempFrame);
    
    return 0;
}

void videotoolbox_uninit(AVCodecContext *pAVCodecContext)
{
    av_videotoolbox_default_free(pAVCodecContext);
}

int videotoolbox_init(AVCodecContext *pAVCodecContext)
{
    int ret = 0;
    AVVideotoolboxContext *vtctx = av_videotoolbox_alloc_context();
    vtctx->cv_pix_fmt_type = kCVPixelFormatType_420YpCbCr8Planar;
    ret = av_videotoolbox_default_init2(pAVCodecContext, vtctx);
    if (ret < 0) {
        videotoolbox_uninit(pAVCodecContext);
    }
    return ret;
}
