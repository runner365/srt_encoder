#ifndef SIMPLE_BBR_CTRL_H
#define SIMPLE_BBR_CTRL_H
#include "congestion_ctrl.h"
//#include "slog/slog.h"
#include <memory>
#include <string.h>
#include <mutex>

#define RTT_LIST_MAX    5
#define BW_LIST_MAX     5
#define STATE_LIST_MAX  5
#define CWND_GAIN_DEF   1.2
#define RTT_INIT        100

class SimpleBBRCtrl : public CongestionCtrlI
{
public:
    SimpleBBRCtrl():_rtt_index(0)
        ,_rtt_min(RTT_INIT)
        ,_rtt_ready(false)
        ,_bw_index(0)
        ,_bw_max(0)
        ,_bw_ready(false)
        ,_state_index(0)
        ,_state_ready(false)
        ,_cwnd_gain(CWND_GAIN_DEF)
        ,_current_state(STATE_KEEP)
    {
        memset(_rtt_array, 0, sizeof(_rtt_array));
        memset(_bw_array, 0, sizeof(_bw_array));
    }

    virtual ~SimpleBBRCtrl(){}

    //update current rtt in ms
    virtual void update_rtt(int rtt) {
        std::lock_guard<std::mutex> lock(_mutex);

        if (rtt < 1) {
            rtt = 10;
        }

        _rtt_array[_rtt_index] = rtt;
        _rtt_index = (_rtt_index + 1)%RTT_LIST_MAX;
        if (_rtt_index == 0) {
            _rtt_ready = true;
        }

        if (_rtt_ready) {
            _rtt_min = _rtt_array[0];
            for (int index = 0; index < RTT_LIST_MAX; index++) {
                //SLOGF(SLOG_INFO, "update_rtt rtt[%d]=%d, input rtt:%d, ", 
                //index, _rtt_array[index], rtt);
                if ((_rtt_min == 0) || (_rtt_min > _rtt_array[index])) {
                    _rtt_min = _rtt_array[index];
                }
            }
        }
    }

    //update current max bandwith(bits/s)
    virtual void update_maxbw(unsigned long long maxbw) {
        std::lock_guard<std::mutex> lock(_mutex);
        _bw_array[_bw_index] = maxbw;
        _bw_index = (_bw_index + 1)%BW_LIST_MAX;
        if (_bw_index == 0) {
            _bw_ready = true;
        }

        if (_bw_ready) {
            _bw_max = _bw_array[0];
            int64_t total = 0;
            for (int index = 0; index < BW_LIST_MAX; index++) {
                total += _bw_array[index];
                if ((_bw_max == 0) || (_bw_max < _bw_array[index])) {
                    _bw_max = _bw_array[index];
                }
            }
            int64_t avg_bw = total/BW_LIST_MAX;
            _bw_max = (_bw_max + avg_bw)/2;
        }
    }

    //set gain which is used for the condition (inflight >= cwnd_gain × bdp)
    virtual void set_cwnd_gain(double gain) {
        _cwnd_gain = gain;
    }

    //get the state whether adjust the encoder bitrate.
    virtual int get_state(int inflight) {
        std::lock_guard<std::mutex> lock(_mutex);
        const int INCR = 1;
        const int KEEP = 0;
        const int DECR = -1;
        int current_state = KEEP;

        if (!_rtt_ready || !_bw_ready) {
            return STATE_KEEP;
        }
        double bdp = _bw_max*_rtt_min/1000;
        inflight = inflight*8;

        if ((double)inflight > _cwnd_gain * bdp) {
            current_state = DECR;
        } else if ((double)inflight < bdp) {
            current_state = INCR;
        } else {
            current_state = KEEP;
        }

        _state_array[_state_index] = current_state;
        _state_index = (_state_index+1)%STATE_LIST_MAX;
        if (_state_index == 0) {
            _state_ready = true;
        }

        if (!_state_ready) {
            return STATE_KEEP;
        }
        int final_state = 0;
        for (int index = 0; index < STATE_LIST_MAX; index++) {
            final_state += _state_array[index];
        }
        //SLOGF(SLOG_INFO, "get congestion state, inflight:%d, _bw_max:%lu, _rtt_min:%d, bdp:%f, final_state:%d",
        //    inflight, _bw_max, _rtt_min, bdp, final_state);
        if (final_state > INCR) {
            return STATE_INCR;
        } else if (final_state < DECR) {
            return STATE_DECR;
        }

        return STATE_KEEP;
    }

private:
    int _rtt_array[RTT_LIST_MAX];
    int _rtt_index;
    int _rtt_min;//ms
    bool _rtt_ready;

    unsigned long long _bw_array[BW_LIST_MAX];//bits/s
    int _bw_index;
    unsigned long long _bw_max;//bits/s
    bool _bw_ready;

    int _state_array[STATE_LIST_MAX];
    int _state_index;
    bool _state_ready;

    double _cwnd_gain;
    int _current_state;
    std::mutex _mutex;
};

#endif //SIMPLE_BBR_CTRL_H