import std;

using u64 = unsigned long long;
using i64 = long long;

[[nodiscard]] std::pair<std::string, std::string> SplitAtFirst(std::string s, std::string token)
{
	auto pos = s.find(token);
	if (pos == std::string::npos)
		return std::make_pair(std::move(s), std::string{});
	return std::make_pair(s.substr(0, pos), s.substr(pos + token.size()));
}

[[nodiscard]] std::vector<std::string> SplitAt(std::string s, std::string token)
{
	std::vector<std::string> results;
	if (s.empty()) [[unlikely]]
		return results;

	size_t tokenStart = 0;
	do
	{
		size_t e = s.find(token, tokenStart);
		if (e == std::string::npos)
		{
			results.emplace_back(s.substr(tokenStart));
			break;
		}
		results.emplace_back(s.substr(tokenStart, e - tokenStart));
		tokenStart = e + token.size();
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
		std::cout << "Usage: AoC2025Day11.exe inputFilename\n";
		return -1;
	}

	std::ifstream in(argv[1]);
	if (!in)
	{
		std::cout << std::format("Could not open file {}.\nAborting!\n", argv[1]);
		return -1;
	}

	std::string line;
	std::unordered_map<std::string, std::vector<std::string>> connections;
	std::unordered_map<std::string, u64> pathCounts;
	while (std::getline(in, line))
	{
		auto [name, o] = SplitAtFirst(std::move(line), ": ");
		connections.emplace(name,SplitAt(std::move(o), " "));
	}
	auto ResetPaths = [&pathCounts]()
		{
			pathCounts.clear();
			pathCounts["out"] = 1;
		};
	ResetPaths();

	auto PathsToTarget = y_combinator([&](auto&& PathsToTarget, const std::string& node)-> u64
		{
			if (auto iter = pathCounts.find(node); iter != pathCounts.cend())
				return iter->second;
			u64 result = 0;
			for (const auto& child : connections[node])
				result += PathsToTarget(child);
			pathCounts[node] = result;
			return result;
		});

	u64 part1 = PathsToTarget("you"), front, mid, last;
	std::string f = "fft", s = "dac";

	ResetPaths();
	u64 dacCount = PathsToTarget(s);
	ResetPaths();
	u64 fftCount = PathsToTarget(f);

	if (dacCount > fftCount)
	{
		std::swap(f, s);
		last = fftCount;
	}
	else last = dacCount;
	
	auto SetTarget = [&pathCounts](const std::string& target)
		{
			pathCounts.clear();
			pathCounts["out"] = 0;
			pathCounts[target] = 1;
		};
	SetTarget(f);
	front = PathsToTarget("svr");

	SetTarget(s);
	mid = PathsToTarget(f);
	std::cout << std::format("Part 1: {}\nPart 2: {}\n", part1, front * mid * last);
	std::cout << std::format("Duration: {}\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - ChronoStart));
}