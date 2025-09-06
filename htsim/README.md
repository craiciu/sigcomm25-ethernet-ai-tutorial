# Overview

In this tutorial we will be using [UEC htsim](https://github.com/ultraethernet/uet-htsim) which in turn is based on the open source htsim network simulator.

To participate in the hands-on session follow the instructions below that help you install, build and then run experiments with htsim. 


## Getting Started

htsim is written in C++, and has no major dependencies.  
It should compile and run with g++ or clang on MacOS or Linux.  

To get started with running experiments, take a look in the `htsim/sim/datacenter` directory where there are some examples.  
These examples generally require bash, python3 and gnuplot.

## Building the project

Install the Python requirements by running:

```bash
pip install -r requirements.txt
```

Then compile the project from the `sim/` folder by running

```bash
# To configure the cmake project
cmake -S . -B build
# To build the project
cmake --build build --parallel 
```

##  Running a validation suite

From the `sim/datacenter/` folder, run:

```bash
python3 validate.py validate_uec_sender.txt
```
This will proceed to run a series of experiments, stating for each of them if the test has passed or not. The wide majority of the tests should pass.

In more detail, the console output shows the experiments being run, including the full command line, whether the outputs match expectations (for number of flows finished and their tail flow completion time), as well as summaries. Take for instance the output for the first experiment:

```
Experiment: Single Connection 
==========================================
Running ./htsim_uec -tm connection_matrices/one.cm -end 1000 -sender_cc_only 
[PASS] Tail FCT 176.2 us below the target of 190 us
[PASS] Connection count 1
FCT Spread 176.2 -> 176.2 ratio 1.0
Summary: New: 490 Rtx: 0 RTS: 0 Bounced: 0 ACKs: 124 NACKs: 0 Pulls: 0 sleek_pkts: 0
Using binary: ./htsim_uec
```

There are other validation scripts you can similarly run such as validate_uec_rcv.txt (which uses the receiver-driven congestion control scheme) and validate_load_balancing_failed_snd.txt

## Running Custom Scenarios

The UEC simulation binary is called `htsim_uec` and is located in `sim/datacenter/htsim_uec`.
To run a custom setup, a traffic/connection matrix must be provided.

You can run a single network connection using UEC CMS as follows:

```bash
./htsim_uec -tm connection_matrices/one.cm
```
The output consists of two major parts: the configuration and setup section, and the runtime section.
The first part of the output shows the configured/derived/default parameter settings and values used by htsim.
It is important to verify that all the parameters are indeed accepted as expected when using custom configurations.
The section part of the output starts with the `Starting simulation` line and displays per flow information.

```
Starting simulation
Flow Uec_0_13 flowId 1000000001 uecSrc 0 starting at 0
Flow Uec_0_13 flowId 1000000001 uecSrc 0 finished at 176.2 total messages 1 total packets 490 RTS 0 total bytes 2002140 in_flight now 0 fair_inc 0 prop_inc 15978 fast_inc 519430 eta_inc 9321 multi_dec -0 quick_dec -0 nack_dec -0
.Done
New: 490 Rtx: 0 RTS: 0 Bounced: 0 ACKs: 124 NACKs: 0 Pulls: 0 sleek_pkts: 0
```
In this specific example, it displays the single flow's start and end information, including the flow completion time and details on the specific congestion control mechanism used.
The last line shows a summary of the run, starting with the total number of packets sent, retransmissions, control messages, ACKs, etc.

To get more details, the `-debug` flag increases the output and shows more details on the active congestion control mechanism.

You can run more involved traffic patterns by using different connection matrices. Examples connection matrices can be found in `sim/datacenter/connection_matrices`. For instance, a connection matrix for a 15 node incast (called incast.cm) looks like this:
```
Nodes 16
Connections 15
0->15 start 0 size 2000000
1->15 start 0 size 2000000
2->15 start 0 size 2000000
3->15 start 0 size 2000000
4->15 start 0 size 2000000
5->15 start 0 size 2000000
6->15 start 0 size 2000000
7->15 start 0 size 2000000
8->15 start 0 size 2000000
9->15 start 0 size 2000000
10->15 start 0 size 2000000
11->15 start 0 size 2000000
12->15 start 0 size 2000000
13->15 start 0 size 2000000
14->15 start 0 size 2000000
```

A second important, but optional parameter is the topology specification. 
At this point, only folded Clos topologies with two or three switch layers are supported.

If no topology-related parameters are provided, htsim uses default parameter values to create a three tier FatTree topology that has a number of endpoints matching the connection matrix.
Custom parameters can be provided as command line parameters or through topology files.  The latter are the preferred method.

Here is an example of a custom topology file for a two tier topology with 32 endpoints, 100Gbps links, oversubscribed 4 to 1 at the spine layer:

```
Nodes 32
Tiers 2
Podsize 32

Tier 0
Downlink_speed_Gbps 100
Radix_Down 8
Radix_Up 2
Downlink_Latency_ns 1000
Switch_Latency_ns 0
Oversubscribed 4

Tier 1
Downlink_speed_Gbps 100
Radix_Down 4
Downlink_Latency_ns 1000
Switch_Latency_ns 0
```
To use a custom topology, we specify it using the -topo parameter.
```bash
./htsim_uec -tm connection_matrices/perm_32n_32c_2MB.cm -topo topologies/leaf_spine_tiny.topo
```
The `datacenter/topologies` folder contains more examples of topologies.

## Exercise 1: How many paths do you need for good performance?
To understand the effect of collisions, we will be using a fully provisioned leaf spine topology with 1024 nodes and a permutation traffic matrix. We can specify the number of paths each connection is allowed to use by passing the -paths parameter. The default value is 64.

For instance, to run with 128 paths per connection, you can run.
```
./htsim_uec -topo topologies/leaf_spine_1024.topo -tm connection_matrices/perm_1024n_1024c_0u_2000000b.cm -paths 128 | grep finished > 128.dat
```
To parse the results, we have filtered for output lines containing the word "finished" and saved them to 128.dat file.

To visualise the CDF of the flow completion times, you can use gnuplot (or your favourite visualisation tool):
plot "128.dat" using 9:($0/10.24)

__Task__: vary the number of paths from 1 to a large value and plot the evolution of the tail flow completion time. How many paths are sufficient before performance stops improving?

## Exercise 2: Which load balancing algorithm is better?

The UEC model supports a number of load balancing algorithms including Oblivious, Bitmap, REPS and the combination of REPS and Bitmap (which is the default). You can change the load balancing algorithm by passing the '-load_balancing_algo' algorithm as follows.

```
./htsim_uec -topo topologies/leaf_spine_1024.topo -tm connection_matrices/perm_1024n_1024c_0u_2000000b.cm -paths 128 -load_balancing_algo reps| grep finished >  128_reps.dat
```

By default, the UEC receiver generates one cumulative ACK at for every 16KB of data received. This reduces the performance of the REPS algorithm, and can influence other load balancing algorithms too. To force one SACK per received packet you can pass the -sack_threshold 4000 in the command line.

Use the leaf-spine topology for this task (-topo topologies/leaf_spine_1024.topo)

__Task__: Draw CDF of flow completion times for bitmap, REPS (with the default 16KB SACK threshold) and with SACK per packet), as well as for the mixed (bitmap and REPS). 

