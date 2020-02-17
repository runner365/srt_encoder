English | [中文](https://github.com/runner365/srt_encoder/blob/master/README-Cn.md)
# SRT Encoder
An adaptive encoder based on SRT.<br/>
The encoder estimate the output bitrate based on rtt, maxBw, inflight to implement the adaptive bitrate encoder.

- ffmpeg commnand <br/>
It only need to replace ffmpeg.c and add srt_header.h in fftools fold. Then compile the ffmpeg which supports the adaptive encoder.
- based on srt protocol <br/>
srt is a simple, low latency, high efficient transport protocol.
- based on bbr congestion algorithem <br/>
It estimates the bitrate by bbr.

## 1. introduction
The encoder estimate the output bitrate based on rtt, maxBw, inflight and use the bbr algorithem to implement the adaptive bitrate encoder.<br/>

## 2. how to compile
The ffmpeg depends on libsrt, libx264, libfdk_aac.
### 2.1 replace ffmpeg.c
The adaptive encoder is developed based on ffmpeg4.1. It need to replace ffmpeg.c and add srt_header.h in fftools fold. <br/>
ffmpeg4.1 official download: [ffmpeg-4.1.tar.xz](https://johnvansickle.com/ffmpeg/release-source/ffmpeg-4.1.tar.xz)

### 2.2 compile ffmpeg
go to wiki：[How to compile](https://github.com/runner365/srt_encoder/wiki/How-to-compile-cn)

## 3. how to use it
### 3.1 srt in srs
srs is live server which supports rtmp/hls/rtp live service. <br/>
Now it supports srt live service in "develop" branch. <br/>
srs github：[https://github.com/ossrs/srs](https://github.com/ossrs/srs) <br/>

How to use srt service in srs，go to wiki：[v4_CN_SRTWiki](https://github.com/ossrs/srs/wiki/v4_CN_SRTWiki) <br/>

### 3.2 srt encoder 
run command：<br/>
ffmpeg -re -i source.mp4 -c:v libx264 -s 640x360 -b:v 700k -c:a libfdk_aac -ab 64k -ar 44100 -ac 2 -f mpegts 'srt://127.0.0.1:10080?streamid=#!::h=live/livestream,m=publish' <br/>

Note：
* can't configure -crf <br/>
crf can't be configured because crf mode conflict with the adaptive bitrate mode in ffmpeg.
* can't configure -qp <br/>
qp can't be configured because qp mode conflict with the adaptive bitrate mode in ffmpeg.
* the media format must be mpegts in srt <br/>
