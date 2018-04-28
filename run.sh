make
export OMP_SCHEDULE="static,1"
./bf_b inputs/rmat15.dimacs 8 0
export OMP_SCHEDULE="static,8"
./bf_b inputs/rmat15.dimacs 8 0
export OMP_SCHEDULE="dynamic,32"
./bf_b inputs/rmat15.dimacs 8 0