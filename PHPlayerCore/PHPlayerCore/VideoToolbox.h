//
//  VideoToolbox.h
//  PHPlayerCore
//
//  Created by huhexiang on 17/2/8.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#ifndef VideoToolbox_h
#define VideoToolbox_h

#include "avcodec.h"
#include "libavcodec/videotoolbox.h"

int videotoolbox_retrieve_data(AVCodecContext *pAVCodecContext, AVFrame *pFrame);
void videotoolbox_uninit(AVCodecContext *pAVCodecContext);
int videotoolbox_init(AVCodecContext *pAVCodecContext);

#endif /* VideoToolbox_h */
