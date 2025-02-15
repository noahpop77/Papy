to run benchmarks, invoke corresponding benchmark script from porject_root/benchmarks/
e.g.
```bash
your/path/to/Papy> cd benchmarks
your/path/to/Papy/benchmarks$> sudo benchmark_linux.sh -threads 1 -runs 4 -count 1000
```
On Linux, nginx is used as a http server\
On Windows, you need to `npm install` in `benchmarks/` first (express is used)

Why is this an entire script running several processes and not just bench.cpp*?\
The reason is that real world is a bit complicated and such benchmark is closer to what you would observe in real world\
Windows benchmark exist for fun, use Linux if you want real benchmarking\
*you can actually do the same realistic benchmark in cpp, but result will not be any different - you will still be io bound