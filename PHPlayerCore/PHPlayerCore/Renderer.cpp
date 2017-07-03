//
//  Renderer.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "Renderer.hpp"
#include "PHPlayerCore.hpp"
#include "Demuxer.hpp"
#include "Decoder.hpp"
#include "SubtitleDecoder.hpp"
#include "SubtitleRenderer.hpp"
#include "FrameQueue.hpp"

extern "C"{
#include "swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
}

Renderer::Renderer(PHPlayerCore *player)
{
    this->player = player;
    userData = 0;
    videoCallback = 0;
    audioClock = 0;
}

Renderer::~Renderer()
{
    
}

void Renderer::start()
{
    std::thread renderVideoThread(&Renderer::renderVideo, this);
    renderVideoThread.detach();
}

void Renderer::play()
{
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one();
}

void Renderer::setVideoCallback(void *userData, VideoCallback callback)
{
    this->userData = userData;
    videoCallback = callback;
}

double Renderer::getAudioClock()
{
    return audioClock;
}

void Renderer::setAudioClock(double clock)
{
    audioClock = clock;
}

void Renderer::subOverlay(AVSubtitle *sub, AVFrame *frame)
{
    if (sub->format == 0) {
        int n = sub->num_rects;
        for (int i = 0; i < n; i++) {
            AVSubtitleRect*rect = sub->rects[i];
        }
    }
}

void Renderer::renderVideo()
{
    AVFrame *frame = NULL;
    AVFrame *rgbaFrame = av_frame_alloc();
    AVCodecContext *codecContext = player->getDemuxer()->getVideoDecoder()->getCodecContex();
    rgbaFrame->width = codecContext->width;
    rgbaFrame->height = codecContext->height;
    rgbaFrame->format = AV_PIX_FMT_RGBA;
    
    int size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, codecContext->width, codecContext->height, 1);
    unsigned char *outBuffer = (unsigned char *)av_malloc(size);
    av_image_fill_arrays(rgbaFrame->data, rgbaFrame->linesize, outBuffer, AV_PIX_FMT_RGBA, codecContext->width, codecContext->height, 1);
    
    SwsContext *imageConvertContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, codecContext->width, codecContext->height, AV_PIX_FMT_RGBA, SWS_BILINEAR, NULL, NULL, NULL);
    
    while (true) {
        
        std::unique_lock<std::mutex> lock(mutex);
        while (player->getState() == PH_STATE_PAUSED) {
            cv.wait(lock);
        }
        
        frame = player->getDemuxer()->getVideoDecoder()->getFrameQueue()->front();
        if (frame == 0) {
            break;
        }
        double timestamp;
        timestamp = av_frame_get_best_effort_timestamp(frame)*av_q2d(player->getDemuxer()->getVideoStream()->time_base);
        if (timestamp > audioClock) {
            if (timestamp - audioClock < 0.1) {
                std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)((timestamp - audioClock)*1000)));
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
        sws_scale(imageConvertContext, (uint8_t const * const *)frame->data, frame->linesize, 0, codecContext->height, rgbaFrame->data, rgbaFrame->linesize);
        if (player->getDemuxer()->getSubtitleDecoder()) {
            AVSubtitle * sub = player->getDemuxer()->getSubtitleDecoder()->getSubtitleQueue()->front();
            if (sub) {
//                subOverlay(sub, frame);
                av_free(sub);
            }
        }
        av_frame_free(&frame);
        if (videoCallback) {
            videoCallback(userData, (unsigned char *)rgbaFrame->data[0], rgbaFrame->width, rgbaFrame->height, rgbaFrame->linesize);
        }
    }
    av_free(outBuffer);
    av_frame_free(&rgbaFrame);
}

void Renderer::renderAudio(unsigned char* outData, int *size)
{
    AVFrame * frame = NULL;
    frame = player->getDemuxer()->getAudioDecoder()->getFrameQueue()->front();
    if (frame == 0) {
        return;
    }
    audioClock = frame->pts*av_q2d(player->getDemuxer()->getAudioStream()->time_base);
    if (frame->channels > 0 && frame->channel_layout == 0){
        frame->channel_layout = av_get_default_channel_layout(frame->channels);
    }
    else if (frame->channels == 0 && frame->channel_layout > 0){
        frame->channels = av_get_channel_layout_nb_channels(frame->channel_layout);
    }
    int64_t dstLayout = av_get_default_channel_layout(frame->channels);
    SwrContext *swrCtx = swr_alloc_set_opts(NULL, dstLayout, AV_SAMPLE_FMT_S16, frame->sample_rate, frame->channel_layout, (AVSampleFormat)frame->format, frame->sample_rate, 0, NULL);
    if (swr_init(swrCtx) < 0){
        return ;
    }
    int dstSamples = av_rescale_rnd(swr_get_delay(swrCtx, frame->sample_rate) + frame->nb_samples, frame->sample_rate, frame->sample_rate, AVRounding(1));
    int number = swr_convert(swrCtx, &outData, dstSamples, (const uint8_t**)frame->data, frame->nb_samples);
    int dataSize = frame->channels * number * av_get_bytes_per_sample(AV_SAMPLE_FMT_S16);
    *size = dataSize;
    swr_free(&swrCtx);
    av_frame_free(&frame);
}
