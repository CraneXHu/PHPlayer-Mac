/*
 *  PHPlayerCore.hpp
 *  PHPlayerCore
 *
 *  Created by huhexiang on 17/1/16.
 *  Copyright © 2017年 huhexiang. All rights reserved.
 *
 */

#ifndef PHPlayerCore_
#define PHPlayerCore_

/* The classes below are exported */
#pragma GCC visibility push(default)

#include "Player.hpp"

class PHPlayerCore
{
public:
    PHPlayerCore();
    void HelloWorld(const char *);
    int add(int a, int b);
    bool open(char* file);
    void start();
    void setCallback(Callback callback, void * ctx);
    void getAudioBuffer(unsigned char *outData, int size);
    
private:
    Player *player;
};

#pragma GCC visibility pop
#endif
