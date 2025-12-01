import std;

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day01.exe inputFilename\n";
		return -1;
	}

	int part1 = 0, part2 = 0;

	std::ifstream in(argv[1]);
	std::string line;
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	int pos = 50;
	while (in >> line)
	{
		int delta = std::stoi(line.substr(1));
		part2 += delta / 100;
		delta %= 100;
		if (line[0] == 'L')
		{
			if (pos)
			{
				pos -= delta;
				if (pos <= 0)
					++part2;
				if (pos < 0)
					pos += 100;
			}
			else
				pos = 100 - delta;
		}
		else
		{
			pos += delta;
			if (pos >= 100)
			{
				++part2;
				pos -= 100;
			}
		}
		if (!pos) ++part1;
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}