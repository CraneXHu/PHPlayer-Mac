//
//  Demuxer.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "spdlog/spdlog.h"
#include "Demuxer.hpp"
#include "globalenum.h"
#include "VideoDecoder.hpp"
#include "AudioDecoder.hpp"
#include "SubtitleDecoder.hpp"
#include "Renderer.hpp"
#include "PHPlayerCore.hpp"
#include "PacketQueue.hpp"
extern "C"{
#include "avformat.h"
}

Demuxer::Demuxer(PHPlayerCore *player)
{
    this->player = player;
    
    videoPacketQueue = new PacketQueue(64);
    audioPacketQueue = new PacketQueue(256);
    subtitlePacketQueue = new PacketQueue(16);
    
    videoStream = nullptr;
    audioStream = nullptr;
    subtitleStream = nullptr;
    isRequestSeek = false;
    seekPosition = 0;
    flag = 0;
    
    videoDecoder = nullptr;
    audioDecoder = nullptr;
    subtitleDecoder = nullptr;
}

Demuxer::~Demuxer()
{
    delete videoPacketQueue;
    delete audioPacketQueue;
    delete subtitlePacketQueue;
    
    if (videoDecoder != nullptr) {
        delete videoDecoder;
    }
    
    if (audioDecoder != nullptr) {
        delete audioDecoder;
    }
    
    if (subtitleDecoder != nullptr) {
        delete subtitleDecoder;
    }
}

bool Demuxer::open(char *url)
{
    if (avformat_open_input(&formatContext, url, NULL, NULL) < 0) {
        return false;
    }
    
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        return false;
    }
    findStream();
    
    if (videoStream) {
        videoDecoder = new VideoDecoder(player);
        bool res = videoDecoder->open();
        if (res) {
            videoDecoder->start();
        }
    }
    if (audioStream) {
        audioDecoder = new AudioDecoder(player);
        bool res = audioDecoder->open();
        if (res) {
            audioDecoder->start();
        }
    }

    if (subtitleStream) {
        subtitleDecoder = new SubtitleDecoder(player);
        bool res = subtitleDecoder->open();
        if (res) {
            subtitleDecoder->start();
        }
    }
    return true;
}

void Demuxer::findStream()
{
    int streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        videoStream = NULL;
    } else {
        videoStream = formatContext->streams[streamIndex];
    }
    
    streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        audioStream = NULL;
    } else {
        audioStream = formatContext->streams[streamIndex];
    }
    
    streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_SUBTITLE, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        subtitleStream = NULL;
    } else {
        subtitleStream = formatContext->streams[streamIndex];
    }
}

bool Demuxer::start()
{
    std::thread demuxThread(&Demuxer::demux, this);
    demuxThread.detach();
    
    return true;
}

void Demuxer::close()
{
    clear();
    if (videoDecoder) {
        videoDecoder->close();
    }
    if (audioDecoder) {
        audioDecoder->close();
    }
    if (subtitleDecoder) {
        subtitleDecoder->close();
    }
    avformat_close_input(&formatContext);
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
    
    while (player->getState() != PH_STATE_STOPED) {
        
        //used for network stream
        if (player->getState() == PH_STATE_PAUSED) {
            av_read_pause(formatContext);
        } else {
            av_read_play(formatContext);
        }
        
        if (isRequestSeek) {
            int ret = av_seek_frame(formatContext, -1, seekPosition*AV_TIME_BASE, flag);
//            int64_t pos = seekPosition*AV_TIME_BASE;
//            int ret = avformat_seek_file(formatContext, -1, pos-100, pos, pos+100, flag);
            if (ret < 0) {
                spdlog::get("phplayer")->info("Error while seeking.");
                continue;
            }
            isRequestSeek = false;
            
            clear();
            //put flush packet
            flush();
        }
        AVPacket *packet = av_packet_alloc();
        int ret = av_read_frame(formatContext, packet);
        if (ret == AVERROR(EAGAIN)) {
            av_packet_free(&packet);
            continue;
        }
        if (ret < 0) {
            close();
            break;
        }
        
        if (packet->stream_index == videoStreamIndex) {
            videoPacketQueue->push(packet);
        } else if(packet->stream_index == audioStreamIndex){
            audioPacketQueue->push(packet);
        } else if(packet->stream_index == subtitleStreamIndex){
            subtitlePacketQueue->push(packet);
        }
    }
    close();
    
}

void Demuxer::seek(double position, int flag)
{
    this->seekPosition = position;
    isRequestSeek = true;
    this->flag = flag;
}

void Demuxer::clear()
{
    videoPacketQueue->clear();
    audioPacketQueue->clear();
    subtitlePacketQueue->clear();
    
    if (videoDecoder) {
        videoDecoder->clear();
    }
    if (audioDecoder) {
        audioDecoder->clear();
    }
    if (subtitleDecoder) {
        subtitleDecoder->clear();
    }
}

void Demuxer::flush()
{
    AVPacket *packet = av_packet_alloc();
    char *msg = (char *)av_malloc(6);
    strcpy(msg, "flush");
    packet->data = (uint8_t*)msg;
    packet->size = 6;
    videoPacketQueue->push(packet);
    
    packet = av_packet_alloc();
    msg = (char *)av_malloc(6);
    strcpy(msg, "flush");
    packet->data = (uint8_t*)msg;
    packet->size = 6;
    audioPacketQueue->push(packet);
    
    packet = av_packet_alloc();
    msg = (char *)av_malloc(6);
    strcpy(msg, "flush");
    packet->data = (uint8_t*)msg;
    packet->size = 6;
    subtitlePacketQueue->push(packet);
//    videoDecoder->flush();
//    audioDecoder->flush();
//    subtitleDecoder->flush();
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

Decoder *Demuxer::getVideoDecoder()
{
    return videoDecoder;
}

Decoder *Demuxer::getAudioDecoder()
{
    return audioDecoder;
}

SubtitleDecoder *Demuxer::getSubtitleDecoder()
{
    return subtitleDecoder;
}

int Demuxer::getVideoWidth()
{
    return videoDecoder->getCodecContex()->width;
}

int Demuxer::getVideoHeight()
{
    return videoDecoder->getCodecContex()->height;
}

int Demuxer::getAudioSampleRate()
{
    return audioDecoder->getCodecContex()->sample_rate;
}

int Demuxer::getAudioChannels()
{
    return audioDecoder->getCodecContex()->channels;
}

double Demuxer::getDuration()
{
    return formatContext->duration/AV_TIME_BASE;
}

char *Demuxer::getFileName()
{
    return strrchr(formatContext->filename, '/')+1;
}
