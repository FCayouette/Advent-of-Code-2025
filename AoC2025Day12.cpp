import std;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
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

	std::vector<int> shapeSizes;
	while (std::getline(in, line))
	{
		if (line.empty()) continue;
		if (line[1] != ':') break;
		int size = 0;
		for (int i = 0; i < 3; ++i)
		{
			in >> line;
			size += std::count(line.cbegin(), line.cend(), '#');
		}
		shapeSizes.push_back(size);
	}

	do
	{
		auto [area, numbers] = SplitAtFirst(std::move(line), ": ");
		auto pos = area.find('x');
		int X = std::stoi(area.substr(0, pos)), Y = std::stoi(area.substr(pos + 1)), v, required = 0;
		std::stringstream ss(std::move(numbers));
		for (size_t i = 0; i < shapeSizes.size(); ++i)
		{
			ss >> v;
			required += v * shapeSizes[i];
		}
		if (required < X*Y)
			++part1;
	} while (std::getline(in, line));

	std::cout << std::format("Part 1: {}\n", part1);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}