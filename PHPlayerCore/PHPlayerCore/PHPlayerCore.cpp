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
#include "PHPlayerCore.hpp"
#include "PHPlayerCorePriv.hpp"

PHPlayerCore::PHPlayerCore()
{
    player = new Player();
}

void PHPlayerCore::HelloWorld(const char * s)
{
    PHPlayerCorePriv *theObj = new PHPlayerCorePriv;
    theObj->HelloWorldPriv(s);
    delete theObj;
};

int PHPlayerCore::add(int a, int b)
{
    return a + b;
};

bool PHPlayerCore::open(char *file)
{
    bool ret = player->open(file);
    if (ret == false) {
        return false;
    }
    std::thread demuxThread(&Player::demux, player);
    demuxThread.join();
    std::thread videoThread(&Player::decodeVideo, player);
    videoThread.join();
    return true;
}

void PHPlayerCorePriv::HelloWorldPriv(const char * s) 
{
    std::cout << s << std::endl;
};
