/*
 *  PHPlayerCore.cpp
 *  PHPlayerCore
 *
 *  Created by huhexiang on 17/1/16.
 *  Copyright © 2017年 huhexiang. All rights reserved.
 *
 */

#include <iostream>
#include <thread>
#include "PHPlayerCore.hpp"
#include "Source.hpp"
#include "Demuxer.hpp"
#include "Decoder.hpp"

PHPlayerCore::PHPlayerCore()
{
    source = new Source();
    demuxer = new Demuxer(this);
    videoDecoder = new Decoder(this, PH_DECODER_VIDEO);
    audioDecoder = new Decoder(this, PH_DECODER_AUDIO);
    subtitleDecoder = new Decoder(this, PH_DECODER_SUBTITLE);
}

PHPlayerCore::~PHPlayerCore()
{
    delete source;
    delete demuxer;
    delete videoDecoder;
    delete audioDecoder;
    delete subtitleDecoder;
}

void PHPlayerCore::setState(PlayerState state)
{
    this->state = state;
}

PlayerState PHPlayerCore::getState()
{
    return state;
}

Source* PHPlayerCore::getSource()
{
    return source;
}

Demuxer* PHPlayerCore::getDemuxer()
{
    return demuxer;
}

void PHPlayerCore::seek(int64_t position)
{
    demuxer->seek(position);
}

Decoder *PHPlayerCore::getVideoDecoder()
{
    return videoDecoder;
}

Decoder *PHPlayerCore::getAudioDecoder()
{
    return audioDecoder;
}

Decoder *PHPlayerCore::getSubtitleDecoder()
{
    return subtitleDecoder;
}


//void PHPlayerCore::init()
//{
//    av_register_all();
//}
//
//bool PHPlayerCore::open(char *file)
//{
//    if (avformat_open_input(&pFormatCtx, file, NULL, NULL) < 0) {
//        return false;
//    }
//    
//    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
//        return false;
//    }
//    
//    return true;
//}
//
//void PHPlayerCore::setCallback(Callback callback, void * ctx)
//{
//    player->setCallback(callback, ctx);
//}
//
//void PHPlayerCore::start()
//{
//    std::thread demuxThread(&Player::demux, player);
//    std::thread videoThread(&Player::decodeVideo, player);
//    std::thread audioThread(&Player::decodeAudio, player);
//    std::thread displayThread(&Player::playVideo, player);
//    demuxThread.detach();
//    videoThread.detach();
//    audioThread.detach();
//    displayThread.detach();
//
//}
//
//void PHPlayerCore::getAudioBuffer(unsigned char *outData, int size)
//{
//    player->getAudioBuffer(outData, size);
//}
