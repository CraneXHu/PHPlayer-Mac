//
//  Demuxer.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

extern "C"{
#include "avformat.h"
}
#include "Demuxer.hpp"
#include "Source.hpp"
#include "PHPlayerCore.hpp"
#include "PacketQueue.hpp"

Demuxer::Demuxer(PHPlayerCore *player)
{
    this->player = player;
    
    videoPacketQueue = new PacketQueue(8);
    audioPacketQueue = new PacketQueue(8);
    subtitlePacketQueue = new PacketQueue(8);
    
    isRequestSeek = false;
    seekPosition = 0;
}

Demuxer::~Demuxer()
{
    delete videoPacketQueue;
    delete audioPacketQueue;
    delete subtitlePacketQueue;
}

bool Demuxer::start()
{
    std::thread demuxThread(&Demuxer::demux, this);
    demuxThread.detach();
    return true;
}

void Demuxer::demux()
{
    AVFormatContext *formatContext = player->getSource()->getContext();
    AVPacket *packet = av_packet_alloc();
    
    while (1) {
        
        //used for network stream
//        if (pauseReq) {
//            av_read_pause(pFormatCtx);
//        }
//        if (playReq) {
//            av_read_play(pFormatCtx);
//        }
        
        if (isRequestSeek) {
            av_seek_frame(formatContext, -1, seekPosition*AV_TIME_BASE, AVSEEK_FLAG_ANY);
            isRequestSeek = false;
        }
        int ret = av_read_frame(formatContext, packet);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        }
        if (ret < 0) {
            break;
        }
        
        if (packet->stream_index == videoStream->index) {
            videoPacketQueue->push(packet);
        } else if(packet->stream_index == audioStream->index){
            audioPacketQueue->push(packet);
        } else if(packet->stream_index == subtitleStream->index){
            subtitlePacketQueue->push(packet);
        }
        av_packet_unref(packet);
    }
    
    av_packet_free(&packet);
}

void Demuxer::seek(int64_t position)
{
    this->seekPosition = position;
    isRequestSeek = true;
}

AVStream *Demuxer::getVideoStream()
{
    AVFormatContext *formatContext = player->getSource()->getContext();
    int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        return 0;
    }
    videoStream = formatContext->streams[streamIndex];
    return videoStream;
}

AVStream *Demuxer::getAudioStream()
{
    AVFormatContext *formatContext = player->getSource()->getContext();
    int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        return 0;
    }
    audioStream = formatContext->streams[streamIndex];
    return audioStream;
}

AVStream *Demuxer::getSubtitleStream()
{
    AVFormatContext *formatContext = player->getSource()->getContext();
    int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_SUBTITLE, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        return 0;
    }
    subtitleStream = formatContext->streams[streamIndex];
    return subtitleStream;
}

PacketQueue *Demuxer::getVideoPacketQueue()
{
    return videoPacketQueue;
}

PacketQueue *Demuxer::getAudioPacketQueue()
{
    return audioPacketQueue;
}

PacketQueue *Demuxer::getSubtitlePacketQueue()
{
    return subtitlePacketQueue;
}
