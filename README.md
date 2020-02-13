# SRT Encoder
SRT Encoder基于srt自适应码率的编码器。<br/>
基于srt协议中的rtt，maxBw，inflight等信息预测低延时的出口带宽，动态的调整编码bitrate，从而实现基于网络出口带宽的编码。
github地址: [https://github.com/runner365/srt_encoder](https://github.com/runner365/srt_encoder)
## 1. 原理
基于bbr基本拥塞控制算法，通过一个周期内(1~2seconds)的minRTT, maxBw, 和当前的inflight，预测出编码bitrate的状态机(保持，增加，减少)。<br/>

备注：<br/>
* 本示例仅仅是基础的bbr算法示例，用户可以自己实现接口类(class CongestionCtrlI)中的接口来改进bbr算法；
* SRT还是一个在发展中的协议，关注其拥塞控制的对外参数更新，准确度也在提升中;

## 2. 如何编译
依赖ffmpeg动态库，libsrt, libx264, lib265, libfdk_aac等。
如何编译请看wiki：[How to compile](https://github.com/runner365/srt_encoder/wiki/How-to-compile)

## 3. 如何运行
### 3.1 srt in srs
srs地址：[https://github.com/ossrs/srs](https://github.com/ossrs/srs) <br/>
在develop分支中有支持srt <br/>
如何在srs中使用srt，请看wiki：[v4_CN_SRTWiki](https://github.com/ossrs/srs/wiki/v4_CN_SRTWiki) <br/>

### 3.2 srt_encoder 
./srt_encoder /mnt/move/test.mp4 'srt://10.100.1.100:10080?streamid=#!::h=srs.srt.com.cn/live/livestream,m=publish'<br/>
命令行有两个参数：

* 第一个参数是源文件，也就是被编码的音视频文件；
* srt推流目的地址，也就是支持srt的srs流媒体地址，格式解释[https://github.com/ossrs/srs/wiki/v4_CN_SRTURL](https://github.com/ossrs/srs/wiki/v4_CN_SRTURL)

## 4. libenc api
在libenc库中，提供api给用户开发测试。<br/>
include/libenc.h中每个api函数都有详细解释。<br/>
