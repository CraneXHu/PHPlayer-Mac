//
//  Render.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/21.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef Render_hpp
#define Render_hpp

#include <stdio.h>

class PHPlayerCore;

class Render{
public:
    Render(PHPlayerCore *player);
    ~Render();
    
    void renderVideo();
    void renderAudio(unsigned char* outData, int size);
private:
    PHPlayerCore *player;
    int audioClock;
};
#endif /* Render_hpp */
