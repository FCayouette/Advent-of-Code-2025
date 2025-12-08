import std;

using u64 = unsigned long long;

struct Point3D
{
	constexpr Point3D(int X = 0, int Y = 0, int Z = 0) : x(X), y(Y), z(Z) {}
	int x, y, z;
};

u64 SquareDist(const Point3D& a, const Point3D& b)
{
	u64 x = std::abs(a.x - b.x), y = std::abs(a.y - b.y), z = std::abs(a.z - b.z);
	return x * x + y * y + z * z;
}

struct DistInfo
{
	u64 dist;
	size_t index1, index2;
	bool operator >(const DistInfo& r) const { return dist > r.dist; }
};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day08.exe inputFilename\n";
		return -1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	u64 part1 = 1, part2 = 0;
	int x, y, z;
	char c;
	std::vector<Point3D> points;
	std::priority_queue<DistInfo, std::vector<DistInfo>, std::greater<DistInfo>> queue;
	while (in >> x >> c >> y >> c >> z)
	{
		Point3D p(x, y, z);
		for (size_t i = 0; i < points.size(); ++i)
			queue.emplace(SquareDist(points[i], p), i, points.size());
		points.emplace_back(p);
	}

	std::vector<std::unordered_set<size_t>> circuits;

	auto FindCircuit = [&circuits](size_t index)
		{
			for (size_t r = 0; r < circuits.size(); ++r)
				if (circuits[r].contains(index))
					return r;
			return circuits.size();
		};

	size_t left = points.size(), roundCount =0 ;
	while(true)
	{
		++roundCount;
		DistInfo di = queue.top();
		queue.pop();
		size_t a = FindCircuit(di.index1), b = FindCircuit(di.index2);
		if (a == b)
		{
			if (a == circuits.size())
			{
				std::unordered_set<size_t> n = { di.index1, di.index2 };
				circuits.emplace_back(std::move(n));
				left -= 2;
			}
		}
		else if (a == circuits.size())
		{
			circuits[b].insert(di.index1);
			--left;
		}
		else if (b == circuits.size())
		{
			circuits[a].insert(di.index2);
			--left;
		}
		else
		{
			circuits[a].merge(circuits[b]);
			if (b < circuits.size() - 1)
				std::swap(circuits[b], circuits[circuits.size() - 1]);
			circuits.resize(circuits.size() - 1);
		}

		if (roundCount == 1000)
		{
			std::vector<size_t> sizes;
			for (const auto& c : circuits)
				sizes.push_back(c.size());

			std::sort(sizes.begin(), sizes.end(), std::greater());
			part1 = sizes[0] * sizes[1] * sizes[2];
		}

		if (!left && circuits.size() == 1)
		{
			part2 = points[di.index1].x * points[di.index2].x;
			break;
		}
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}