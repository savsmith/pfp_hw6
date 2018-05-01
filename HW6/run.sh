function run {
	# echo "Over Nodes"
	# echo "Dividing Over Nodes" >> "run.out"
	# echo "Graph, Thread Count, schedule type, schedule num, Exec Time (nanoseconds)" >> "run.out"

	# for g in wiki-example rmat15 rmat22 roadFLA roadNY
	# do
	# 	echo -n $g
	# 	echo -n $g, serial," N/A, N/A, " >> "run.out"
	# 	./bf_a inputs/$g.dimacs 0 >> "run.out"
	# 	echo "" >> "run.out"
	# 	for t in 1 2 4 8
	# 	do
	# 		for s in "static" "dynamic"
	# 		do
	# 			for n in 1 8 32 128 512
	# 			do
	# 				echo -n $g, $t, $s, $n," " >> "run.out"
	# 				export OMP_SCHEDULE=$s,$n
	# 				./bf_b inputs/$g.dimacs $t 0 >> "run.out"
	# 				echo "" >> "run.out"
	# 				echo -n "."
	# 			done
	# 		done
	# 	done
	# 	echo "done."
	# done 
	# echo "Over Edges"
	# echo "" >> "run.out"
	# echo "Dividing Over Edges" >> "run.out"
	# echo "Graph, Thread Count, schedule type, schedule num, Exec Time (nanoseconds)" >> "run.out"
	for g in rmat22 roadFLA roadNY
	do
		echo -n $g
		echo -n $g, serial," N/A, N/A, " >> "run.out"
		./bf_d inputs/$g.dimacs 0 >> "run.out"
		echo "" >> "run.out"
		for t in 1 2 4 8
		do
			for s in "static" "dynamic"
			do
				for n in 1 8 32 128 512
				do
					echo -n $g, $t, $s, $n," " >> "run.out"
					export OMP_SCHEDULE=$s,$n
					./bf_e inputs/$g.dimacs $t 0 >> "run.out"
					echo "" >> "run.out"
					echo -n "."
				done
			done
		done
		echo "done."
	done 
}

make
echo "Beginning..."
#echo "Bellman Ford with OpenMP results" > "run.out"
run
echo "done."