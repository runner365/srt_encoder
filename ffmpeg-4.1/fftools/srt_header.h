#ifndef SRT_HEADER_H
#define SRT_HEADER_H
#ifdef __cplusplus  
extern "C"  
{  
#endif  
#include <libavutil/avstring.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/parseutils.h>
#include <libavutil/mathematics.h>
#include <libswscale/swscale.h>
#include <libavutil/time.h>
#include <libavutil/dict.h>

#include <libavutil/opt.h>
#include <libavutil/channel_layout.h>
#include <libavutil/samplefmt.h>
#include <libswresample/swresample.h>

#ifdef __cplusplus  
};  
#endif
#include <srt/srt.h>

enum SRTMode {
    SRT_MODE_CALLER = 0,
    SRT_MODE_LISTENER = 1,
    SRT_MODE_RENDEZVOUS = 2
};

typedef struct SRTContext {
    const AVClass *av_class;
    int fd;
    int eid;
    int64_t rw_timeout;
    int64_t listen_timeout;
    int recv_buffer_size;
    int send_buffer_size;

    int64_t maxbw;
    int pbkeylen;
    char *passphrase;
    int mss;
    int ffs;
    int ipttl;
    int iptos;
    int64_t inputbw;
    int oheadbw;
    int64_t latency;
    int tlpktdrop;
    int nakreport;
    int64_t connect_timeout;
    int payload_size;
    int64_t rcvlatency;
    int64_t peerlatency;
    enum SRTMode mode;
    int sndbuf;
    int rcvbuf;
    int lossmaxttl;
    int minversion;
    char *streamid;
    char *smoother;
    int messageapi;
    SRT_TRANSTYPE transtype;
} SRTContext;

#endif//SRT_HEADER_H