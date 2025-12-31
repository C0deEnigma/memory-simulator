# Memory Management Simulator

This project implements a simplified **Operating Systems Memory Management Simulator** in C++.  
It models core OS concepts including **dynamic memory allocation** and **multilevel CPU cache behavior**.

The project is divided into modular components with a clear separation between implementation, testing, and documentation.

---

## Features

### Memory Allocation Simulator
- Variable-sized partition allocation
- Allocation strategies:
  - First Fit
  - Best Fit
  - Worst Fit
- Block splitting and coalescing
- External fragmentation handling
- Allocation statistics and memory utilization metrics
- Interactive command-line interface

### Cache Simulator
- Multilevel cache hierarchy (L1 and L2)
- Configurable cache size, block size, and associativity
- Replacement policies:
  - FIFO
  - LRU
- Exclusive cache design (L1–L2)
- Cache hit/miss tracking
- Miss penalty propagation analysis
- Automated test-based validation

---


