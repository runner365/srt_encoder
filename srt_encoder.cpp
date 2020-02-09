#include "libenc.h"
#include "simple_bbr_ctrl.h"
#include <thread>

std::string get_dst_fmt(std::string dst_url) {
    std::string fmt;

    size_t pos = dst_url.find("rtmp://");
    if (pos == 0) {
        fmt = "flv";
        return fmt;
    }

    pos = dst_url.find("srt://");
    if (pos == 0) {
        fmt = "mpegts";
        return fmt;
    }

    pos = dst_url.find("http://");
    if (pos == 0) {
        pos = dst_url.find(".m3u8");
        if (pos != dst_url.npos) {
            fmt = "hls";
            return fmt;
        }
    }

    return fmt;
}

int main(int argn, char** argv) {
    int ret;

    if (argn < 3) {
        return 0;
    }

    std::string src_url = argv[1];
    std::string dst_url = argv[2];

    std::string fmt = get_dst_fmt(dst_url);
    std::shared_ptr<SimpleBBRCtrl> bbr_ctrl_ptr = std::make_shared<SimpleBBRCtrl>();

    init_log("");

    void* handle = create_encode_handle();

    ret = init_reader(handle, src_url);
    if (ret != LIBENC_OK) {
        printf("init reader error.\r\n");
        return 0;
    }

    ret = init_encode_video(handle, "libx264", 640, 360, 
                        "high", "veryfast", 700, 30);
    if (ret != LIBENC_OK) {
        printf("init encode video error.\r\n");
        return 0;
    }

    ret = init_encode_audio(handle, "libfdk_aac", 44100, 2, 64);
    if (ret != LIBENC_OK) {
        printf("init encode audio error.\r\n");
        return 0;
    }
    ret = init_writer(handle, fmt, dst_url);
    if (ret != LIBENC_OK) {
        printf("init writer error.\r\n");
        return 0;
    }
    set_congestion_ctrl(handle, bbr_ctrl_ptr.get());

    ret = enc_start(handle);
    if (ret != LIBENC_OK) {
        printf("encode start error.\r\n");
        return 0;
    }

    do {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        int bitrate = get_current_enc_bitrate(handle);
        printf("current encoder bitrate is %dkbps.\r\n", bitrate/1000);
    } while(true);

    
    enc_stop(handle);
    printf("encode stoped...");

    enc_release(handle);
    printf("encode release...");

    return 1;
}