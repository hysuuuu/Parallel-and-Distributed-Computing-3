make all


echo "Matrix size, Threads,Time (seconds)" > results.csv

# m = {64, 128, 256, 512, 1024, 2048}, t = {2, 4, 8, 16}.
for matrix in 64 128 256 512 1024 2048
do
    for threads in 1 2 4 8 16
    do
        echo "Running $threads threads and matrix size $matrix..."
        TIME=$(./loop_unfolding.bin $matrix $threads 0) 
        echo "$matrix, $threads, $TIME" >> results.csv
    done
done

echo "Benchmark complete. Results saved to results.csv"