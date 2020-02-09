#ifndef ENC_H
#define ENC_H
#include <string>

#define LIBENC_ERROR -1

#define LIBENC_OK 0

/*
function:   init log file, call it before log output
parameters:
1) log_file: log file path, if you set it "", log will be outputed in console.
return: void
*/
void init_log(const std::string log_file);

/*
function: ceate encoder handle
parameter: void
return: type is void* which is the encoder handle.
*/
void* create_encode_handle();

/*
function: init reader by encoder handle and source url
parameter:
1) handle: type is void* which is the encode handle.
2) src_url: type is const std::string& which can be source stream url or local file path
return: type is int, LIBENC_OK means successful.
*/
int init_reader(void* handle, const std::string& src_url);

/*
function: init encode video parameters
parameter:
1) handle:    type is void* which is the encode handle.
2) vcodec:    the string must be "libx264" or "libx265"
3) width:     the width of pix
4) height:    the height of pic
5) profile:   it must be one of the "baseline", "main" or "high"
6) preset:    it must be one of the "ultrafast", "superfast", "veryfast", "faster", "fast", "medium", "slow", "slower" or "veryslow"
7) bitrate_i: video bitrate which unit is kbps.
8) framerate_i: video frame rate which unit is fps.
return: type is int, LIBENC_OK means successful.
*/
int init_encode_video(void* handle, const std::string& vcodec, int width, int height, 
        const std::string& profile, const std::string& preset, int bitrate_i, int framerate_i);

/*
function: init encode audio parameters
parameter:
1) handle:       type is void* which is the encode handle.
2) fmt:          the string must be "libfdk_aac"
3) samplerate_i: it must be one of the 48000, 44100 or 22050.
4) channel:      it must be 1 or 2
5) bitrate:      audio bitrate which unit is kbps, suggest range 64~128.
return: type is int, LIBENC_OK means successful.
*/
int init_encode_audio(void* handle, const std::string fmt, int samplerate_i, 
        int channel, int bitrate);

/*
function: init muxer writer
parameter:
1) handle:  type is void* which is the encode handle.
2) fmt:     the string must be "flv" or "mpegts"
3) dst_url: it must be rtmp url or srt url
   rtmp eg. fmt = "flv", dst_url = "rtmp://127.0.0.1/live/livestream"
   srt eg. fmt = "mpegts", dst_url = "srt://127.0.0.1:10080?streamid=#!::h=live/livestream,m=publish"
return: type is int, LIBENC_OK means successful.
*/
int init_writer(void* handle, const std::string& fmt, const std::string& dst_url);

/*
function: init encode audio parameters
parameter:
1) handle:          type is void* which is the encode handle.
2) congestion_ctrl: it must be object point which is derived from CongestionCtrlI;
return: void
*/
void set_congestion_ctrl(void* handle, void* congestion_ctrl);

/*
function: start encodeing behavior
parameter:
1) handle:  type is void* which is the encode handle.
return: type is int, LIBENC_OK means successful.

befor calling enc_start, 
create_encode_handle, init_reader, init_encode_video, init_encode_audio, init_writer must be called.
*/
int enc_start(void* handle);

/*
function: stop encodeing behavior
parameter:
1) handle:  type is void* which is the encode handle.
return: void.
befor calling enc_stop, enc_start must be called.
*/
void enc_stop(void* handle);

/*
function: release encodeing handle
parameter:
1) handle:  type is void* which is the encode handle.
return: void.
befor calling enc_release, create_encode_handle must be called.
*/
void enc_release(void* handle);

/*
function: get current encoder bitrate
parameter:
1) handle:  type is void* which is the encode handle.
return: bitrate(bits/s).
befor calling get_current_enc_bitrate, enc_start must be called.
*/
int64_t get_current_enc_bitrate(void* handle);

#endif //ENC_H