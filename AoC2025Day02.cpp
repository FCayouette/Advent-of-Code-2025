import std;

#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

[[nodiscard]] std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	if (s.empty()) [[unlikely]]
		return results;

	size_t tokenStart = 0;
	do
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(s.substr(tokenStart));
			break;
		}
		results.emplace_back(s.substr(tokenStart, e - tokenStart));
		tokenStart = e + token.size();
	} while (tokenStart < s.size());

	return results;
}

std::set<u64> EvaluatePattern1(std::string min, std::string max)
{
	u64 upTo = std::stoull(max), minVal = std::stoull(min);
	std::set<u64> result;
	size_t size = min.size();
	if (size > 1)
		for (int i = 1; i < 10; ++i)
		{
			u64 v = i;
			for (int j = 1; j < size; ++j)
				v = v * 10 + i;
			if (v >= minVal && v <= upTo)
				result.insert(v);
		}

	if (min.size() < max.size())
	{
		size = max.size();
		for (int i = 1; i < 10; ++i)
		{
			u64 v = i;
			for (int j = 1; j < size; ++j)
				v = v * 10 + i;
			if (v <= upTo)
				result.insert(v);
			else break;
		}
	}

	return result;
}

std::set<u64> FindInvalid(std::string min, std::string max, int patternSize)
{
	size_t minSize = min.size(), maxSize = max.size();
	if (patternSize > 1 && minSize % patternSize != 0 && maxSize % patternSize != 0)
		return {};
	if (patternSize == 1)
		return EvaluatePattern1(min, max);

	std::set<u64> result;
	u64 upTo = std::stoull(max);
	if (minSize % patternSize == 0)
	{
		int repeat = minSize / patternSize;
		u64 factor = 1;
		for (int i = 0; i < patternSize; ++i)
			factor *= 10;
		u64 v = std::stoull(min.substr(0, patternSize)), minVal = std::stoull(min);
		while (true)
		{
			u64 test = v;
			for (int i = 1; i < repeat; ++i)
				test = test * factor + v;
			++v;
			if (test >= minVal)
			{
				if (test > upTo) break;
				result.insert(test);
			}
			if (v == factor) break;
		}
	}
	else
	{
		int repeat = maxSize / patternSize;
		u64 factor = 1;
		for (int i = 0; i < patternSize; ++i)
			factor *= 10;
		u64 v = factor / 10;
		while (true)
		{
			u64 test = v;
			for (int i = 1; i < repeat; ++i)
				test = test * factor + v;
			++v;
			if (test > upTo) break;
			result.insert(test);
			if (v == factor) break;
		}
	}

	return result;
}

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025DayXX.exe inputFilename\n";
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

	in >> line;
	auto ranges = SplitAt(std::move(line), ",");

	for (const auto r : ranges)
	{
		auto [min, max] = SplitAtFirst(r, "-");
		if (min.size() % 2 == 0)
		{
			u64 minVal = stoull(min), upTo = stoull(max);
			u64 factor = 1, size = min.size() / 2, half = std::stoull(min.substr(0, size));
			for (int i = 0; i < size; ++i)
				factor *= 10;
			while (true)
			{
				u64 test = half + half * factor;
				++half;
				if (test >= minVal)
				{
					if (test <= upTo)
						part1 += test;
					else break;
					if (half == factor) break;
				}
			}
		}
		else if (max.size() % 2 == 0)
		{
			u64 factor = 1, size = max.size() / 2, upTo = std::stoull(max);
			for (int i = 0; i < size; ++i)
				factor *= 10;
			u64 half = factor / 10;
			while (true)
			{
				u64 test = half + half * factor;
				++half;
				if (test > upTo) break;
				part1 += test;
				if (half == factor) break;
			}
		}

		std::set<u64> all;
		for (size_t i = std::max(min.size(), max.size()) / 2; i >= 1; --i)
			all.merge(FindInvalid(min, max, i));
		part2 += std::accumulate(ALLc(all), 0ull);
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}