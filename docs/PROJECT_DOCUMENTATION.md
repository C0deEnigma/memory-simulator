# Memory Management Simulator  
## Project Documentation

---

## 1. Introduction

This project implements a **Memory Management Simulator** in C++, designed to model and analyze core concepts of **Operating Systems memory management**. The simulator focuses on two major subsystems:

1. **Dynamic Memory Allocation**
2. **Multilevel CPU Cache Hierarchy**

The goal of the project is not to replicate hardware-level behavior with cycle accuracy, but to provide a **clear, logically correct, and extensible simulation** of how memory allocation strategies and cache hierarchies behave under different access patterns.

The simulator emphasizes:
- correctness of algorithms
- clarity of design decisions
- reproducible testing

---

### 1.1 Objectives

The primary objectives of this project are:

- To simulate **variable-sized memory partitioning** with realistic allocation strategies
- To study **external fragmentation** and allocation failures
- To implement a **multilevel cache hierarchy** (L1 and L2)
- To analyze **cache hit/miss behavior** under different replacement policies
- To provide **quantitative metrics** for both memory and cache subsystems
- To validate behavior using **automated test cases**

---

### 1.2 Scope of the Simulator

The simulator includes:

- A dynamic memory allocator with:
  - First Fit, Best Fit, and Worst Fit strategies
  - Block splitting and coalescing
  - Fragmentation and utilization metrics
  - Interactive command-line interface

- A cache simulator with:
  - Two cache levels (L1 and L2)
  - Configurable cache parameters
  - FIFO and LRU replacement policies
  - Exclusive cache hierarchy
  - Automated testing (no interactive CLI)

---

### 1.3 Design Philosophy

The simulator is designed with the following goals in mind:

- **Clear state representation**  
  The current state of memory blocks and cache lines is stored explicitly, so behavior is easy to observe and debug.

- **Simple and readable design**  
  Only necessary abstractions are used, keeping the implementation easy to understand and modify.

- **Test-based verification**  
  Automated test cases are used to verify correctness instead of relying only on manual testing.

- **Predictable behavior**  
  Given the same sequence of operations, the simulator always produces the same results.

---

### 1.4 Intended Audience

This project is intended for:
- Undergraduate students studying Operating Systems
- Learners exploring memory allocation and cache behavior
- Instructors requiring a demonstrable OS simulation
- Developers interested in algorithmic trade-offs in memory systems

No prior knowledge of hardware-level cache design is assumed.

---


## 2. Dynamic Memory Allocation Simulator

This component simulates **variable-sized memory allocation** as used in operating systems that rely on contiguous memory allocation without paging.

---

### 2.1 Memory Model

Physical memory is simulated as a **contiguous address space** of fixed size specified at program start.

Memory is divided into **blocks**, where each block represents a contiguous segment of memory.  
Blocks are stored in a **linked list**, preserving their physical order in memory.

Each memory block contains:
- `start` : starting address of the block
- `size`  : size of the block
- `free`  : allocation status
- `id`    : allocation identifier (`-1` if the block is free)

A linked list is used because:
- Blocks are created and removed dynamically
- Splitting and merging blocks is frequent
- Maintaining physical adjacency is essential for coalescing

---

### 2.2 Allocation Identifiers

Each allocated block is assigned a **unique integer ID**.

This ID is returned to the user on allocation and is later used for deallocation.

Using block IDs instead of raw addresses:
- simplifies the command-line interface
- avoids ambiguity during deallocation
- keeps the simulator independent of real memory addresses

---

### 2.3 Allocation Strategies

The allocator supports the following strategies:

#### First Fit
Traverses the block list from the beginning and allocates the first free block large enough to satisfy the request.

#### Best Fit
Searches all free blocks and selects the smallest block that can satisfy the request, reducing leftover free space.

#### Worst Fit
Selects the largest available free block, attempting to reduce future fragmentation of medium-sized blocks.

All strategies share the same allocation mechanics once a suitable block is chosen.

---

### 2.4 Block Splitting

If a selected free block is **larger than the requested size**, it is split into:
1. An allocated block of the requested size
2. A remaining free block

The remaining free block is inserted **immediately after** the allocated block in the list.

This preserves:
- correct memory ordering
- adjacency relationships for future coalescing

---

### 2.5 Deallocation

When a block is freed:
- its `free` flag is set to true
- its allocation ID is reset
- adjacent blocks are checked for possible merging

