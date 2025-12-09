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
	int x, y, minX = 1000000, maxX = 0;
	char c;
	std::vector<Point> points;
	std::map<u64, std::vector<u64>> outsideCorners;
	std::set<Point> limits, outside, cur, next;
	while (in >> x >> c >> y)
	{
		for (const Point& p : points)
		{
			int mx = std::min(x, p.x), mX = std::max(x, p.x), my = std::min(y, p.y), mY = std::max(y, p.y);
			u64 area = (u64)(mX - mx + 1) * (u64)(mY - my + 1);
			if (area > part1)
				part1 = area;
		}
		if (!points.empty())
		{
			if (x == points.back().x)
				for (int i = std::min(y, points.back().y); i <= std::max(y, points.back().y); ++i)
					limits.emplace(x, i);
			else
				for (int i = std::min(x, points.back().x); i <= std::max(x, points.back().x); ++i)
					limits.emplace(i, y);
		}
		points.emplace_back(x, y);
		minX = std::min(x, minX);
		maxX = std::max(x, maxX);
	}
	if (points.back().x == points.front().x)
		for (int i = std::min(points.front().y, points.back().y); i <= std::max(points.front().y, points.back().y); ++i)
			limits.emplace(x, i);
	else
		for (int i = std::min(points.front().x, points.back().x); i <= std::max(points.front().x, points.back().x); ++i)
			limits.emplace(i, y);
	
	Point seed = *limits.cbegin() + Point(-1, 0);
	outside.insert(seed);
	cur.insert(seed);
	while (!cur.empty())
	{
		for (Point p : cur)
			for (Point a : allDir)
				if (Point t = p + a; !limits.contains(t) && !outside.contains(t))
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
	outsideCorners.clear();
	for (Point p : outside)
		outsideCorners[p.x].push_back(p.y);

	std::vector<int> toDelete;
	for (auto iter = std::next(outsideCorners.cbegin()), prev = outsideCorners.cbegin(); iter != outsideCorners.cend(); ++iter, ++prev)
		if (prev->second.size() == iter->second.size() && prev->second == iter->second)
			toDelete.push_back(iter->first);

	for (int x : toDelete)
		outsideCorners.erase(x);
	
	for (int i = 0; i < points.size() - 1; ++i)
		for (int j = i + 1; j < points.size(); ++j)
		{
			int mx = std::min(points[i].x, points[j].x), mX = std::max(points[i].x, points[j].x), my = std::min(points[i].y, points[j].y), mY = std::max(points[i].y, points[j].y);
			u64 area = (u64)(mX - mx + 1) * (u64)(mY - my + 1);
			bool valid = area > part2;
			for (u64 x = mx; x <= mX && valid; ++x)
				if (outsideCorners.contains(x))
					valid = std::find_if(ALLc(outsideCorners[x]), [my, mY](const u64 v) {return my <= v && v <= mY; }) == outsideCorners[x].cend();

			if (valid)
				part2 = std::max(part2, area);
		}
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}