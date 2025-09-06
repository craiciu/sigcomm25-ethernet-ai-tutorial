#ifndef SIGCOMM_LB
#define SIGCOMM_LB

#include "uec_mp.h"

class SigcommLoadBalancing : public UecMultipath {
public:
    SigcommLoadBalancing(uint16_t no_of_paths, bool debug);
    void processEv(uint16_t path_id, PathFeedback feedback) override;
    uint16_t nextEntropy(uint64_t seq_sent, uint64_t cur_cwnd_in_pkts) override;
    void set_debug_tag(string debug_tag) override;
private:
};

#endif