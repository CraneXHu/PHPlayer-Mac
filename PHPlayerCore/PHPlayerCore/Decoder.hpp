//
//  Decoder.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Decoder_hpp
#define Decoder_hpp

#include <stdio.h>
#include <atomic>
#include "globalenum.h"

struct AVCodecContext;
class Demuxer;
class PHPlayerCore;
class PacketQueue;
class FrameQueue;

class Decoder{
public:
    Decoder(PHPlayerCore *player, DecoderType type);
    ~Decoder();

    bool open();
    bool start();
    void clear();
    void flush();
    void decode();
    void close();
    
    void setEnableHardwareAcceleration(bool isEnable);
    bool isEnableHardwareAcceleration();
    
    AVCodecContext *getCodecContex();
    FrameQueue *getFrameQueue();
    
private:
    PHPlayerCore *player;
    DecoderType type;
    std::atomic<bool> hardwareAcceleration;
    AVCodecContext *codecContext;
    PacketQueue *packetQueue;
    FrameQueue *frameQueue;
};
#endif /* Decoder_hpp */
