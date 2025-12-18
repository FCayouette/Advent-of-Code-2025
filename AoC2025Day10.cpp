import std;

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
		std::string pattern = line.substr(1, line.size() - 2);
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

		std::map<int, std::vector<std::vector<int>>> deltas = { {0,{std::vector(pattern.size()+1, 0)}} };
		for (int i = 1, maxI = 1 << flippers.size(); i < maxI; ++i)
		{
			int index = 0, test = i, val = 0;
			std::vector<unsigned char> p;
			std::vector delta(pattern.size()+1, 0);
			delta.back() = std::popcount((unsigned int)test);
			while (test > 0)
			{
				if (test & 0x01)
				{
					int flip = flippers[index], fi = 0;
					val ^= flip;
					while (flip)
					{
						if (flip & 0x01)
							--delta[fi];
						++fi;
						flip >>= 1;
					}
				}
				test >>= 1;
				++index;
			}
			deltas[val].emplace_back(std::move(delta));
		}
		int target = std::accumulate(pattern.crbegin(), pattern.crend(), 0, [](int t, char c) { return t * 2 + (c == '#'); });
		const std::vector<std::vector<int>>& tp = deltas[target];
		part1 += std::accumulate(std::next(tp.cbegin()), tp.cend(), tp.front().back(), [](int m, const std::vector<int>& v) {return std::min(m, v.back()); });

		std::vector<int> joltages = SplitAt(line.substr(1, line.size() - 1), ',');
		std::map<std::vector<int>, int> memo = { {std::vector<int>(pattern.size(), 0), 0} };

		auto Solve = y_combinator([&](auto&& Solve, std::vector<int> toMatch) -> int
			{
				if (auto iter = memo.find(toMatch); iter != memo.cend())
					return iter->second;
				int bestResult = 1'000'000, oddPattern = 0;
				if (std::find_if(toMatch.cbegin(), toMatch.cend(), [](int x) { return x < 0; }) != toMatch.cend())
				{
					memo[toMatch] = bestResult;
					return bestResult;
				}
				oddPattern = std::accumulate(toMatch.crbegin(), toMatch.crend(), 0, [](int t, int c) {return t * 2 | (c & 0x00000001); });
				for (const std::vector<int>& d : deltas[oddPattern])
				{
					std::vector<int> copy;
					copy.reserve(toMatch.size());
					std::transform(toMatch.cbegin(), toMatch.cend(), d.cbegin(), std::back_inserter(copy), [](int c, int d) { return (c + d) / 2; });
					bestResult = std::min(2 * Solve(std::move(copy)) + d.back(), bestResult);
				}
				memo[toMatch] = bestResult;
				return bestResult;
			});

		part2 += Solve(joltages);
	}

	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, part2);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}