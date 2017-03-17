//
//  SubtitleRenderer.hpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/15.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef SubtitleRenderer_hpp
#define SubtitleRenderer_hpp

#include <stdio.h>
#include <ass/ass.h>

class PHPlayerCore;
class SubtitleRenderer{
public:
    SubtitleRenderer(PHPlayerCore *player);
    void init();
    void uninit();
    void processSub(char *ass, int64_t start_time, int64_t duration);
    ASS_Image* render_frame(double time_ms);
    
private:
    PHPlayerCore *player;
    ASS_Library* library;
    ASS_Renderer* renderer;
    ASS_Track* track;
};
#endif /* SubtitleRenderer_hpp */
