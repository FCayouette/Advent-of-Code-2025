import std;

using u64 = unsigned long long;

struct Point3D
{
	constexpr Point3D(int X = 0, int Y = 0, int Z = 0) : x(X), y(Y), z(Z) {}
	constexpr u64 SquareDist(const Point3D& r) const {
		long long dx = r.x - x, dy = r.y - y, dz = r.z - z;
		return dx * dx + dy * dy + dz * dz;
	}
	int x, y, z;
};

struct DistInfo
{
	u64 dist;
	int index1, index2;
	bool operator >(const DistInfo& r) const { return dist > r.dist; }
};

template <typename T, std::enable_if_t<std::is_signed_v<T>, bool> = true>
struct DisjointSet
{
	DisjointSet(size_t size) : setInfo(static_cast<T>(size), -1), setCount(size) {}

	T Find(T x)
	{
		if (setInfo[x] < 0) return x;
		return setInfo[x] = Find(setInfo[x]);
	}

	void Union(T r1, T r2)
	{
		if (T i1 = Find(r1), i2 = Find(r2); i1 != i2)
		{
			setInfo[i1] += setInfo[i2];
			setInfo[i2] = i1;
			--setCount;
		}
	}
	
	std::vector<T> setInfo;
	size_t setCount;
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

	int part1 = 0, part2 = 0, x, y, z;
	char c;
	std::vector<Point3D> points;
	std::priority_queue<DistInfo, std::vector<DistInfo>, std::greater<DistInfo>> queue;
	while (in >> x >> c >> y >> c >> z)
	{
		Point3D p(x, y, z);
		for (size_t i = 0; i < points.size(); ++i)
			queue.emplace(p.SquareDist(points[i]), i, points.size());
		points.emplace_back(p);
	}

	size_t roundCount = 0;
	DisjointSet<int> disjoint(points.size());
	while(true)
	{
		++roundCount;
		DistInfo di = queue.top();
		queue.pop();

		disjoint.Union(di.index1, di.index2);
		
		if (roundCount == 1000)
		{
			std::vector copy = disjoint.setInfo;
			std::sort(copy.begin(), copy.end());
			part1 = std::accumulate(copy.cbegin(), copy.cbegin() + 3, -1ll, std::multiplies());
		}

		if (disjoint.setCount == 1)
		{
			part2 = points[di.index1].x * points[di.index2].x;
			break;
		}
	}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}