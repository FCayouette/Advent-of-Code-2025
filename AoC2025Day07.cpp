import std;

#define ALLc(x) (x).cbegin(),(x).cend()
using u64 = unsigned long long;

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day07.exe inputFilename\n";
		return -1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	u64 part1 = 0;
	std::string line;
	std::unordered_map<int, u64> paths, next;

	in >> line;
	int pos = line.find('S');

	paths[pos] = 1;

	auto Insert = [&next](int p, u64 val) ->void
		{
			if (auto iter = next.find(p); iter != next.end())
				iter->second += val;
			else next[p] = val;
		};
	while (in >> line)
	{
		for (auto [p, v] : paths)
		{
			if (line[p] == '^')
			{
				Insert(p - 1, v);
				Insert(p + 1, v);
				++part1;
			}
			else Insert(p, v);
		}

		std::swap(paths, next);
		next.clear();
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, std::accumulate(ALLc(paths), 0ull, [](u64 t, const auto& p) { return t + p.second; }));
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}