//
//  Player.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/17.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "Player.hpp"

Player::Player():pFormatCtx(0)
{
    av_register_all();
};

bool Player::open(char *file)
{
    if (avformat_open_input(&pFormatCtx, file, NULL, NULL) < 0) {
        return false;
    }
    
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
        return false;
    }
    
    int streamIndex = 0;
    AVStream *pAVStream = 0;
    int ret = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (ret < 0) {
        return false;
    } else {
        videoStreamIndex = ret;
        pAVStream = pFormatCtx->streams[videoStreamIndex];
        
        AVCodec *dec = avcodec_find_decoder(pAVStream->codecpar->codec_id);
        if (!dec) {
            return false;
        }
        
        pVideoCodecCtx = avcodec_alloc_context3(dec);
        if (!pVideoCodecCtx) {
            return false;
        }
        
        if ((ret = avcodec_parameters_to_context(pVideoCodecCtx, pAVStream->codecpar)) < 0) {
            return false;
        }
    }
    
    ret = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (ret < 0) {
        return false;
    } else {
        audioStreamIndex = ret;
        pAVStream = pFormatCtx->streams[audioStreamIndex];
        
        AVCodec *dec = avcodec_find_decoder(pAVStream->codecpar->codec_id);
        if (!dec) {
            return false;
        }
        
        pAudioCodecCtx = avcodec_alloc_context3(dec);
        if (!pAudioCodecCtx) {
            return false;
        }
        
        if ((ret = avcodec_parameters_to_context(pAudioCodecCtx, pAVStream->codecpar)) < 0) {
            return false;
        }
    }

    return true;
};

void Player::demux()
{
    AVPacket pkt;
    av_init_packet(&pkt);
    
    int ret = 0;
    while (1) {
        ret = av_read_frame(pFormatCtx, &pkt);
        if (ret == AVERROR(EAGAIN)) {
            continue;
        }
        if (ret < 0) {
            break;
        }
        if (pkt.stream_index == videoStreamIndex) {
            videoPacketQueue.push(pkt);
        } else if(pkt.stream_index == audioStreamIndex){
            audioPacketQueue.push(pkt);
        }
        av_packet_unref(&pkt);
    }
}

void Player::decodeVideo()
{
    AVPacket pkt;
    AVFrame *pFrame = av_frame_alloc();
    while (videoPacketQueue.front(pkt)) {
        int gotFrame = 0;
        int ret = decodeVideo(&pkt, pFrame, &gotFrame);

    }
    
}

int Player::decodeVideo(AVPacket *pPacket, AVFrame *pFrame,int *gotFrame)
{
    *gotFrame = 0;
    int ret = avcodec_send_packet(pVideoCodecCtx, pPacket);
    if (ret < 0 && ret != AVERROR_EOF) {
        return ret;
    }
    ret = avcodec_receive_frame(pVideoCodecCtx, pFrame);
    if (ret < 0 && ret != AVERROR(EAGAIN)) {
        return ret;
    }
    if (ret >= 0) {
        *gotFrame = 1;
    }
    return 0;
}

void Player::decodeAudio()
{
    
}

int Player::decodeAudio(AVPacket *pPacket, AVFrame *pFrame,int *gotFrame)
{
    int ret = avcodec_send_packet(pAudioCodecCtx, pPacket);
    if (ret < 0 && ret != AVERROR_EOF) {
        return ret;
    }
    while (1) {
        ret = avcodec_receive_frame(pAudioCodecCtx, pFrame);
        if (ret < 0 && ret != AVERROR(EAGAIN)) {
            return ret;
        }
    }
    return 0;
}
