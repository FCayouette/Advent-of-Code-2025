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

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	u64 part1 = 0, part2 = 0;
	std::vector<std::vector<u64>> values;
	std::vector<std::string > cephMath;
	std::string line;
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

	std::vector<u64> vals;
	for (int i = cephMath.front().size()-1; i >= 0; --i)
	{
		u64 val = 0;
		for (size_t j = 0; j < cephMath.size() - 1; ++j)
			if (char c = cephMath[j][i]; c != ' ')
				val = val * 10 + c - '0';
		if (char c = cephMath.back()[i]; c == ' ')
		{
			if (val > 0)
				vals.push_back(val);
			else
				vals.clear();
		}
		else 
			part2 += (c == '+') ? std::accumulate(ALLc(vals), val) : std::accumulate(ALLc(vals), val, std::multiplies());
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}