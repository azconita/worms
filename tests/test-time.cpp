#include <chrono>
#include <iostream>
#include <thread>

int main() {
	using delta = std::chrono::duration<double, std::nano> ;
	std::chrono::nanoseconds diff(0);
	auto start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::nanoseconds(17000000)-diff);
	auto end = std::chrono::high_resolution_clock::now();
	//delta = end - start - diff;
	//std::chrono::duration<double, std::ratio<1, 1000>> elapsed_miliseconds = end-start;
	std::cout << "time: " << delta(end-start-diff).count() << "\n";
	diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start) - std::chrono::nanoseconds(17000000);

	std::cout << "time: " << diff.count() << "\n";

	start = std::chrono::high_resolution_clock::now();
	std::this_thread::sleep_for(std::chrono::nanoseconds(17000000)-diff);
	end = std::chrono::high_resolution_clock::now();

	diff = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start) - std::chrono::nanoseconds(17000000);
	std::cout << "time: " << diff.count() << "\n";
	diff = (diff.count() > 0) ? diff : std::chrono::nanoseconds(0);
	std::cout << "diff: " << diff.count() << "\n";
}
