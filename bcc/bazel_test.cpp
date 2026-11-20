#include <cstdlib>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

#include <gtest/gtest.h>

#include "bcc/bazel.hpp"
#include "tools/cpp/runfiles/runfiles.h"

namespace {

std::string
helper_path()
{
  std::string error;
  std::unique_ptr<bazel::tools::cpp::runfiles::Runfiles> runfiles(
    bazel::tools::cpp::runfiles::Runfiles::CreateForTest(BAZEL_CURRENT_REPOSITORY, &error));
  if (!runfiles) {
    throw std::runtime_error(error);
  }
  auto path = runfiles->Rlocation("bazel-compile-commands/bcc/bazel_info_test_helper");
#ifdef _WIN32
  path += ".exe";
#endif
  return path;
}

TEST(bazel, verbose_info)
{
  auto const helper = helper_path();
  ASSERT_TRUE(std::filesystem::exists(helper)) << helper;
  // The subprocess captures inherited diagnostics without filling another pipe.
  EXPECT_EXIT(
    {
      auto const bazel = bcc::bazel::create(helper, {}, {});
      std::exit(bazel.workspace_path() == "/workspace" && bazel.execution_root() == "/execution_root" ? 0 : 2);
    },
    testing::ExitedWithCode(0),
    "bazel-info-after");
}

TEST(bazel, verbose_info_failure)
{
  auto const helper = helper_path();
  ASSERT_TRUE(std::filesystem::exists(helper)) << helper;
  EXPECT_EXIT(
    {
      try {
        bcc::bazel::create(helper, { "--fail" }, {});
      } catch (bcc::bazel_error const& error) {
        std::exit(std::string(error.what()).find("exit code 7") != std::string::npos ? 7 : 2);
      }
      std::exit(3);
    },
    testing::ExitedWithCode(7),
    "bazel-info-after");
}

} // namespace
