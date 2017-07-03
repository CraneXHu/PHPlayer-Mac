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
#include "spdlog/spdlog.h"
#include "PHPlayerCore.hpp"
#include "Demuxer.hpp"
#include "VideoDecoder.hpp"
#include "Renderer.hpp"
#include "SubtitleRenderer.hpp"
#include "AudioRenderer.hpp"
extern "C"{
#include "avformat.h"
#include "avcodec.h"
}

void av_log_callback(void* avcl, int level, const char *fmt, va_list vl)
{
    char msg[1024];
    sprintf(msg, fmt, vl);
    spdlog::get("ffmpeg")->info(msg);
}

PHPlayerCore::PHPlayerCore()
{
    demuxer = new Demuxer(this);
    renderer = new Renderer(this);
    subRenderer = new SubtitleRenderer(this);
    audioRenderer = new AudioRenderer(this);
    init();
}

PHPlayerCore::~PHPlayerCore()
{
    delete demuxer;
    delete renderer;
}

void PHPlayerCore::init()
{
    av_register_all();
    avformat_network_init();
//    av_log_set_callback(av_log_callback);
    spdlog::basic_logger_st("phplayer", "phplayer.log");
    spdlog::basic_logger_st("ffmpeg", "ffmpeg.log");
    spdlog::get("phplayer")->info("init");
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
    demuxer->open(file);
    audioRenderer->init();
    return true;
}

void PHPlayerCore::start()
{
    demuxer->start();
    renderer->start();
    audioRenderer->play();
}

void PHPlayerCore::pause()
{
    setState(PH_STATE_PAUSED);
    audioRenderer->pause();
}

void PHPlayerCore::play()
{
    setState(PH_STATE_RUNNING);
    renderer->play();
    audioRenderer->play();
}

void PHPlayerCore::stop()
{
    setState(PH_STATE_STOPED);
    audioRenderer->stop();
}

void PHPlayerCore::seek(double position, int flag)
{
    renderer->setAudioClock(position);
    demuxer->seek(position, flag);
}

void PHPlayerCore::forward(double duration)
{
    double current = renderer->getAudioClock();
    seek(duration + current, 0);
}

void PHPlayerCore::backward(double duration)
{
    double current = renderer->getAudioClock();
    seek(-duration + current, 1);
}

void PHPlayerCore::setEnableHardwareAcceleration(bool isEnable)
{
    ((VideoDecoder*)demuxer->getVideoDecoder())->setEnableHardwareAcceleration(isEnable);
}

bool PHPlayerCore::isEnableHardwareAcceleration()
{
    return ((VideoDecoder*)demuxer->getVideoDecoder())->isEnableHardwareAcceleration();
}

void PHPlayerCore::setVolume(float volume)
{
    audioRenderer->setPlaybackVolume(volume);
}

//void PHPlayerCore::clear()
//{
//    demuxer->clear();
//}

void PHPlayerCore::setVideoCallback(void *userData, VideoCallback callback)
{
    renderer->setVideoCallback(userData, callback);
}

void PHPlayerCore::getAudioData(unsigned char *outData, int *size)
{
    renderer->renderAudio(outData, size);
}

int PHPlayerCore::getVideoWidth()
{
    return demuxer->getVideoWidth();
}

int PHPlayerCore::getVideoHeight()
{
    return demuxer->getVideoHeight();
}

int PHPlayerCore::getAudioSampleRate()
{
    return demuxer->getAudioSampleRate();
}

int PHPlayerCore::getAudioChannels()
{
    return demuxer->getAudioChannels();
}

double PHPlayerCore::getDuration()
{
    return demuxer->getDuration();
}

double PHPlayerCore::getCurrentTime()
{
    return renderer->getAudioClock();
}

char *PHPlayerCore::getFileName()
{
    return demuxer->getFileName();
}

Demuxer* PHPlayerCore::getDemuxer()
{
    return demuxer;
}

Renderer *PHPlayerCore::getRender()
{
    return renderer;
}

SubtitleRenderer *PHPlayerCore::getSubRender()
{
    return subRenderer;
}

