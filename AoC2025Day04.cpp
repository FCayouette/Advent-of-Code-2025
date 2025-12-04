import std;

struct Point
{
	constexpr Point operator + (const Point& p) const { return Point(x + p.x, y + p.y); }

	template<typename U>
	bool Within(const U& grid) const { return x >= 0 && y >= 0 && x < grid.size() && y < grid[x].size(); }
	template<typename U>
	auto& GetAt(U& grid) const { return grid[x][y]; }
	template<typename U>
	const auto& GetAt(const U& grid) const { return grid[x][y]; }

	int x = {}, y = {};
};
constexpr std::array<Point, 8> directions = { Point(-1, -1), Point(-1, 0), Point(-1, 1), Point(0,-1), Point(0, 1), Point(1, -1), Point(1, 0), Point(1,1) };


int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day04.exe inputFilename\n";
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

	std::vector<std::string> map, next;
	while (in >> line)
		map.emplace_back(std::move(line));
	next = map;

	int round = 0;
	bool found;
	do 
	{
		found = false;
		for (int i = 0; i < map.size(); ++i)
			for (int j = 0; j < map.front().size(); ++j)
				if (Point p(i, j); p.GetAt(map) == '@')
				{
					int count = 0;
					for (Point d : directions)
						if (Point np = p + d; np.Within(map) && np.GetAt(map) == '@' && ++count >= 4) 
							break;
					if (count < 4)
					{
						found = true;
						++part2;
						if (round == 0)
							++part1;
						p.GetAt(next) = '.';
					}
					else
						p.GetAt(next) = '@';
				}
				else p.GetAt(next) = '.';
		std::swap(map, next);
		++round;
	} while (found);

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}