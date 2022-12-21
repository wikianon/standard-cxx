all : perf benchmark

CXXFLAGS=-O1 -g -fno-omit-frame-pointer

bench : bench_v_11 bench_v_98 bench_d_98 bench_d_11

perf : rep_v_98 rep_v_11 rep_d_98 rep_d_11

bench_v_11 : bench.cxx
	g++ bench.cxx $(CXXFLAGS) -std=c++11 -o bench_v_11

bench_v_98 : bench.cxx
	g++ bench.cxx $(CXXFLAGS) -std=c++98 -o bench_v_98

bench_d_11 : bench.cxx
	g++ bench.cxx $(CXXFLAGS) -DDEQUE -std=c++11 -o bench_d_11

bench_d_98 : bench.cxx
	g++ bench.cxx $(CXXFLAGS) -DDEQUE -std=c++98 -o bench_d_98

PERF=perf record -a --call-graph fp

rep_v_98 : bench_v_98
	$(PERF) -o rep_v_98 ./bench_v_98

rep_v_11 : bench_v_11
	$(PERF) -o rep_v_11 ./bench_v_11

rep_d_98 : bench_d_98
	$(PERF) -o rep_d_98 ./bench_d_98

rep_d_11 : bench_d_11
	$(PERF) -o rep_d_11 ./bench_d_11

FORMAT="real %e\nuser %U\nsys %S\nmem %M\n"

benchmark : bench_v_98 bench_v_11 bench_d_98 bench_d_11
	time -f $(FORMAT) ./bench_v_98
	time -f $(FORMAT) ./bench_v_11
	time -f $(FORMAT) ./bench_d_98
	time -f $(FORMAT) ./bench_d_11

PERF_REP=perf report -g -G --stdio
screen : perf
	screen $(PERF_REP) -i rep_v_98
	screen $(PERF_REP) -i rep_v_11
	screen $(PERF_REP) -i rep_d_98
	screen $(PERF_REP) -i rep_d_11

clean :
	rm -rf rep_{d,v}_{98,11} bench_{d,v}_{98,11}