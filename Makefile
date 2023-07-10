

build_dir := $(shell mkdir -p build)

build/result: build/example.o build/sanitizer.o
	clang++ -g  -I/usr/lib/llvm-15/lib/clang/15.0.7/include -fsanitize=address build/example.o build/sanitizer.o -o $@

build/sanitizer.o: sanitizer.cpp
	clang++ -g -I/usr/lib/llvm-15/lib/clang/15.0.7/include -c -fsanitize=address sanitizer.cpp -o $@

build/example.o: example.cpp
# clang++ -g -c -fsanitize-coverage=trace-pc -fsanitize-coverage=inline-8bit-counters -fsanitize-coverage=trace-loads example.cpp
# clang++ -g -S -emit-llvm -fsanitize-coverage=trace-pc -fsanitize-coverage=inline-8bit-counters -fsanitize-coverage=trace-loads example.cpp
	clang++ -g -c -I/usr/lib/llvm-15/lib/clang/15.0.7/include -fsanitize-coverage=trace-pc-guard example.cpp -o $@
	clang++ -g -S -I/usr/lib/llvm-15/lib/clang/15.0.7/include -emit-llvm -fno-discard-value-names -fsanitize-coverage=trace-pc-guard example.cpp -o build/example.ll

clean:
	rm build/*
