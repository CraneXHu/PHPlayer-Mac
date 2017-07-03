//
//  AudioRenderer.cpp
//  PHPlayerCore
//
//  Created by huhexiang on 17/3/16.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

#include "AudioRenderer.hpp"
#include "Renderer.hpp"
#include <math.h>
#include "PHPlayerCore.hpp"

void audioQueueOuptutCallback(void *clientData, AudioQueueRef AQ, AudioQueueBufferRef buffer) {
    
    PHPlayerCore *player = 0;
    player = (PHPlayerCore*)clientData;
    if (player) {
        int size = 0;
        player->getAudioData((unsigned char *)buffer->mAudioData, &size);
        buffer->mAudioDataByteSize = size;
    }
    AudioQueueEnqueueBuffer(AQ, buffer, 0, 0);
}

AudioRenderer::AudioRenderer(PHPlayerCore *player)
{
    this->player = player;
    audioQueueBufferRefs = new AudioQueueBufferRef[3];
}

AudioRenderer::~AudioRenderer()
{
    delete [] audioQueueBufferRefs;
}

void AudioRenderer::init()
{
    //s16p
    AudioStreamBasicDescription streamDescription;
    streamDescription.mSampleRate = player->getAudioSampleRate();
    streamDescription.mFormatID = kAudioFormatLinearPCM;
    streamDescription.mFormatFlags = kLinearPCMFormatFlagIsPacked | kLinearPCMFormatFlagIsSignedInteger;
    streamDescription.mChannelsPerFrame = player->getAudioChannels();
    streamDescription.mFramesPerPacket = 1;
    streamDescription.mBitsPerChannel = 16;
    
    streamDescription.mBytesPerFrame = streamDescription.mBitsPerChannel * streamDescription.mChannelsPerFrame / 8;
    streamDescription.mBytesPerPacket = streamDescription.mBytesPerFrame * streamDescription.mFramesPerPacket;
    
    /* Set the desired format */
    OSStatus status = AudioQueueNewOutput(&streamDescription,
                                          audioQueueOuptutCallback,
                                          player,
                                          nil,
                                          nil,
                                          0,
                                          &audioQueueRef);
    if (status != noErr) {
        return;
    }
    
    AudioQueueSetParameter(audioQueueRef, kAudioQueueParam_Volume, 1.0);
    
    bufferSize = streamDescription.mSampleRate;
    
    
    for (int i = 0; i < 3; i++)
    {
        OSStatus status = AudioQueueAllocateBuffer(audioQueueRef, bufferSize, &audioQueueBufferRefs[i]);
        if (status != noErr) {
            return;
        }
        audioQueueBufferRefs[i]->mAudioDataByteSize = 10;
        audioQueueOuptutCallback(0, audioQueueRef, audioQueueBufferRefs[i]);
    }
}

void AudioRenderer::play()
{
    AudioQueueStart(audioQueueRef, 0);
}

void AudioRenderer::pause()
{
    AudioQueuePause(audioQueueRef);
}

void AudioRenderer::flush()
{
    AudioQueueFlush(audioQueueRef);
}

void AudioRenderer::stop()
{
    AudioQueueStop(audioQueueRef, true);
    AudioQueueDispose(audioQueueRef, true);
}

void AudioRenderer::setPlaybackRate(float rate)
{
    if (fabsf(rate - 1.0) <= 0.000001) {
        int propValue = 1;
        AudioQueueSetProperty(audioQueueRef, kAudioQueueProperty_TimePitchBypass, &propValue, sizeof(int));
        AudioQueueSetParameter(audioQueueRef, kAudioQueueParam_PlayRate, 1.0);
    } else {
        int propValue = 0;
        AudioQueueSetProperty(audioQueueRef, kAudioQueueProperty_TimePitchBypass, &propValue, sizeof(int));
        AudioQueueSetParameter(audioQueueRef, kAudioQueueParam_PlayRate, rate);
    }
}

void AudioRenderer::setPlaybackVolume(float volume)
{
    float aq_volume = volume;
    if (fabsf(aq_volume - 1.0) <= 0.000001) {
        AudioQueueSetParameter(audioQueueRef, kAudioQueueParam_Volume, 1.0);
    } else {
        AudioQueueSetParameter(audioQueueRef, kAudioQueueParam_Volume, aq_volume);
    }
}

