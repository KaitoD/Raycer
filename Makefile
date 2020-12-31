raycer:
	g++ src/ray_main.cc -o raycer -Iinclude -O3 -fopenmp -march=native -mavx 

clean:
	rm raycer*