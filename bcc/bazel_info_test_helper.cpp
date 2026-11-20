#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <string_view>

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

int
main(int argc, char** argv)
{
#ifdef _WIN32
  if (_setmode(_fileno(stdout), _O_BINARY) == -1) {
    return 2;
  }
#endif
  // Each block exceeds ordinary pipe capacity. Diagnostics can precede the
  // location (startup) or follow it (shutdown), including on failed commands.
  auto const is_aquery = std::find(argv + 1, argv + argc, std::string_view("aquery")) != argv + argc;
  auto const noise = std::string(2 * 1024 * 1024, 'x');
  std::cerr << "bazel-info-before\n" << noise << '\n';
  if (!is_aquery) {
    std::cout << "/" << argv[argc - 1] << std::endl;
  }
  std::cerr << noise << (is_aquery ? "\nbazel-aquery-after\n" : "\nbazel-info-after\n");
  if (!is_aquery) {
    std::cout << noise << std::endl;
  }
  // An empty stream encodes an empty ActionGraphContainer for aquery.
  return std::find(argv + 1, argv + argc, std::string_view("--fail")) != argv + argc ? 7 : 0;
}
