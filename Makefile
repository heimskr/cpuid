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

xtest: cpuid
	./cpuid x2apic

xdbg: cpuid
	$(DEBUGGER) ./cpuid -- x2apic

btest: cpuid
	./cpuid both

bdbg: cpuid
	$(DEBUGGER) ./cpuid -- both
