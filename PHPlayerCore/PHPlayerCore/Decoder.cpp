//
//  Decoder.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "Decoder.hpp"
#include "PHPlayerCore.hpp"
#include "PacketQueue.hpp"
#include "FrameQueue.hpp"
#include "Demuxer.hpp"

Decoder::Decoder(PHPlayerCore *player, DecoderType type)
{
    this->player = player;
    this->type = type;
    
    frameQueue = new FrameQueue(16);
    if (type == PH_DECODER_VIDEO) {
        packetQueue = player->getDemuxer()->getVideoPacketQueue();
    } else if(type == PH_DECODER_AUDIO) {
        packetQueue = player->getDemuxer()->getAudioPacketQueue();
    } else {
        packetQueue = player->getDemuxer()->getSubtitlePacketQueue();
    }
}

Decoder::~Decoder()
{
    delete frameQueue;
}

bool Decoder::openDecoder()
{
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
    
//    codecContext->get_format = get_format;
//    codecContext->thread_safe_callbacks = 1;
    
    ret = avcodec_open2(codecContext, codec, NULL);
    if (ret < 0) {
        return false;
    }
    return true;
}

bool Decoder::start()
{
    bool ret = openDecoder();
    if (ret == false) {
        return false;
    }
    std::thread decodeThread(&Decoder::decode, this);
    decodeThread.detach();
    return true;
}

void Decoder::decode()
{
    AVPacket *pkt = av_packet_alloc();
    while (player->getState() != PH_STATE_STOPED) {
        
        bool isSucceed = packetQueue->front(pkt);
        if (!isSucceed) {
            continue;
        }
        
        int ret = avcodec_send_packet(codecContext, pkt);
        if (ret < 0) {
            goto out;
        }
        
        while (!ret) {
            AVFrame *frame = av_frame_alloc();
            ret = avcodec_receive_frame(codecContext, frame);
            if (!ret)
            {
                frameQueue->push(frame);
            }
            av_frame_free(&frame);
        }
        out:
            av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
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
