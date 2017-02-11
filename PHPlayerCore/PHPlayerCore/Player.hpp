//
//  Player.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/1/17.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <thread>

struct AVPacket;
struct AVFormatContext;
struct AVCodecContext;
class PacketQueue;
class FrameQueue;

typedef void (*Callback)(void *ctx, unsigned char *data, int width, int height, int *linesize);
class Player
{
public:
    Player();
    bool open(char * file);
    void demux();
    void decodeVideo();
    int decodeVideoPacket(AVPacket *pPacket);
    void decodeAudio();
    int decodeAudioPacket(AVPacket *pPacket);
    void play();
    void setCallback(Callback callback, void *ctx);
    
private:
    AVFormatContext *pFormatCtx;
    AVCodecContext *pVideoCodecCtx;
    AVCodecContext *pAudioCodecCtx;
    PacketQueue *videoPacketQueue;
    FrameQueue *videoFrameQueue;
    PacketQueue *audioPacketQueue;
    int videoStreamIndex;
    int audioStreamIndex;
    bool isEnded;
    Callback reciveImage;
    void *ctx;
    
    std::mutex mutex;
    std::mutex frameMutex;
    std::condition_variable cond;
    std::condition_variable frameCond;
};
#endif /* Player_hpp */
