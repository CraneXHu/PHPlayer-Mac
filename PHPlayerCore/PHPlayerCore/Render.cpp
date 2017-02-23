//
//  Render.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "Render.hpp"
#include "PHPlayerCore.hpp"
#include "Demuxer.hpp"
#include "Decoder.hpp"
#include "FrameQueue.hpp"

extern "C"{
#include "swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
}

Render::Render(PHPlayerCore *player)
{
    this->player = player;
    userData = 0;
    videoCallback = 0;
    audioClock = 0;
}

Render::~Render()
{
    
}

void Render::start()
{
    std::thread renderVideoThread(&Render::renderVideo, this);
    renderVideoThread.detach();
}

void Render::setVideoCallback(void *userData, VideoCallback callback)
{
    this->userData = userData;
    videoCallback = callback;
}

void Render::renderVideo()
{
    AVFrame *frame = av_frame_alloc();
    AVFrame *rgbaFrame = av_frame_alloc();
    AVCodecContext *codecContext = player->getVideoDecoder()->getCodecContex();
    rgbaFrame->width = codecContext->width;
    rgbaFrame->height = codecContext->height;
    rgbaFrame->format = AV_PIX_FMT_RGBA;
    
    int size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, codecContext->width, codecContext->height, 1);
    unsigned char *outBuffer = (unsigned char *)av_malloc(size);
    av_image_fill_arrays(rgbaFrame->data, rgbaFrame->linesize, outBuffer, AV_PIX_FMT_RGBA, codecContext->width, codecContext->height, 1);
    
    SwsContext *imageConvertContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, codecContext->width, codecContext->height, AV_PIX_FMT_RGBA, SWS_BILINEAR, NULL, NULL, NULL);
    
    while (1) {
        
        bool ret = player->getVideoDecoder()->getFrameQueue()->front(&frame);
        if (ret == false) {
            continue;
        }
        double timestamp;
        timestamp = av_frame_get_best_effort_timestamp(frame)*av_q2d(player->getDemuxer()->getVideoStream()->time_base);
        if (timestamp > audioClock) {
            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)((timestamp - audioClock)*1000)));
        }
        sws_scale(imageConvertContext, (uint8_t const * const *)frame->data, frame->linesize, 0, codecContext->height, rgbaFrame->data, rgbaFrame->linesize);
        av_frame_unref(frame);
        if (videoCallback) {
            videoCallback(userData, (unsigned char *)rgbaFrame->data[0], rgbaFrame->width, rgbaFrame->height, rgbaFrame->linesize);
        }
    }
    av_free(outBuffer);
    av_frame_free(&rgbaFrame);
    av_frame_free(&frame);
}

void Render::renderAudio(unsigned char* outData, int *size)
{
    AVFrame * frame = av_frame_alloc();
    bool ret = player->getAudioDecoder()->getFrameQueue()->front(&frame);
    if (ret == false) {
        av_frame_free(&frame);
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
