import std;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

[[nodiscard]] std::vector<int> SplitAt(std::string s, char token)
{
	std::vector<int> results;
	if (s.empty()) [[unlikely]]
		return results;

	size_t tokenStart = 0;
	do
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(std::stoi(s.substr(tokenStart)));
			break;
		}
		results.emplace_back(std::stoi(s.substr(tokenStart, e - tokenStart)));
		tokenStart = e + 1;
	} while (tokenStart < s.size());

	return results;
}

template <typename T>
class y_combinator {
	T lambda;
public:
	constexpr y_combinator(T&& t) : lambda(std::forward<T>(t)) {}
	template <typename...Args>
	constexpr decltype(auto) operator()(Args&&...args) const { return lambda(std::move(*this), std::forward<Args>(args)...); }
};

int main(int argc, char* argv[])
{
	auto ChronoStart = std::chrono::high_resolution_clock::now();
	if (argc < 2)
	{
		std::cout << "Usage: AoC2025Day10.exe inputFilename\n";
		return -1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	int part1 = 0, part2 = 0;
	std::string line;

	while (in >> line)
	{
		int target = 0;
		std::string pattern = line.substr(1, line.size() - 2);
		for (int i = 0, x = 1; i < pattern.size(); ++i, x <<= 1)
			if (pattern[i] == '#')
				target += x;
		std::vector<int> flippers;
		while (in >> line)
		{
			if (line[0] == '{')
				break;
			int flip = 0;
			for (const auto& s : SplitAt(line.substr(1, line.size() - 1), ','))
				flip += (1 << s);
			flippers.push_back(flip);
		}

		std::set<int> cur = { 0 }, next, all = cur;
		int count = 0;
		while (!cur.contains(target))
		{
			++count;
			for (int i : cur)
				for (int t : flippers)
					if (int v = i ^ t; all.insert(v).second)
						next.insert(v);

			std::swap(cur, next);
			next.clear();
		}
		part1 += count;

		std::map<int, std::vector<std::vector<unsigned char>>> patterns = { {0,{{}}} };
		for (int i = 1, maxI = 1 << flippers.size(); i < maxI; ++i)
		{
			int index = 0, test = i, val = 0;
			std::vector<unsigned char> p;
			while (test > 0)
			{
				if (test & 0x01)
				{
					val ^= flippers[index];
					p.push_back(index);
				}
				test >>= 1;
				++index;
			}
			patterns[val].emplace_back(std::move(p));
		}
		std::vector<int> joltages = SplitAt(line.substr(1, line.size() - 1), ',');
		std::map<std::vector<int>, int> memo = { {std::vector<int>(pattern.size(), 0), 0} };

		auto Solve = y_combinator([&](auto&& Solve, std::vector<int> toMatch) -> int
			{
				if (auto iter = memo.find(toMatch); iter != memo.cend())
					return iter->second;
				int bestResult = 1'000'000;
				if (auto iter = std::find_if(toMatch.cbegin(), toMatch.cend(), [](int x) {return x < 0; }); iter != toMatch.cend())
				{
					memo[toMatch] = bestResult;
					return bestResult;
				}
				int oddPattern = 0;
				for (int i = 0; i < toMatch.size(); ++i)
					if (toMatch[i] & 0x01)
						oddPattern |= (1 << i);

				for (const std::vector<unsigned char>& p : patterns[oddPattern])
				{
					std::vector copy = toMatch;
					for (unsigned char c : p)
					{
						int t = flippers[c], index = 0;
						while (t > 0)
						{
							if (t & 0x01) --copy[index];
							t >>= 1;
							++index;
						}
					}
					for (int& x : copy)
						x >>= 1;
						
					bestResult = std::min(2 * Solve(std::move(copy)) + (int)p.size(), bestResult);
				}
				memo[toMatch] = bestResult;
				return bestResult;
			});
		
		part2 += Solve(joltages);
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}