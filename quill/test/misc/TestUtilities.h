#pragma once

#include "doctest/doctest.h"

#include "DocTestExtensions.h"
#include "quill/detail/misc/Common.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <string>
#include <system_error>
#include <vector>

namespace quill
{
namespace testing
{
// Convert the given file to a vector
std::vector<std::string> file_contents(detail::std_fs::path const& filename);
std::vector<std::wstring> wfile_contents(detail::std_fs::path const& filename);

  // Search a vector for the given string
bool file_contains(std::vector<std::string> const& file_vector, std::string search_string);
} // namespace testing
} // namespace quill
