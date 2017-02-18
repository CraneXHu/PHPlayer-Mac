//
//  Audio.swift
//  PHPlayer
//
//  Created by huhexiang on 17/2/12.
//  Copyright © 2017年 huhexiang. All rights reserved.
//

import Foundation
import AudioToolbox

class AudioController : NSObject {
    
    let audioQueueNumberBuffers = 3
    var audioQueueRef: AudioQueueRef?
    var audioQueueBufferRefs:[AudioQueueBufferRef] = []
    var player: CPPWrapper?
    var bufferSize: UInt32?
    
    init(player: CPPWrapper) {
        super.init()
        self.player = player
    }
    
    func initWithAudioSpec(sampleRate: Float64, channels: UInt32) {
        
        //s16p
        var streamDescription : AudioStreamBasicDescription? = AudioStreamBasicDescription()
        streamDescription!.mSampleRate = sampleRate
        streamDescription!.mFormatID = kAudioFormatLinearPCM
        streamDescription!.mFormatFlags = kLinearPCMFormatFlagIsPacked | kLinearPCMFormatFlagIsSignedInteger
        streamDescription!.mChannelsPerFrame = 2
        streamDescription!.mFramesPerPacket = 1
        streamDescription!.mBitsPerChannel = 16
        
        streamDescription!.mBytesPerFrame = streamDescription!.mBitsPerChannel * streamDescription!.mChannelsPerFrame / 8;
        streamDescription!.mBytesPerPacket = streamDescription!.mBytesPerFrame * streamDescription!.mFramesPerPacket;
        
        /* Set the desired format */
        let status: OSStatus = AudioQueueNewOutput(&streamDescription!,
                                              audioQueueOuptutCallback,
                                              UnsafeMutableRawPointer(Unmanaged.passRetained(self).toOpaque()),
                                              nil,
                                              nil,
                                              0,
                                              &audioQueueRef);
        if (status != noErr) {
            return;
        }
        
        AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_Volume, 1.0)
        
        bufferSize = 1024*6
        
        
        for _ in 0..<audioQueueNumberBuffers
        {
            var audioQueueBufferRef: AudioQueueBufferRef?
            let status: OSStatus = AudioQueueAllocateBuffer(audioQueueRef!, bufferSize!, &audioQueueBufferRef)
            if status != noErr {
                return
            }
            audioQueueBufferRefs.append(audioQueueBufferRef!)
            audioQueueBufferRef?.pointee.mAudioDataByteSize = bufferSize!
            AudioQueueEnqueueBuffer(audioQueueRef!, audioQueueBufferRef!, 0, nil)
        }
        
        AudioQueueAddPropertyListener(audioQueueRef!, kAudioQueueProperty_IsRunning, audioQueueIsRunningCallback, nil)

//        
//        var propValue: UInt32 = 1;
//        AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_EnableTimePitch, &propValue, UInt32(MemoryLayout<UInt32>.size))
//        propValue = 1;
//        AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchBypass, &propValue, UInt32(MemoryLayout<UInt32>.size));
//        propValue = kAudioQueueTimePitchAlgorithm_Spectral;
//        AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchAlgorithm, &propValue, UInt32(MemoryLayout<UInt32>.size));
        
    }
    
    func play() {
        let status: OSStatus = AudioQueueStart(audioQueueRef!, nil)
        if (status != noErr) {
            return ;
        }
    
    }
    
    func pause() {
        let status: OSStatus = AudioQueuePause(audioQueueRef!)
        if (status != noErr) {
            return ;
        }
    }
    
    func flush() {
        AudioQueueFlush(audioQueueRef!)
    }
    
    func stop() {
        AudioQueueStop(audioQueueRef!, true);
        AudioQueueDispose(audioQueueRef!, true);
    }
    
    func setPlaybackRate(playbackRate: Float) {
        if fabsf(playbackRate - 1.0) <= 0.000001 {
            var propValue: UInt32 = 1
            AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchBypass, &propValue, UInt32(MemoryLayout<UInt32>.size))
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_PlayRate, 1.0)
        } else {
            var propValue: UInt32 = 0
            AudioQueueSetProperty(audioQueueRef!, kAudioQueueProperty_TimePitchBypass, &propValue, UInt32(MemoryLayout<UInt32>.size))
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_PlayRate, playbackRate)
        }
    }
    
    func setPlaybackVolume(playbackVolume: Float) {
        let aq_volume: Float = playbackVolume
        if (fabsf(aq_volume - 1.0) <= 0.000001) {
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_Volume, 1.0)
        } else {
            AudioQueueSetParameter(audioQueueRef!, kAudioQueueParam_Volume, aq_volume)
        }
    }
    
}

func audioQueueOuptutCallback(clientData: UnsafeMutableRawPointer?, AQ: AudioQueueRef, buffer: AudioQueueBufferRef) {
    
    let audioController = Unmanaged<AudioController>.fromOpaque(UnsafeRawPointer(clientData)!).takeUnretainedValue()
    audioController.player?.getAudioBuffer(buffer.pointee.mAudioData.assumingMemoryBound(to: UInt8.self), size: Int32(buffer.pointee.mAudioDataByteSize))
//    buffer.pointee.mAudioDataByteSize = audioController.bufferSize!
    AudioQueueEnqueueBuffer(AQ, buffer, 0, nil)
}

func audioQueueIsRunningCallback(clientData: UnsafeMutableRawPointer?, AQ: AudioQueueRef, inID: AudioQueuePropertyID)
{
    var running: UInt32 = 0
    var output: UInt32 = UInt32(MemoryLayout<UInt32>.size);
    let err : OSStatus = AudioQueueGetProperty(AQ, kAudioQueueProperty_IsRunning, &running, &output);
    if err != noErr {
    
    }
}