If the given ID does not exist, the request is ignored safely.

---

### 2.6 Coalescing of Free Blocks

To reduce external fragmentation, **adjacent free blocks are merged** after deallocation.

Coalescing is performed in the following order:
1. Previous block
2. Next block

#### Reason for coalescing the previous block first

Merging with the previous block first ensures:
- the leftmost block is preserved
- iterators remain valid after deletion
- correct physical ordering of memory blocks

This approach avoids unnecessary list traversal and prevents iterator invalidation errors.

Coalescing is performed **locally**, only around the freed block, since fragmentation effects are localized.

---

### 2.7 Fragmentation Metrics

#### Internal Fragmentation

Internal Fragmentation = 0

Reason:
- The allocator uses variable-sized partitions
- Blocks are allocated with exact requested size
- No unused space exists inside allocated blocks

---

#### External Fragmentation

External fragmentation measures how scattered free memory is.

It is computed as: (total_free_memory - largest_free_block) / total_free_memory


A high value indicates that although enough memory exists, it may not be usable for large allocations.

---

### 2.8 Allocation Failure Rate

The allocator tracks:
- total allocation requests
- failed allocation attempts

The failure rate is computed as: failed_requests / total_requests


This helps quantify the impact of fragmentation on allocation success.

---

### 2.9 Allocator Interface

An interactive command-line interface is provided to:
- issue allocation and deallocation requests
- visualize memory layout
- inspect fragmentation statistics

This interface is intended for **exploration and debugging**, while correctness is verified using automated tests.

---

## 3. Allocator Testing Strategy

The correctness of the memory allocator is verified using **automated test cases**, implemented separately from the interactive command-line interface.

Testing focuses on validating:
- correctness of allocation strategies
- block splitting behavior
- coalescing of free blocks
- fragmentation metrics
- allocation failure handling

Each test is designed to be **independent and deterministic**.

---

### 3.1 Test Design Principles

The allocator tests follow these principles:

- **Independent test cases**  
  Each test creates its own allocator instance to avoid shared state.

- **Deterministic sequences**  
  Allocation and deallocation sequences are fixed, ensuring reproducible results.

- **Observable outcomes**  
  Memory layout and statistics are printed and manually verifiable.

- **Edge-case coverage**  
  Tests include allocation failures, full memory usage, and fragmentation scenarios.

---

### 3.2 Tested Scenarios

The following scenarios are explicitly tested:

#### Basic Allocation
- Sequential allocations using First Fit
- Verification of correct block placement and IDs

#### Strategy Comparison
- First Fit, Best Fit, and Worst Fit behavior
- Verification that each strategy selects the correct free block

#### Block Splitting
- Allocation smaller than free block size
- Correct creation of remaining free blocks

#### Deallocation and Coalescing
- Freeing blocks in different orders
- Merging of adjacent free blocks
- Verification that non-adjacent blocks are not merged

#### Fragmentation Analysis
- External fragmentation after multiple allocations and frees
- Validation of largest free block calculation

#### Allocation Failure
- Allocation requests exceeding available contiguous memory
- Handling of zero-sized allocation requests
- Correct failure rate reporting

---

### 3.3 Test Implementation

Allocator tests are implemented in: tests/test_allocator.cpp


Each test function:
- executes a predefined sequence of operations
- prints the memory layout using `dumpMemory()`
- prints statistics using `printStats()`

Sample test outputs are stored in the `docs/` directory for reference and documentation purposes.

---

### 3.4 Manual Verification

Although tests are automated, verification is performed by:
- comparing memory dumps before and after operations
- checking block boundaries and IDs
- validating fragmentation percentages and failure rates

This approach ensures both **logical correctness** and **interpretability** of results.

---

### 3.5 Rationale for Test-Based Validation

Automated tests are preferred over relying solely on the CLI because:
- they ensure consistent behavior across runs
- they allow focused testing of specific scenarios
- they reduce human error during verification

The CLI is retained primarily for interactive exploration, while tests serve as the primary validation mechanism.

---

## 4. Cache Simulator Design

This component simulates a **multilevel CPU cache hierarchy** to study cache behavior under different access patterns and replacement policies.

The simulator models a simplified but realistic cache system, focusing on correctness of logic rather than hardware-level timing accuracy.

---

### 4.1 Cache Hierarchy

The simulator implements a **two-level cache hierarchy**:

