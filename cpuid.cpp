#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

unsigned int get_x2apic_id() {
	unsigned int edx;
	// When calling cpuid with eax=0xb, it seems you need to have ebx=0 to prevent a segfault.
	__asm__("cpuid" : "=d" (edx) : "a" (0xb), "b" (0));
	// When you call cpuid with eax=0xb, the 32-bit value in edx contains the CPU ID.
	// This is useful for systems with 256 or more cores.
	return edx;
}

unsigned int get_core_id() {
	unsigned int ebx;
	__asm__("cpuid" : "=b" (ebx) : "a" (1));
	// When you call cpuid with eax=1, the upper 8 bits of ebx contain the CPU ID.
	return ebx >> 24;
}

int main(int argc, char **argv) {
	std::vector<std::thread> threads {};
	std::mutex mutex;

	const std::string arg = 1 < argc? argv[1] : "";

	if (arg == "both") {
		for (int i = 0; i < 16; ++i) {
			threads.push_back(std::thread([&, i]() {
				std::unique_lock<std::mutex> lock {mutex};
				std::cout << "CPU[" << std::setw(2) << i << "]: " << std::setw(2) << get_core_id() << " "
				          << get_x2apic_id() << "\n";
			}));
		}
	} else {
		bool use_x2apic = arg == "x2apic";
		for (int i = 0; i < 16; ++i) {
			threads.push_back(std::thread([&, i]() {
				std::unique_lock<std::mutex> lock {mutex};
				std::cout << "CPU[" << std::setw(2) << i << "]: " << (use_x2apic? get_x2apic_id() : get_core_id()) << "\n";
			}));
		}
	}

	for (int i = 0; i < 16; ++i)
		threads[i].join();
}
