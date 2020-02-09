#ifndef CONGESTION_CTRL_H
#define CONGESTION_CTRL_H

#define STATE_INCR 0x01
#define STATE_DECR 0x02
#define STATE_KEEP 0x03

/*
CongestionCtrlI is a interface which support bbr congestion control.
*/
class CongestionCtrlI {
public:
    //update current rtt in ms
    virtual void update_rtt(int rtt) = 0;
    //update current max bandwith(bits/s)
    virtual void update_maxbw(unsigned long long maxbw) = 0;
    //set gain which is used for the condition (inflight >= cwnd_gain × bdp)
    virtual void set_cwnd_gain(double gain) = 0;

    //get the state whether adjust the encoder bitrate.
    //input parameter: inflight the sender send the packet which isn't confirmed by ACK.
    //return:
    //1) STATE_INCR: the encoder bitrate can be increased.
    //2) STATE_DECR: the encoder bitrate can be decreased.
    //3) STATE_KEEP: the encoder bitrate can be keeped.
    virtual int get_state(int inflight) = 0;
};

#endif //CONGESTION_CTRL_H