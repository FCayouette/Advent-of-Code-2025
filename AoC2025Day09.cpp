import std;

#define ALLc(x) (x).cbegin(),(x).cend()

using u64 = unsigned long long;
template <typename T>
constexpr bool IsEqual(T a, T b)
{
	if constexpr (std::is_floating_point_v<T>)
		return (a < b ? b - a : a - b) < static_cast<T>(0.000001);
	else
		return a == b;
}

template<typename T>
struct Coord
{
	constexpr bool operator <  (const Coord& p) const { if (x < p.x) return true; else if (p.x < x) return false; else return y < p.y; }
	constexpr Coord operator + (const Coord& p) const { return Coord(x + p.x, y + p.y); }
	constexpr bool operator==(const Coord& p) const { return IsEqual(x, p.x) && IsEqual(y, p.y); }
	T x = {}, y = {};
};
using Point = Coord<int>;
constexpr std::array<Point, 4> directions = { Point(1, 0), Point(0, 1), Point(-1, 0), Point(0,-1) };
constexpr std::array<Point, 8> allDir = { Point(-1, -1), Point(-1,0), Point(-1,1), Point(0,-1), Point(0,1), Point(1,-1), Point(1,0), Point(1,1) };

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day09.exe inputFilename\n";
		return -1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}
	u64 part1 = 0, part2 = 0;
	int x, y;
	char c;
	std::vector<Point> points, compressed;
	std::map<u64, std::vector<u64>> outsideCorners;
	std::set<Point> limits, outside, cur, next;
	std::set<int> coordX, coordY;
	std::vector<int> compressedX, compressedY;
	std::map<int, int> xToCompressed, yToCompressed;

	while (in >> x >> c >> y)
	{
		for (const Point& p : points)
		{
			int mx = std::min(x, p.x), mX = std::max(x, p.x), my = std::min(y, p.y), mY = std::max(y, p.y);
			u64 area = (u64)(mX - mx + 1) * (u64)(mY - my + 1);
			if (area > part1)
				part1 = area;
		}
		points.emplace_back(x, y);
		coordX.insert(x);
		coordY.insert(y);
	}
	
	for (int x : coordX)
	{
		compressedX.emplace_back(x-1);
		xToCompressed[x] = compressedX.size();
		compressedX.emplace_back(x);
	}
	compressedX.emplace_back(*coordX.crbegin() + 1);
	for (int y : coordY)
	{
		compressedY.emplace_back(y-1);
		yToCompressed[y] = compressedY.size();
		compressedY.emplace_back(y);
	}
	compressedY.emplace_back(*coordY.crbegin() + 1);

	for (Point p : points)
		compressed.emplace_back(xToCompressed[p.x], yToCompressed[p.y]);

	for (int i = 1; i <= compressed.size(); ++i)
		if (Point a = compressed[i - 1], b = compressed[i % compressed.size()]; a.x == b.x)
			for (int y = std::min(a.y, b.y), maxY = std::max(a.y, b.y); y <= maxY; ++y)
				limits.emplace(a.x, y);
		else
			for (int x = std::min(a.x, b.x), maxX = std::max(a.x, b.x); x <= maxX; ++x)
				limits.emplace(x, a.y);

	Point seed = *limits.cbegin() + Point(-1, 0);
	outside.insert(seed);
	cur.insert(seed);
	while (!cur.empty())
	{
		for (Point p : cur)
			for (Point a : allDir)
				if (Point t = p+a; !limits.contains(t) && !outside.contains(t))
					for (Point d : directions)
						if (Point np = t + d; limits.contains(np))
						{
							next.insert(t);
							outside.insert(t);
							break;
						}
		std::swap(cur, next);
		next.clear();
	}

	for (Point p : outside)
		outsideCorners[p.x].push_back(p.y);

	std::vector<int> toDelete;
	for (auto iter = std::next(outsideCorners.cbegin()), prev = outsideCorners.cbegin(); iter != outsideCorners.cend(); ++iter, ++prev)
		if (prev->second.size() == iter->second.size() && prev->second == iter->second)
			toDelete.push_back(iter->first);

	for (int x : toDelete)
		outsideCorners.erase(x);

	for (int i = 0; i < compressed.size() - 1; ++i)
		for (int j = i + 1; j < compressed.size(); ++j)
		{
			int mx = std::min(compressed[i].x, compressed[j].x), mX = std::max(compressed[i].x, compressed[j].x), my = std::min(compressed[i].y, compressed[j].y), mY = std::max(compressed[i].y, compressed[j].y);
			u64 area = (u64)(compressedX[mX] - compressedX[mx] + 1) * (u64)(compressedY[mY] - compressedY[my] + 1);
			bool valid = area > part2;
			for (u64 x = mx; x <= mX && valid; ++x)
				if (outsideCorners.contains(x))
					valid = std::find_if(ALLc(outsideCorners[x]), [my, mY](const u64 v) {return my <= v && v <= mY; }) == outsideCorners[x].cend();

			if (valid && area > part2)
				part2 = area;
		}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}