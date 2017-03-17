//
//  VideoDecoder.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "VideoDecoder.hpp"
#include "spdlog/spdlog.h"
#include "PHPlayerCore.hpp"
#include "Demuxer.hpp"
#include "PacketQueue.hpp"
#include "FrameQueue.hpp"
extern "C" {
#include "libavutil/pixdesc.h"
#include "VideoToolbox.h"
}

static enum AVPixelFormat get_format(AVCodecContext *s, const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p;
    for(p = pix_fmts; *p != -1; p++){
        const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(*p);
        if (!(desc->flags & AV_PIX_FMT_FLAG_HWACCEL)){
            break;
        }
        if (*p == AV_PIX_FMT_VIDEOTOOLBOX){
            videotoolbox_init(s);
            break;
        }
    }
    return *p;
}

static int get_buffer(AVCodecContext *s, AVFrame *frame, int flags)
{
    return avcodec_default_get_buffer2(s, frame, flags);
}

VideoDecoder::VideoDecoder(PHPlayerCore *player):Decoder(player)
{
    
}

bool VideoDecoder::open()
{
    packetQueue = player->getDemuxer()->getVideoPacketQueue();
    AVStream *stream;
    stream = player->getDemuxer()->getVideoStream();
    if (stream == NULL) {
        return false;
    }
    
    AVCodec *codec = avcodec_find_decoder(stream->codecpar->codec_id);
    if (!codec) {
        return false;
    }
    
    codecContext = avcodec_alloc_context3(codec);
    if (!codecContext) {
        return false;
    }
    
    int ret = 0;
    if ((ret = avcodec_parameters_to_context(codecContext, stream->codecpar)) < 0) {
        return false;
    }
    
    if (hardwareAcceleration){
        codecContext->get_format = get_format;
        codecContext->get_buffer2 = get_buffer;
        codecContext->thread_safe_callbacks = 1;
    }
    
    ret = avcodec_open2(codecContext, codec, NULL);
    if (ret < 0) {
        return false;
    }
    return true;
}

void VideoDecoder::start()
{
    std::thread decodeThread(&VideoDecoder::decode, this);
    decodeThread.detach();
}

void VideoDecoder::decode()
{
    AVPacket *packet = 0;
    while (true) {
        
        packet = packetQueue->front();
        if (packet == 0) {
            break;
        }
        
        if (strcmp((char*)packet->data, "flush") == 0) {
            if (!hardwareAcceleration) {
                avcodec_flush_buffers(codecContext);
            }
            av_packet_free(&packet);
            spdlog::get("phplayer")->info("Flush codec buffers.");
            continue;
        }
        
        int ret = avcodec_send_packet(codecContext, packet);
        if (ret < 0) {
            goto out;
        }
        
        while (!ret) {
            AVFrame *frame = av_frame_alloc();
            ret = avcodec_receive_frame(codecContext, frame);
            if (!ret)
            {
                if (hardwareAcceleration){
                    videotoolbox_retrieve_data(codecContext, frame);
                }
                
                frameQueue->push(frame);
            } else {
                av_frame_free(&frame);
            }
        }
    out:
        av_packet_free(&packet);
        
    }
    
    if(hardwareAcceleration){
        videotoolbox_uninit(codecContext);
    }
}

void VideoDecoder::setEnableHardwareAcceleration(bool isEnable)
{
    hardwareAcceleration = isEnable;
}

bool VideoDecoder::isEnableHardwareAcceleration()
{
    return hardwareAcceleration;
}
