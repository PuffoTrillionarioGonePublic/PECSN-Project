# Project 8–Controltower

A control tower manages the air traffic of an airport. The airport has one runway for landing/take-off and one parking area for airplanes. The runway can be occupied by one airplane at a time, whereas the  parking  area  can  contain  one  or  more  of  them  simultaneously.  The  parking  area  have  infinite capacity.
Whenever an airplane reaches the airport area, it goes through the following steps:
1. the airplane queues for landing until it receives the ok from the control tower (assume that airplanes always have enough fuel to remain in the air for an arbitrary time);
2. the airplane performs the landing operation, which takes a time tl;
3. theairplane remains in the parking area for a time tp;
4. the airplane queues for take-off until it receives the ok from the control tower;
5. the airplane takes-off in a time to and leaves the system.

When the runway is unoccupied, the control tower serves one airplane according to the following policy: 
1. serve an airplane queued for landing;
2. if the landing queue is empty, serve an airplane in the take-off queue.

Consider the following workload: airplane interarrival times are IID RVs (to be described later), the values tl, to and tp are IID RV (to be described later).

Model the system described above and study the waiting time in both landing and take-off queues with a varying workload. Study how the number of airplanes in the parking area varies in relation with tl, tp. More in detail, at least the following scenarios must be evaluated:
- Constant interarrival times, constant service times. 
- Exponential distribution of all the above RVs, with the same means as the previous case.

In all cases, it is up to the team to calibrate the scenarios so that meaningful results are obtained.

Project deliverables:

a) Documentation (according to the standards set during the lectures)

b) Simulator code

c) Presentation (up to 10 slides maximum)
