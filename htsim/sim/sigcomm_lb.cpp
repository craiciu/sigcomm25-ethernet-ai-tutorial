#include "sigcomm_lb.h"

SigcommLoadBalancing::SigcommLoadBalancing(uint16_t no_of_paths, bool debug)
    : UecMultipath(debug){
		//TODO: initialize load balancer state for given number of paths.
}

void SigcommLoadBalancing::set_debug_tag(string debug_tag) {
}

void SigcommLoadBalancing::processEv(uint16_t path_id, PathFeedback feedback) {
	//process feedback we received from this path_id and update local state

}

uint16_t SigcommLoadBalancing::nextEntropy(uint64_t seq_sent, uint64_t cur_cwnd_in_pkts) {
	// generate next entropy value based on current load balancing state
	// TODO: return the chosen entropy value. Returning 0 for now, which makes the connection behave like single path. 
	return 0;
}