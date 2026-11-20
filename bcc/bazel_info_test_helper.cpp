#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>

int
main(int argc, char** argv)
{
  // Each block exceeds ordinary pipe capacity. Diagnostics can precede the
  // location (startup) or follow it (shutdown), including on failed commands.
  auto const noise = std::string(2 * 1024 * 1024, 'x');
  std::cerr << "bazel-info-before\n" << noise << '\n';
  std::cout << "/" << argv[argc - 1] << std::endl;
  std::cerr << noise << "\nbazel-info-after\n";
  std::cout << noise << std::endl;
  return std::find(argv + 1, argv + argc, std::string_view("--fail")) != argv + argc ? 7 : 0;
}
