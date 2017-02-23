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
    
    videoPacketQueue = new PacketQueue(64);
    audioPacketQueue = new PacketQueue(256);
    subtitlePacketQueue = new PacketQueue(16);
    
    videoStream = NULL;
    audioStream = NULL;
    subtitleStream = NULL;
    isRequestSeek = false;
    seekPosition = 0;
}

Demuxer::~Demuxer()
{
    delete videoPacketQueue;
    delete audioPacketQueue;
    delete subtitlePacketQueue;
}

void Demuxer::findStream()
{
    AVFormatContext *formatContext = player->getSource()->getContext();
    int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        videoStream = 0;
    } else {
        videoStream = formatContext->streams[streamIndex];
    }
    
    formatContext = player->getSource()->getContext();
    streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        audioStream = NULL;
    } else {
        audioStream = formatContext->streams[streamIndex];
    }
    
    formatContext = player->getSource()->getContext();
    streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_SUBTITLE, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        subtitleStream = NULL;
    } else {
        subtitleStream = formatContext->streams[streamIndex];
    }
}

bool Demuxer::start()
{
//    findStream();
    std::thread demuxThread(&Demuxer::demux, this);
    demuxThread.detach();
    return true;
}

void Demuxer::demux()
{
    int videoStreamIndex = -1;
    int audioStreamIndex = -1;
    int subtitleStreamIndex = -1;
    if (videoStream) {
        videoStreamIndex = videoStream->index;
    }
    if (audioStream) {
        audioStreamIndex = audioStream->index;
    }
    if (subtitleStream) {
        subtitleStreamIndex = subtitleStream->index;
    }
    
    AVFormatContext *formatContext = player->getSource()->getContext();
    AVPacket *packet = av_packet_alloc();
    while (player->getState() != PH_STATE_STOPED) {
        
        //used for network stream
        if (player->getState() == PH_STATE_PAUSED) {
            av_read_pause(formatContext);
        } else {
            av_read_play(formatContext);
        }
        
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
        
        if (packet->stream_index == videoStreamIndex) {
            videoPacketQueue->push(packet);
        } else if(packet->stream_index == audioStreamIndex){
            audioPacketQueue->push(packet);
        } else if(packet->stream_index == subtitleStreamIndex){
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

void Demuxer::clear()
{
    videoPacketQueue->clear();
    audioPacketQueue->clear();
    subtitlePacketQueue->clear();
}

AVStream *Demuxer::getVideoStream()
{
    return videoStream;
}

AVStream *Demuxer::getAudioStream()
{
    return audioStream;
}

AVStream *Demuxer::getSubtitleStream()
{
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
