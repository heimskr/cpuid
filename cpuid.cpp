#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

unsigned int get_apic_id() {
	unsigned int edx;
	__asm__("cpuid" : "=d" (edx) : "a" (0xb));
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

	bool use_apic = 1 < argc && std::string(argv[1]) == "apic";
	for (int i = 0; i < 16; ++i) {
		threads.push_back(std::thread([&, i]() {
			std::unique_lock<std::mutex> lock {mutex};
			std::cout << "CPU[" << std::setw(2) << i << "]: " << (use_apic? get_apic_id() : get_core_id()) << "\n";
		}));
	}

	for (int i = 0; i < 16; ++i)
		threads[i].join();
}
