import std;

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

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025DayXX.exe inputFilename\n";
		return -1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	int part1 = 0;
	std::string line;

	while (in >> line)
	{
		int target = 0;
		std::string pattern = line.substr(1, line.size() - 2);
		for (int i = 0, x = 1; i < pattern.size(); ++i, x <<= 1)
			if (pattern[i] == '#')
				target += x;
		std::vector<int> flippers;
		while (in >> line)
		{
			if (line[0] == '{')
				break;
			auto vals = SplitAt(line.substr(1, line.size() - 1), ",");
			int flip = 0;
			for (const auto& s : vals)
				flip += (1 << std::stoi(s));
			flippers.push_back(flip);
		}

		std::set<int> cur = { 0 }, next, all = cur;
		int count = 0;
		while (!cur.contains(target))
		{
			++count;
			for (int i : cur)
				for (int t : flippers)
					if (int v = i ^ t; all.insert(v).second)
						next.insert(v);

			std::swap(cur, next);
			next.clear();
		}
		part1 += count;
	}

	std::cout << std::format("Part 1: {}\n", part1);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}