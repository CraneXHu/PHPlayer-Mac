//
//  Player.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/17.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "Player.hpp"
#include "PacketQueue.hpp"
#include "FrameQueue.hpp"

extern "C"{
#include "swscale.h"
#include "libavutil/imgutils.h"
}

/* no AV sync correction is done if below the minimum AV sync threshold */
#define AV_SYNC_THRESHOLD_MIN 0.04
/* AV sync correction is done if above the maximum AV sync threshold */
#define AV_SYNC_THRESHOLD_MAX 0.1
/* If a frame duration is longer than this, it will not be duplicated to compensate AV sync */
#define AV_SYNC_FRAMEDUP_THRESHOLD 0.1
/* no AV correction is done if too big error */
#define AV_NOSYNC_THRESHOLD 10.0

static enum AVPixelFormat get_format(struct AVCodecContext *s, const enum AVPixelFormat * fmt)
{
    while (*fmt != AV_PIX_FMT_NONE) {
        if (*fmt == AV_PIX_FMT_YUV420P) {
            return AV_PIX_FMT_YUV420P;
        }
        fmt++;
    }
    return AV_PIX_FMT_NONE;
}

Player::Player():pFormatCtx(0)
{
    videoPacketQueue = new PacketQueue();
    audioPacketQueue = new PacketQueue();
    videoFrameQueue = new FrameQueue();
    audioFrameQueue = new FrameQueue();
    
    isEnded = false;
    
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
    
    AVStream *pAVStream = 0;
    int ret = av_find_best_stream(pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (ret < 0) {
        return false;
    } else {
        videoStreamIndex = ret;
        pAVStream = pFormatCtx->streams[videoStreamIndex];
        
        AVCodec *dec = avcodec_find_decoder(pAVStream->codecpar->codec_id);
//        dec = avcodec_find_decoder_by_name("h264_vda");
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
//        pVideoCodecCtx->get_format = [](struct AVCodecContext *s, const enum AVPixelFormat * fmt) -> enum AVPixelFormat {
//            
//            while (*fmt != AV_PIX_FMT_NONE) {
//                if (*fmt == AV_PIX_FMT_YUV420P) {
//                    return AV_PIX_FMT_YUV420P;
//                }
//                fmt++;
//            }
//            return AV_PIX_FMT_VDA;
//        };
        pVideoCodecCtx->get_format = get_format;
        pVideoCodecCtx->thread_safe_callbacks = 1;
        
        ret = avcodec_open2(pVideoCodecCtx, dec, NULL);
        if (ret < 0) {
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
        
        pAudioCodecCtx->get_format = get_format;
        pAudioCodecCtx->thread_safe_callbacks = 1;
        
        ret = avcodec_open2(pAudioCodecCtx, dec, NULL);
        if (ret < 0) {
            return false;
        }
    }

    return true;
};

void Player::demux()
{
    printf("Demux Thread id : %d\n", std::this_thread::get_id());
    
	AVPacket *packet = av_packet_alloc();
    
    int ret = 0;
    while (1) {
        ret = av_read_frame(pFormatCtx, packet);
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
        }
        av_packet_unref(packet);
    }
    av_packet_free(&packet);
    isEnded = true;
}

void Player::decodeVideo()
{
    printf("Video Thread id : %d\n", std::this_thread::get_id());
    
	AVPacket *pkt = av_packet_alloc();
    while (!isEnded) {

        bool isSucceed = videoPacketQueue->front(pkt);
        if (!isSucceed) {
            continue;
        }
        int ret = decodeVideoPacket(pkt);
        av_packet_unref(pkt);
    }
    av_packet_free(&pkt);
}

int Player::decodeVideoPacket(AVPacket *pPacket)
{
    AVFrame *pFrame = av_frame_alloc();
    
    int ret = avcodec_send_packet(pVideoCodecCtx, pPacket);
    if (ret < 0) {
        goto out;
    }

    while (!ret) {
        ret = avcodec_receive_frame(pVideoCodecCtx, pFrame);
        if (!ret)
        {
            videoFrameQueue->push(pFrame);
            av_frame_unref(pFrame);
        }
    }
out:
    av_freep(pFrame);
    if (ret == AVERROR(EAGAIN))
        return 0;
    return ret;
}

void Player::decodeAudio()
{
	AVPacket *pkt = av_packet_alloc();
    
    while (!isEnded) {
        bool isSuccess = audioPacketQueue->front(pkt);
        if (!isSuccess) {
            continue;
        }
        int ret = decodeAudioPacket(pkt);
        av_packet_unref(pkt);
        
    }
    av_packet_free(&pkt);
}

int Player::decodeAudioPacket(AVPacket *pPacket)
{
    AVFrame *pFrame = av_frame_alloc();
    
    int ret = avcodec_send_packet(pAudioCodecCtx, pPacket);
    if (ret < 0) {
        goto out;
    }
    
    while (!ret) {
        ret = avcodec_receive_frame(pAudioCodecCtx, pFrame);
        if (!ret)
        {
            audioFrameQueue->push(pFrame);
            av_frame_unref(pFrame);
        }
    }
out:
    av_frame_free(&pFrame);
    if (ret == AVERROR(EAGAIN))
        return 0;
    return ret;
}

void Player::setCallback(Callback callback, void *ctx)
{
    reciveImage = callback;
    this->ctx = ctx;
}

void Player::playVideo()
{
    printf("Display Thread id : %d\n", std::this_thread::get_id());
    
    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pRGBAFrame = av_frame_alloc();
    pRGBAFrame->width = pVideoCodecCtx->width;
    pRGBAFrame->height = pVideoCodecCtx->height;
    pRGBAFrame->format = AV_PIX_FMT_RGBA;
    
    size_t size = av_image_get_buffer_size(AV_PIX_FMT_RGBA, pVideoCodecCtx->width, pVideoCodecCtx->height, 1);
    unsigned char *outBuffer = (unsigned char *)av_malloc(size);
    av_image_fill_arrays(pRGBAFrame->data, pRGBAFrame->linesize, outBuffer, AV_PIX_FMT_RGBA, pVideoCodecCtx->width, pVideoCodecCtx->height, 1);
    
    SwsContext *imageConvertContext = sws_getContext(pVideoCodecCtx->width, pVideoCodecCtx->height, pVideoCodecCtx->pix_fmt, pVideoCodecCtx->width, pVideoCodecCtx->height, AV_PIX_FMT_RGBA, SWS_BICUBIC, NULL, NULL, NULL);
    
    while (!isEnded) {
        
//        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        
        bool ret = videoFrameQueue->front(&pFrame);
        if (ret == false) {
            continue;
        }
        double timestamp;
        if(pFrame->pts == AV_NOPTS_VALUE) {
            timestamp = 0;
        } else {
            timestamp = av_frame_get_best_effort_timestamp(pFrame)*av_q2d(pFormatCtx->streams[videoStreamIndex]->time_base);
        }
        double frameRate = av_q2d(pFormatCtx->streams[videoStreamIndex]->avg_frame_rate);
        frameRate += pFrame->repeat_pict * (frameRate * 0.5);
        if (timestamp == 0.0) {
            std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)(frameRate*1000)));
        }else {
            if (fabs(timestamp - audioClock) > AV_SYNC_THRESHOLD_MIN &&
                fabs(timestamp - audioClock) < AV_NOSYNC_THRESHOLD) {
                if (timestamp > audioClock) {
                    std::this_thread::sleep_for(std::chrono::milliseconds((unsigned long)((timestamp - audioClock)*1000000)));
                }
            }
        }
        sws_scale(imageConvertContext, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0, pVideoCodecCtx->height, pRGBAFrame->data, pRGBAFrame->linesize);
        av_frame_unref(pFrame);
        reciveImage(ctx, (unsigned char *)pRGBAFrame->data, pRGBAFrame->width, pRGBAFrame->height, pRGBAFrame->linesize);
    }
    av_free(outBuffer);
    av_frame_free(&pRGBAFrame);
}

void Player::audioCallback()
{
    AVFrame *pFrame = av_frame_alloc();
    audioFrameQueue->front(&pFrame);
    std::unique_lock<std::mutex> lock(mutex);
    audioClock = pFrame->pts*av_q2d(pFormatCtx->streams[audioStreamIndex]->time_base);
}
