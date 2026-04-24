watchexec -c -- "cmake -B build && cmake --build build && ctest --test-dir build/sysl --output-on-failure"
