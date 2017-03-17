//
//  SubtitleRenderer.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/15.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "SubtitleRenderer.hpp"
#include "PHPlayerCore.hpp"
extern "C"{
#include "avformat.h"
}

SubtitleRenderer::SubtitleRenderer(PHPlayerCore *player)
{
    this->player = player;
    library = 0;
    renderer = 0;
    track = 0;
    
    init();
}

void SubtitleRenderer::init()
{
    library = ass_library_init();
    renderer = ass_renderer_init(library);
}

void SubtitleRenderer::uninit()
{
    if (track) {
        ass_free_track(track);
    }
    if (renderer) {
        ass_renderer_done(renderer);
    }
    if (library) {
        ass_library_done(library);
    }
}

void SubtitleRenderer::processSub(char *ass, int64_t start_time, int64_t duration)
{
    if (LIBAVCODEC_VERSION_INT < AV_VERSION_INT(57,25,100)){
        ass_process_data(track, ass, strlen(ass));
    }
    else{
        ass_process_chunk(track, ass, strlen(ass),
                          start_time, duration);
    }
}

ASS_Image* SubtitleRenderer::render_frame(double time_ms)
{
    int detect_change = 0;
    ASS_Image *image = ass_render_frame(renderer, track,
                                        time_ms, &detect_change);
    return image;
}
