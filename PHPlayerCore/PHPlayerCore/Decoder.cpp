//
//  Decoder.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "spdlog/spdlog.h"
#include "Decoder.hpp"
#include "PHPlayerCore.hpp"
#include "PacketQueue.hpp"
#include "FrameQueue.hpp"
#include "Demuxer.hpp"

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

Decoder::Decoder(PHPlayerCore *player, DecoderType type)
{
    this->player = player;
    this->type = type;
    
    frameQueue = new FrameQueue(16);
}

Decoder::~Decoder()
{
    delete frameQueue;
}

bool Decoder::open()
{
    if (type == PH_DECODER_VIDEO) {
        packetQueue = player->getDemuxer()->getVideoPacketQueue();
    } else if(type == PH_DECODER_AUDIO) {
        packetQueue = player->getDemuxer()->getAudioPacketQueue();
    } else {
        packetQueue = player->getDemuxer()->getSubtitlePacketQueue();
    }
    
    AVStream *stream;
    if (type == PH_DECODER_VIDEO) {
        stream = player->getDemuxer()->getVideoStream();
    } else if(type == PH_DECODER_AUDIO) {
        stream = player->getDemuxer()->getAudioStream();
    } else {
        stream = player->getDemuxer()->getSubtitleStream();
    }
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
    
    if (type == PH_DECODER_VIDEO){
//        codecContext->get_format = get_format;
        //    codecContext->get_buffer2 = get_buffer;
        //    codecContext->thread_safe_callbacks = 1;
    }
    
    ret = avcodec_open2(codecContext, codec, NULL);
    if (ret < 0) {
        return false;
    }
    return true;
}

bool Decoder::start()
{
    std::thread decodeThread(&Decoder::decode, this);
    decodeThread.detach();
    return true;
}

void Decoder::close()
{
    packetQueue->push(0);
    frameQueue->push(0);
    
//    if(type == PH_DECODER_VIDEO){
//        videotoolbox_uninit(codecContext);
//    }
//    avcodec_close(codecContext);
}

void Decoder::decode()
{
    AVPacket *packet = 0;
    while (true) {
        
        packet = packetQueue->front();
        if (packet == 0) {
            break;
        }
        
        if (packet->size == 0) {
            avcodec_flush_buffers(codecContext);
            spdlog::get("phplayer.log")->info("Flush codec buffers.");
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
//                if (type == PH_DECODER_VIDEO){
//                    videotoolbox_retrieve_data(codecContext, frame);
//                }

                frameQueue->push(frame);
            } else {
                av_frame_free(&frame);
            }
        }
    out:
        av_packet_free(&packet);
    }
}

void Decoder::clear()
{
    frameQueue->clear();
}

AVCodecContext *Decoder::getCodecContex()
{
    return codecContext;
}

FrameQueue *Decoder::getFrameQueue()
{
    return frameQueue;
}