- **L1 Cache**
- **L2 Cache**

Each memory access is processed in the following order:
1. L1 cache
2. L2 cache
3. Main memory (on miss in both levels)

---

### 4.2 Exclusive Cache Design

The cache hierarchy follows an **exclusive cache policy**.

Under this design:
- A memory block exists in **only one cache level at a time**
- If a block is promoted from L2 to L1, it is removed from L2
- If a block is evicted from L1, it is demoted to L2

#### Reason for choosing an exclusive design

- Avoids duplication of cache blocks across levels
- Makes demotion and promotion behavior explicit
- Reduces total cache storage redundancy

---

### 4.3 Cache Configuration Parameters

Each cache level is configurable with:

- Cache size
- Block size
- Associativity
- Replacement policy (FIFO or LRU)

The number of sets in a cache is computed as: number_of_sets = cache_size / (block_size × associativity)


This allows the simulator to model:
- Direct-mapped caches (associativity = 1)
- Set-associative caches
- Fully associative caches (single set)

---

### 4.4 Cache Data Structures

Each cache level is implemented as:
- A vector of sets
- Each set is represented using a `deque<int>`

Each deque stores **block addresses**, not raw memory addresses.

#### Reason for storing block addresses

- Cache behavior depends on block granularity, not byte-level addresses
- Block addresses uniquely identify cache lines
- Simplifies eviction, demotion, and promotion logic
- Avoids recomputation during cache operations

This design also keeps cache logic independent of raw memory addressing details.

---

### 4.5 Cache Access Logic

For each memory access:

1. **L1 Access**
   - If the block is found, it is a hit
   - Replacement policy is applied if needed (LRU update)

2. **L1 Miss → L2 Access**
   - If the block is found in L2:
     - The block is promoted to L1
     - It is removed from L2
     - Any evicted L1 block is demoted to L2

3. **Miss in L1 and L2**
   - The block is inserted into L1
   - Any evicted L1 block is demoted to L2

This models realistic cache miss propagation across levels.

---

### 4.6 Replacement Policies

Each cache level supports the following replacement policies:

#### FIFO (First-In, First-Out)
- The oldest block in the set is evicted
- Implemented by removing from the front of the deque

#### LRU (Least Recently Used)
- Recently accessed blocks are moved to the back of the deque
- The least recently used block is evicted

Both policies share the same underlying data structure, differing only in update behavior during access.

---

### 4.7 Cache Statistics

Each cache level independently tracks:
- Number of hits
- Number of misses

The simulator reports:
- Hit ratio per cache level
- Miss propagation between cache levels

Miss propagation metrics include:
- L1 misses resulting in L2 access
- L2 misses resulting in main memory access

These statistics help analyze cache effectiveness and hierarchy behavior.

---

### 4.8 No Cache Command-Line Interface

Unlike the allocator, the cache simulator does **not provide an interactive CLI**.

Reason:
- Cache behavior depends on access patterns rather than individual commands
- Automated tests provide clearer and more reliable validation
- CLI-based testing would be less precise for cache analysis

All cache functionality is validated using structured test cases.

---

## 5. Cache Testing Strategy

The cache simulator is validated using **automated test cases** rather than interactive input.  
Testing focuses on verifying correctness of cache behavior under different access patterns and replacement policies.

---

### 5.1 Testing Goals

The cache tests are designed to verify:

- Correct hit and miss behavior at each cache level
- Proper promotion and demotion of blocks between L1 and L2
- Correct eviction according to FIFO and LRU policies
- Accurate tracking of cache statistics
- Correct propagation of misses to lower cache levels
- Proper reset of cache statistics between test cases

---

### 5.2 Test Design Principles

The following principles are followed:

- **Independent tests**  
  Each test constructs a fresh cache hierarchy to avoid shared state.

- **Controlled access patterns**  
  Address sequences are chosen carefully to trigger specific cache behaviors such as eviction and promotion.

- **Deterministic results**  
  The same access sequence always produces the same cache state and statistics.

---

### 5.3 Tested Scenarios

The following scenarios are explicitly covered:

#### Basic Cache Hits
- Verifies repeated access to the same block results in L1 hits
- Confirms correct hit ratio calculation

#### L1 Miss and L2 Miss
- Ensures blocks are inserted into L1 on cold misses
- Confirms miss propagation to lower levels

