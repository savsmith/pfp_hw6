function run {
	echo "Graph, Thread Count, schedule type, schedule num, Exec Time (nanoseconds)" >> "run.out"

	for g in wiki-example rmat15 rmat22 roadFLA roadNY
	do
		for t in 1 2 4 8
		do
			for s in "static" "dynamic"
			do
				for n in 1 8 32 128 512
				do
					echo -n $g, $t, $s, $n," " >> "run.out"
					export OMP_SCHEDULE=$s,$n
					./bf_b inputs/$g.dimacs $t 0 >> "run.out"
					echo "" >> "run.out"
				done
			done
		done
	done 
}

make
echo "Bellman Ford with OpenMP results" > "run.out"
run