Also test the oblivious load balancing algorithm with default queue sizes, but also with larger queue sizes (hint change the maximum queue sizes per port by passing the -q followed by the queue size in MTU. By default, the queue sizes are set to 1BDP (around 40 packets for 100Gbps). Try setting it to 100 packets.  

__Task__: Repeat the task above but introduce a soft failure by passing "-failed 1" to the simulator. This will drop one link from each TOR to a spine to 25% capacity, simulating an asymmetric network. 

__Task__: Repeat the task above but use a three tier topology instead that has the same number of nodes. Hint: you can use a custom three tier topology file, or omit the -topo parameter which makes the simulator default to a 3 tier fully provisioned topology.

## Exercise 3: Build a new load balancing algorithm

We have created a stub load balancing algorithm for you to play with in sim/lb_sigcomm.h and sim/lb_sigcomm.cpp
The algorithm can be enabled by passing -load_balancing_algo sigcomm to the simulator.

The algorithm has a constructor which initializes local state (if any), and two methods which are called by the transport protocol:

```
    SigcommLoadBalancing(uint16_t no_of_paths, bool debug);
```
The no_of_paths parameter specifies how many paths can be concurently used by the load balancer at any point in time. You can ignore the debug flag.

```
    void processEv(uint16_t path_id, PathFeedback feedback) override;
```
If called whenever an ACK, NACK is received or a timeout fires for the given path_id. The PathFeedback specifies the event (can be PATH_GOOD, PATH_ECN, PATH_NACK or PATH_TIMEOUT. If you are building a stateful load balancer, use this information to update you local path state. 

```
    uint16_t nextEntropy(uint64_t seq_sent, uint64_t cur_cwnd_in_pkts) override;
```
This is called when a packet is being prepared to be sent. Use local state to select the best path!

__Task__: Design and implement a new load balancing algorithm by completing the above functions.

__Task__: Measure the FCT of your algorithm to the existing algorithms such as REPS or Bitmap for a permutation traffic matrix (e.g. similar to the tasks at Exercise 2). 

## Default Parameters

If no parameters are provided, the defaults aim to follow the UEC specification defaults where it makes sense:

- congestion control mechanism: NSCC
  - Key parameters for NSCC such as network RTT, BDP, etc. are automatically derived
- topology: 3-tier fat tree, 12us RTT
- link speed: 100Gbps
- packet trimming is enabled


# Generating validation files for other network speeds.

```bash
python3 generate_permutation_experiments.py 800 NSCC > nscc_800gbps_test.txt
```

# Repository

The repository layout is as follows:

- `sim` the main congestion control simulation files
- `sim/datacenter` simulation scenarios, topologies, binaries, and validation scripts
- `sim/datacenter/connection_matrices` collection of connection matrices used by the validation scripts as well as Python scripts to generate them
- `sim/datacenter/topologies` collection of connection matrices used by the validation scripts

In addition to the UEC congestion management code, the repository contains a wide range of other network protocols.
These are currently not maintained and there is no expectation for any of them to work correctly or at all.

## About htsim Network Simulator
htsim is a high-performance discrete event simulator, inspired by ns2, but much faster, primarily intended to examine congestion control algorithm behaviour.  It was originally written by [Mark Handley](http://www0.cs.ucl.ac.uk/staff/M.Handley/) to allow [Damon Wishik](https://www.cl.cam.ac.uk/~djw1005/) to examine TCP stability issues when large numbers of flows are multiplexed.  It was extended by [Costin Raiciu](http://nets.cs.pub.ro/~costin/) to examine [Multipath TCP performance](http://nets.cs.pub.ro/~costin/files/mptcp-nsdi.pdf) during the MPTCP standardization process, and models of datacentre networks were added to [examine multipath transport](http://nets.cs.pub.ro/~costin/files/mptcp_dc_sigcomm.pdf) in a variety of datacentre topologies.  [NDP](http://nets.cs.pub.ro/~costin/files/ndp.pdf) was developed using htsim, and simple models of DCTCP, DCQCN were added for comparison.  Later htsim was adopted by Correct Networks (now part of Broadcom) to develop [EQDS](http://nets.cs.pub.ro/~costin/files/eqds.pdf), and switch models were improved to allow a variety of forwarding methods.  Support for a simple RoCE model, PFC, Swift and HPCC were added.


