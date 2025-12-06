import std;

#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day06.exe inputFilename\n";
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

	std::vector<std::vector<u64>> values;
	std::vector<std::string > cephMath;
	while (std::getline(in, line))
	{
		int i = 0;
		while (line[i] == ' ') ++i;
		std::basic_stringstream ss(line);
		if (std::isdigit(line[i]))
		{
			u64 v;
			std::vector<u64> vals;
			while (ss >> v)
				vals.push_back(v);
			values.emplace_back(std::move(vals));
		}
		else
		{
			char op;
			size_t row = 0;
			while (ss >> op)
			{
				if (op == '+')
					for (const auto& v : values)
						part1 += v[row];
				else
				{
					u64 col = 1;
					for (const auto& v : values)
						col *= v[row];
					part1 += col;
				}
				++row;
			}
		}
		cephMath.emplace_back(std::move(line));
	}

	int start = 0, end = 1, MAX = cephMath.front().size();
	while (start < MAX)
	{
		bool separator = false;
		while (end < MAX)
		{
			separator = true;
			for (const auto& cLine : cephMath)
				if (cLine[end] != ' ')
				{
					separator = false;
					++end;
					break;
				}
			if (separator) break;
		}
		--end;

		std::vector<u64> vals;

		for (int i = end; i >= start; --i)
		{
			u64 v = 0;
			for (int x = 0; x < cephMath.size() - 1; ++x)
				if (cephMath[x][i] != ' ')
					v = v * 10 + (cephMath[x][i] - '0');
			vals.push_back(v);
		}
		part2 += cephMath.back()[start] == '+' ? std::accumulate(ALLc(vals), 0ull) : std::accumulate(ALLc(vals), 1ull, std::multiplies());
		
		start = end + 2;
		end = start + 1;
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}