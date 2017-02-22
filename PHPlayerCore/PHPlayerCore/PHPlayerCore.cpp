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
#include "Render.hpp"

extern "C" {
#include "avcodec.h"
}

PHPlayerCore::PHPlayerCore()
{
    source = new Source();
    demuxer = new Demuxer(this);
    videoDecoder = new Decoder(this, PH_DECODER_VIDEO);
    audioDecoder = new Decoder(this, PH_DECODER_AUDIO);
    subtitleDecoder = new Decoder(this, PH_DECODER_SUBTITLE);
    render = new Render(this);
}

PHPlayerCore::~PHPlayerCore()
{
    delete source;
    delete demuxer;
    delete videoDecoder;
    delete audioDecoder;
    delete subtitleDecoder;
    delete render;
}

void PHPlayerCore::init()
{
    Source::init();
}

void PHPlayerCore::setState(PlayerState state)
{
    this->state = state;
}

PlayerState PHPlayerCore::getState()
{
    return state;
}

bool PHPlayerCore::open(char *file)
{
    return source->open(file);
}

void PHPlayerCore::start()
{
    demuxer->start();
    videoDecoder->start();
    audioDecoder->start();
    subtitleDecoder->start();
    render->start();
}

void PHPlayerCore::pause()
{
    
}

void PHPlayerCore::stop()
{
    
}

void PHPlayerCore::setVideoCallback(void *userData, VideoCallback callback)
{
    render->setVideoCallback(userData, callback);
}

void PHPlayerCore::getAudioData(unsigned char *outData, int size)
{
    render->renderAudio(outData, size);
}

int PHPlayerCore::getVideoWidth()
{
    return videoDecoder->getCodecContex()->width;
}

int PHPlayerCore::getVideoHeight()
{
    return videoDecoder->getCodecContex()->height;
}

int PHPlayerCore::getAudioSampleRate()
{
    return audioDecoder->getCodecContex()->sample_rate;
}

int PHPlayerCore::getAudioChannels()
{
    return audioDecoder->getCodecContex()->channels;
}

Source* PHPlayerCore::getSource()
{
    return source;
}

Demuxer* PHPlayerCore::getDemuxer()
{
    return demuxer;
}

void PHPlayerCore::seek(__int64_t position)
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

