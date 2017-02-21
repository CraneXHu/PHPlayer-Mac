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
#include "globalenum.h"

struct AVCodecContext;
class PHPlayerCore;
class PacketQueue;
class FrameQueue;

class Decoder{
public:
    Decoder(PHPlayerCore *player, DecoderType type);
    ~Decoder();

    bool openDecoder();
    bool start();
    void decode();
    
    AVCodecContext *getCodecContex();
    FrameQueue *getFrameQueue();
    
private:
    PHPlayerCore *player;
    DecoderType type;
    AVCodecContext *codecContext;
    PacketQueue *packetQueue;
    FrameQueue *frameQueue;
};
#endif /* Decoder_hpp */
