//
//  Demuxer.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Demuxer_hpp
#define Demuxer_hpp

#include <stdio.h>

struct AVFormatContext;
struct AVStream;
class PacketQueue;
class PHPlayerCore;
class Decoder;

class Demuxer {
public:
    Demuxer(PHPlayerCore *player);
    ~Demuxer();
    
    bool open(char *url);
    void findStream();
    bool start();
    void demux();
    void seek(double position, int flag);
    void clear();
    void flush();
    void close();
    
    AVStream *getVideoStream();
    AVStream *getAudioStream();
    AVStream *getSubtitleStream();
    PacketQueue *getVideoPacketQueue();
    PacketQueue *getAudioPacketQueue();
    PacketQueue *getSubtitlePacketQueue();
    
    Decoder *getVideoDecoder();
    Decoder *getAudioDecoder();
    Decoder *getSubtitleDecoder();
    
    int getVideoWidth();
    int getVideoHeight();
    
    int getAudioSampleRate();
    int getAudioChannels();
    
    char * getFileName();
    double getDuration();
    
private:
    PHPlayerCore *player;
    AVFormatContext *formatContext;
    AVStream *videoStream;
    AVStream *audioStream;
    AVStream *subtitleStream;
    PacketQueue *videoPacketQueue;
    PacketQueue *audioPacketQueue;
    PacketQueue *subtitlePacketQueue;
    Decoder *videoDecoder;
    Decoder *audioDecoder;
    Decoder *subtitleDecoder;
    bool isRequestSeek;
    double seekPosition;
    int flag;
};
#endif /* Demuxer_hpp */
