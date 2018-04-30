function check {
	if ! diff -q out.txt rmat15.solution &> /dev/null;
	then
		echo "Error Occurred. Solutions do not match."
	else
		echo "Solutions match."
	fi
}

function run {
	echo -n "bf_a - "
	./bf_a inputs/rmat15.dimacs 1 > "out.txt"
	check

	for t in 1 2 4 8
	do
		echo -n "bf_b $t - "
		./bf_b inputs/rmat15.dimacs $t 1 > "out.txt"
		check
	done

	echo -n "bf_d - "
	./bf_d inputs/rmat15.dimacs 1 > "out.txt"
	check

	for t in 1 2 4 8
	do
		echo -n "bf_d $t - "
		./bf_d inputs/rmat15.dimacs $t 1 > "out.txt"
		check
	done
}

make
echo "Beginning..."
run
echo "done."