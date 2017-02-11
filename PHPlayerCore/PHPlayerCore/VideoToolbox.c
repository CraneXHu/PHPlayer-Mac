//
//  VideoToolbox.c
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/8.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "VideoToolbox.h"

int videotoolbox_retrieve_data(AVCodecContext *pAVCodecContext, AVFrame *pFrame)
{
    int ret = 0;
    return ret;
}

void videotoolbox_uninit(AVCodecContext *pAVCodecContext)
{
    av_videotoolbox_default_free(pAVCodecContext);
}

int videotoolbox_init(AVCodecContext *pAVCodecContext)
{
    int ret = 0;
    ret = av_videotoolbox_default_init(pAVCodecContext);
    if (ret < 0) {
        videotoolbox_uninit(pAVCodecContext);
    }
    return ret;
}
