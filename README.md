# SRT Encoder
srt encoder是基于srt发送的测试工具。<br/>
基于srt协议中的rtt，maxBw，inflight等信息预测低延时的出口带宽，动态的调整编码bitrate，从而实现基于网络出口带宽的svc编码。

## 1. 原理
基于bbr基本拥塞控制算法，通过一个周期内(1~2seconds)的minRTT, maxBw, 和当前的inflight，预测出编码bitrate的状态机(保持，增加，减少)。<br/>

备注：<br/>
* 本示例仅仅是基础的bbr算法示例，用户可以自己实现接口类(class CongestionCtrlI)中的接口来改进bbr算法；
* SRT还是一个在发展中的协议，关注其拥塞控制的对外参数更新，准确度也在提升中;

## 2. 如何编译
依赖ffmpeg动态库，libsrt, libx264, lib265, libfdk_aac等。
### 2.1 libx264
wget https://johnvansickle.com/ffmpeg/release-source/libx264-git.tar.xz <br/>
xz -d libx264-git.tar.xz <br/>
tar -xvf libx264-git.tar <br/>
cd ./libx264-git <br/>
./configure <br/>
make -j 2 <br/>
make install <br/>

### 2.2 libx265
yum install cmake
wget https://johnvansickle.com/ffmpeg/release-source/libx265-git.tar.xz  <br/>
xz -d libx265-git.tar.xz <br/>
tar -xvf libx265-git.tar  <br/>
cd libx265-git/source/ <br/>
mkdir build <br/>
cd build/ <br/>
cmake .. <br/>
make -j 2 <br/>
make install <br/>

### 2.3 libfdk_aac
wget https://downloads.sourceforge.net/opencore-amr/fdk-aac-2.0.1.tar.gz  <br/>
tar -xzvf fdk-aac-2.0.1.tar.gz   <br/>
cd fdk-aac-2.0.1  <br/>
./configure   <br/>
make -j 2  <br/>
make install  <br/>

### 2.4 libsrt
yum install git <br/>
git clone https://github.com/Haivision/srt.git <br/>
cd srt <br/>
./configure <br/>
make && make install <br/>

### 2.5 ffmpeg
wget https://johnvansickle.com/ffmpeg/release-source/ffmpeg-4.1.tar.xz <br/>
xz -d ffmpeg-4.1.tar.xz <br/>
tar -xvf ffmpeg-4.1.tar  <br/>
cd ffmpeg-4.1 <br/>
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig <br/>
./configure --target-os=linux --arch=x86_64 --enable-cross-compile --disable-avdevice --disable-doc --disable-devices --disable-ffplay --enable-libfdk-aac --enable-libx264 --enable-libx265 --enable-libsrt  --enable-nonfree --disable-asm --enable-gpl --pkgconfigdir=/usr/local/lib/pkgconfig --enable-shared <br/>
make -j 2 <br/>
make install

### 2.6 srt_encoder
cp lib/linux/libenc.so /usr/local/lib <br/>
ln /usr/local/lib/libenc.so /lib64/libenc.so <br/>
make根目下的Makefile即可 </br>

## 3. 如何运行
### 3.1 srt in srs
srs地址：[https://github.com/ossrs/srs](https://github.com/ossrs/srs) <br/>
在develop分支中有支持srt <br/>
启动: ./objs/srs -c conf/srt.conf
### 3.2 srt_encoder 
./srt_encoder /mnt/move/test.mp4 'srt://10.100.1.100:10080?streamid=#!::h=srs.srt.com.cn/live/livestream,m=publish'<br/>
命令行有两个参数：

* 第一个参数是源文件，也就是被编码的音视频文件；
* srt推流目的地址，也就是支持srt的srs流媒体地址，格式解释[https://github.com/ossrs/srs/wiki/v4_CN_SRTURL](https://github.com/ossrs/srs/wiki/v4_CN_SRTURL)

### 4. libenc api
在libenc库中，提供api给用户开发测试。<br/>
include/libenc.h中每个api函数都有详细解释。<br/>
