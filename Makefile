DEBUGGER := gdb

ifeq ($(shell uname -s), Darwin)
	DEBUGGER := lldb
endif

cpuid: cpuid.cpp
	g++ -std=c++17 cpuid.cpp -o cpuid -pthread

test: cpuid
	./cpuid

dbg: cpuid
	$(DEBUGGER) ./cpuid

atest: cpuid
	./cpuid apic

adbg: cpuid
	$(DEBUGGER) ./cpuid -- apic
