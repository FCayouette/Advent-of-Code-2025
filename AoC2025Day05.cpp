import std;

#define ALLc(x) (x).cbegin(),(x).cend()
using u64 = unsigned long long;

struct Range
{
	u64 min = {}, max = {};
	constexpr bool within(u64 v) const{	return min <= v && v <= max; }
};

[[nodiscard]] Range GetRange(std::string s)
{
	auto pos = s.find('-');
	return Range(std::stoull(s.substr(0, pos)), std::stoull(s.substr(pos + 1)));
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day05.exe inputFilename\n";
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

	std::vector<Range> ranges, merged;
	bool range = true;
	while (std::getline(in, line))
	{
		if (line.empty())
		{
			range = false;
			continue;
		}
		if (range)
			ranges.emplace_back(GetRange(std::move(line)));
		else
		{
			u64 val = std::stoull(line);
			for (const Range& r : ranges)
				if (r.within(val))
				{
					++part1;
					break;
				}
		}
	};
	
	while (merged.size() != ranges.size())
	{
		merged.clear();
		for (const Range& r : ranges)
		{
			bool found = false;
			for (Range& check : merged)
				if (!(r.max < check.min || check.max < r.min))
				{
					found = true;
					check.min = std::min(r.min, check.min);
					check.max = std::max(r.max, check.max);
					break;
				}
			if (!found)
				merged.push_back(r);
		}
		std::swap(merged, ranges);
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, std::accumulate(ALLc(ranges), ranges.size(), [](u64 t, const Range& r) { return t + r.max - r.min; }));
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}