import std;

using u64 = unsigned long long;

u64 GetJoltage(const std::vector<u64>& values, int batteries)
{
	u64 max = 0;
	for (int i = 0, index = 0, range_start = 0; i < batteries; ++i, index = 0)
	{
		u64 maxVal = 0;
		for (int j = range_start, maxJ = values.size() + i - batteries + 1; j < maxJ; ++j)
			if (values[j] > maxVal)
			{
				maxVal = values[j];
				index = j;
			}
		max = 10 * max + maxVal;
		range_start = index + 1;
	}
	return max;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day03.exe inputFilename\n";
		return -1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	u64 part1 = 0, part2 = 0;
	std::string line;
	while (in >> line)
	{
		std::vector<u64> values;
		for (char c : line)
			values.push_back(c - '0');
		
		part1 += GetJoltage(values, 2); 
		part2 += GetJoltage(values, 12);
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}