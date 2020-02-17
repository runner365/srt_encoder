[English](https://github.com/runner365/srt_encoder/blob/master/README.md) | 中文
# SRT Encoder
SRT Encoder基于srt自适应bitrate的直播编码器。<br/>
基于srt协议中的rtt，maxBw，inflight等信息预测低延时的出口带宽，动态的调整编码bitrate，从而实现基于网络出口带宽的编码。<br/>
- 简单易用，使用ffmpeg命令行 <br/>
基于ffmpeg-4.1, 通过自定义的ffmpeg，实现srt自适应码率编码器。只需要替换ffmpeg4.1源码fftools目录下的ffmpeg.c，和加入srt_header.h，编译ffmpeg即可。
- 基于srt直播协议 <br/>
srt是基于udt针对音视频特性而设计的传输协议，简单/实时/高效。
- 基于bbr拥塞算法 <br/>
基于bbr拥塞算法，预测出口bitrate，自适应bitrate动态编码。

## 1. 原理
基于bbr基本拥塞控制算法，通过一个周期内(1~2seconds)的minRTT, maxBw, 和当前的inflight，预测出编码bitrate的状态机(保持，增加，减少)。<br/>

## 2. 如何编译
依赖ffmpeg动态库，libsrt, libx264, lib265, libfdk_aac等。
### 2.1 替代ffmpeg.c
编码器是基于ffmpeg4.1开发的，只需要把ffmpeg-4.1/fftools目录下的ffmpeg.c和srt_header.h拷贝替换到对应ffmpeg4.1版本对应的目录下。
ffmpeg4.1的官方打包源码地址: [ffmpeg-4.1.tar.xz](https://johnvansickle.com/ffmpeg/release-source/ffmpeg-4.1.tar.xz)

### 2.2 如何编译ffmpeg
请看wiki：[How to compile](https://github.com/runner365/srt_encoder/wiki/How-to-compile-cn)

## 3. 如何运行
### 3.1 srt in srs
srs地址：[https://github.com/ossrs/srs](https://github.com/ossrs/srs) <br/>
在develop分支中有支持srt直播服务 <br/>
如何在srs中使用srt直播服务，请看wiki：[v4_CN_SRTWiki](https://github.com/ossrs/srs/wiki/v4_CN_SRTWiki) <br/>

### 3.2 srt encoder 
运行：<br/>
ffmpeg -re -i source.mp4 -c:v libx264 -s 640x360 -b:v 700k -c:a libfdk_aac -ab 64k -ar 44100 -ac 2 -f mpegts 'srt://127.0.0.1:10080?streamid=#!::h=live/livestream,m=publish' <br/>

备注：
* 不能配置-crf <br/>
因为x264的crf模式与动态bitrate冲突
* 不能配置-qp <br>
因为x264的qp模式与动态bitrate冲突
* srt内的媒体封装格式是mpegts
