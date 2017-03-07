//
//  Source.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "Source.hpp"
#include "PHPlayerCore.hpp"
#include "Demuxer.hpp"
extern "C"{
#include "avformat.h"
}

Source::Source(PHPlayerCore *player)
{
    formatContext = 0;
    this->player = player;
}

Source::~Source()
{
    
}

void Source::init(){
    av_register_all();
    avformat_network_init();
}

bool Source::open(char *file)
{
    if (avformat_open_input(&formatContext, file, NULL, NULL) < 0) {
        return false;
    }
    
    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        return false;
    }
    player->getDemuxer()->findStream();
    return true;
}

void Source::close()
{
    avformat_close_input(&formatContext);
}

double Source::getDuration()
{
    //second
   return formatContext->duration/AV_TIME_BASE;
}

char *Source::getFileName()
{
    return formatContext->filename;
}

AVFormatContext* Source::getContext(){
    return formatContext;
}
