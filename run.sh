make all

echo "Matrix size,Threads,Order,Time(s)" > results.csv

# m = {64,128,256,512,1024,2048}, t = {1,2,4,8,16}, order = {0..5}
for matrix in 64 128 256 512 1024 2048; do
  for threads in 1 2 4 8 16; do
    for order in {0..5}; do
      echo "Running size=$matrix threads=$threads order=$order..."
      TIME=$(LOOP_ORDER=$order ./loop_unfolding.bin $matrix $threads 0)
      echo "$matrix,$threads,$order,$TIME" >> results.csv
    done
  done
done

echo "Benchmark complete. Results saved to results.csv"
