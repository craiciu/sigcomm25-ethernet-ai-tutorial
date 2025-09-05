# Ethernet Networking for AI tutorial (Sigcomm 2025)

# Purpose and Scope

This repository starts from a fork of the UET transport simulation
project to which it adds instructions to help participants in the
Sigcomm 2025 Ethernet Networking for AI tutorial begin exploring the
behaviour of congestion control and load balancing algorithms design
for AI workloads.

The simulation works with synthetic traffic patterns called connection
matrices which specify flow start times and sizes; all flows are
assumed to be backlogged. The simulator does transport-level
simulation only.  The transport does not accurately model the UET
transport; rather it is an idealized sprayed transport that has
zero-RTT setup and that supports multiple load balancing algorithms,
congestion control algorithms, and topologies.

The simulator does not model computation, semantic level operations
(i.e. RDMA writes), colllective implementations or other higher level
constructs.

#About htsim

HTSIM is a high-performance discrete event simulator used for network
simulation.  It offers faster simulation methods compared to other
options, making it ideal for modeling and developing congestion
algorithms and new network protocols. The role of htsim in the Ultra
Ethernet Consortium (UEC) standards development is to support the
transport layer working group's work on congestion control mechanisms.

In UEC, htsim:
- provides a platform for continuous implementation and development of UEC transport layer.
- is used to simulate and run different topologies and scenarios, helping to identify issues in the current specifications and estimate the throughput and latency for given parameters like topology, flow matrix and congestion configuration.
- provides a reference for users and developers to run simulations with different configurable parameters for various scenarios and algorithms

htsim's role is deliberately focused on congestion control.

UEC's htsim is not:

- a complete implementation of the UEC transport specification.
- a standard in any way; specifically, it is not part of the official UEC standards release.
  While we aim to match the spec as closely as possible, there might be discrepancies between the UEC CMS specification and the simulator.
  Only the official CMS specification is significant, the simulator is not.


# Getting Started with the Tutorial

Check the [README](htsim/README.md) file in the `htsim/` folder.
