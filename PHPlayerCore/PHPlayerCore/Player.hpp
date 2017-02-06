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

struct AVPacket;
struct AVFormatContext;
struct AVCodecContext;
class PacketQueue;
class FrameQueue;

class Player
{
public:
    Player();
    bool open(char * file);
    void demux();
    void decodeVideo();
    int decodeVideoPacket(AVPacket *pPacket);
    void decodeAudio();
    int decodeAudio(AVPacket *pPacket);
    
private:
    AVFormatContext *pFormatCtx;
    AVCodecContext *pVideoCodecCtx;
    AVCodecContext *pAudioCodecCtx;
    PacketQueue *videoPacketQueue;
    FrameQueue *videoFrameQueue;
    PacketQueue *audioPacketQueue;
    int videoStreamIndex;
    int audioStreamIndex;
};
#endif /* Player_hpp */
