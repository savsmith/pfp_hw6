# pfp_hw6
source set_env_hw4.sh to set up icc.
make: builds files bf_a bf_b bf_d bf_e (No C as that part is done by using export OMP_SCHEDULE)
To run these executables:
for all these files given 1 for print will print out the SSSP. 0 for print will print out the exectime. 
bf_a <input.dimacs> <print? 0/1> 
bf_b <input.dimacs> <thread count> <print? 0/1> 
bf_d <input.dimacs> <print? 0/1> 
bf_e <input.dimacs> <thread count> <print? 0/1> 

You must export OMP_SCHEDULE to set those.  

source run.sh: Runs all the files. WARNING this takes almost over 2 hours to do. I think our code could've been more efficient
