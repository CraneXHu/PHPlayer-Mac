//
//  AudioDecoder.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/14.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "AudioDecoder.hpp"
#include "spdlog/spdlog.h"
#include "PHPlayerCore.hpp"
#include "Demuxer.hpp"
#include "PacketQueue.hpp"
#include "FrameQueue.hpp"
extern "C" {
#include "libavutil/pixdesc.h"
#include "VideoToolbox.h"
}

AudioDecoder::AudioDecoder(PHPlayerCore *player):Decoder(player)
{
    
}

bool AudioDecoder::open()
{
    packetQueue = player->getDemuxer()->getAudioPacketQueue();
    AVStream *stream;
    stream = player->getDemuxer()->getAudioStream();
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
    
    ret = avcodec_open2(codecContext, codec, NULL);
    if (ret < 0) {
        return false;
    }
    return true;
}

void AudioDecoder::start()
{
    std::thread decodeThread(&AudioDecoder::decode, this);
    decodeThread.detach();
}

void AudioDecoder::decode()
{
    AVPacket *packet = 0;
    while (true) {
        
        packet = packetQueue->front();
        if (packet == 0) {
            break;
        }
        
        if (strcmp((char*)packet->data, "flush") == 0) {
            avcodec_flush_buffers(codecContext);
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
                frameQueue->push(frame);
            } else {
                av_frame_free(&frame);
            }
        }
    out:
        av_packet_free(&packet);
        
    }
}
