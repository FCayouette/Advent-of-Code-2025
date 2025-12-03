import std;

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day03.exe inputFilename\n";
		return -1;
	}

	u64 part1 = 0, part2 = 0;

	std::ifstream in(argv[1]);
	std::string line;
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	while (in >> line)
	{
		std::vector<u64> values;
		for (int i = 0; i < line.size(); ++i)
			values.push_back(line[i] - '0');
		u64 max = 0;
		for (int i = 0; i < line.size() - 1; ++i)
			if (max <= values[i] * 10 + 9)
				for (int j = i+1; j < line.size(); ++j)
					max = std::max(max, values[i] * 10 + values[j]);
		part1 += max;
		
		max = 0;
		for (int i = 0, range_start = 0; i < 12; ++i)
		{
			u64 maxVal = 0, index = 0;
			for (int j = range_start; j < values.size()+i-11; ++j)
				if (values[j] > maxVal)
				{
					maxVal = values[j];
					index = j;
				}
			max = 10 * max + maxVal;
			range_start = index + 1;
		}
		part2 += max;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}