#### Eviction and Demotion
- Fills L1 beyond its associativity
- Verifies evicted blocks from L1 are correctly demoted to L2
- Confirms subsequent access hits in L2

#### FIFO Replacement Policy
- Ensures eviction follows insertion order
- Validates correct demotion behavior under FIFO

#### LRU Replacement Policy
- Ensures eviction follows least-recently-used order
- Confirms that recently accessed blocks are retained

#### Statistics Reset
- Verifies that hit and miss counters can be reset
- Ensures statistics after reset reflect only new accesses

---

### 5.4 Test Implementation

Cache tests are implemented in: tests/test_cache.cpp


Each test:
- Executes a predefined sequence of memory accesses
- Prints cache statistics using `printStats()`
- Compares results against expected behavior

Sample outputs are saved in the `docs/` directory for reference and documentation.

---

### 5.5 Rationale for Test-Based Validation

A command-line interface is not used for cache simulation because:
- Cache behavior depends on access sequences rather than isolated commands
- Automated tests provide clearer and more precise validation
- Test cases allow direct verification of replacement and demotion logic

This approach ensures the cache simulator is both **correct** and **easy to reason about**.

---

## 6. Cache Statistics and Miss Propagation

The cache simulator reports statistics separately for each cache level to analyze cache effectiveness and hierarchy behavior.

---

### 6.1 Cache-Level Statistics

For both L1 and L2 caches, the following metrics are tracked:

- Number of cache hits
- Number of cache misses
- Hit ratio

The hit ratio is computed as: hits / (hits + misses)

Statistics are maintained independently for each cache level.

---

### 6.2 Miss Propagation

Miss propagation describes how cache misses travel through the cache hierarchy.

The simulator reports:
- Number of L1 misses that result in L2 access
- Number of L2 misses that result in main memory access

This models the penalty of cache misses at higher levels and provides insight into overall memory access behavior.

---

### 6.3 Statistics Reset

Cache statistics can be reset programmatically to:
- isolate individual test cases
- prevent interference between test runs

Resetting statistics does not modify cache contents.

---

## 7. Design Decisions Summary

This section summarizes key design choices made during implementation, along with the rationale behind them.

---

### Variable-Sized Partitioning

The memory allocator uses variable-sized partitions instead of fixed partitions to:
- avoid internal fragmentation
- allocate memory exactly as requested
- model realistic contiguous memory allocation

---

### Linked List for Memory Blocks

Memory blocks are stored in a linked list to:
- support dynamic splitting and merging
- preserve physical adjacency of blocks
- allow efficient insertion and deletion during allocation and deallocation

---

### Local Coalescing Strategy

Free blocks are coalesced only with adjacent neighbors at deallocation time.

The previous block is coalesced before the next block to:
- preserve the leftmost block
- avoid iterator invalidation
- maintain correct memory ordering without re-traversal

---

### Block ID–Based Deallocation

Blocks are freed using allocation IDs rather than addresses to:
- simplify the user interface
- avoid ambiguity during deallocation
- keep the simulator independent of actual memory addresses

---

### Exclusive Cache Hierarchy

The cache simulator follows an exclusive L1–L2 design:
- blocks exist in only one cache level at a time
- promoted blocks are removed from lower levels
- evicted blocks from L1 are demoted to L2

This avoids redundancy and simplifies cache state management.

---

### Block Address Storage in Cache

Cache sets store block addresses instead of raw addresses to:
- represent cache behavior at block granularity
- simplify eviction and demotion logic
- keep cache logic independent of byte-level addressing

---

### Deque-Based Cache Sets

Each cache set is implemented using a `deque` to:
- support FIFO and LRU replacement policies
- maintain deterministic eviction order
- keep lookup overhead minimal due to low associativity

---

### Test-Driven Validation

Automated tests are used as the primary validation mechanism for both:
- memory allocator
- cache simulator

This ensures deterministic behavior and reliable verification across runs.

---

## 8. Conclusion

This project successfully implements a modular **Memory Management Simulator** that models key operating system concepts, including dynamic memory allocation and multilevel cache behavior.

The allocator demonstrates realistic allocation strategies, fragmentation handling, and deallocation mechanics, while the cache simulator accurately models cache hierarchy behavior with configurable replacement policies.

Through careful design choices and automated testing, the simulator achieves correctness, clarity, and reproducibility. The project serves as a practical tool for understanding memory management trade-offs and cache performance in operating systems.

---


