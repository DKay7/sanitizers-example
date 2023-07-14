

build_dir := $(shell mkdir -p build)
CC=clang-16
CXX=clang++-16

build/result-dynamic: build/example-module.so build/example.o build/sanitizer.o 
	$(CXX) -o $@ -g -Wl,--export-dynamic  -ldl -I/usr/lib/llvm-16/lib/clang/16/include -fsanitize=address build/sanitizer.o build/example.o

build/sanitizer.o: sanitizer.cpp
	$(CXX) -g -I/usr/lib/llvm-16/lib/clang/16/include -c -fsanitize=address sanitizer.cpp -o $@

build/example.o: module-main.cpp
	$(CXX) -g -c -I/usr/lib/llvm-16/lib/clang/16/include -fsanitize-coverage=trace-pc-guard module-main.cpp -o $@
	$(CXX) -g -S -I/usr/lib/llvm-16/lib/clang/16/include -emit-llvm -fno-discard-value-names -fsanitize-coverage=trace-pc-guard module-main.cpp -o build/example.ll

build/example-module.so: example-module.cpp
	$(CXX) -shared -fPIC -I/usr/lib/llvm-16/lib/clang/16/include -fsanitize-coverage=trace-pc-guard  example-module.cpp -o $@

#--------------------------------------------------------------------------------

build/result: build/lib.so  build/sanitizer.o
	$(CXX) -o $@ -g -Wl,--export-dynamic  -ldl -I/usr/lib/llvm-16/lib/$(CC)/15.0.7/include -fsanitize=address build/example.o


clean:
	rm build/*
