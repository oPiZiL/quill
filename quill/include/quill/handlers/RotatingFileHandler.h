/**
 * Copyright(c) 2020-present, Odysseas Georgoudis & quill contributors.
 * Distributed under the MIT License (http://opensource.org/licenses/MIT)
 */

#pragma once

#include "quill/Fmt.h"                    // for memory_buffer
#include "quill/detail/misc/Attributes.h" // for QUILL_ATTRIBUTE_COLD, QUIL...
#include "quill/handlers/FileHandler.h"   // for FileHandler
#include <chrono>                         // for nanoseconds
#include <cstddef>                        // for size_t
#include <cstdint>                        // for uint32_t

namespace quill
{
/**
 * Rotating file handler based on file size
 */
class RotatingFileHandler final : public FileHandler
{
public:
  /**
   * constructor
   * @param base_filename Base file name to be used for logs
   * @param mode the mode to open_file the file
   * @param max_bytes max size per file in bytes
   * @param backup_count maximum log files
   * @param replace_oldest_files if set to true, oldest logs will get overwritten when backup_count
   * is reached. if set to false then when backup_count is reached the rotation will stop and
   * the base log file will grow indefinitely
   * @param clean_old_files Setting this to true will also clean any previous rotated log files when
   * mode="w" is used
   * @throws on invalid rotation values
   */
  RotatingFileHandler(detail::std_fs::path const& base_filename, std::string const& mode, size_t max_bytes,
                      uint32_t backup_count, bool overwrite_oldest_files, bool clean_old_files = false);

  /**
   * Destructor
   */
  ~RotatingFileHandler() override = default;

  /**
   * Write a formatted log message to the stream
   * @param formatted_log_message input log message to write
   * @param log_message_timestamp log message timestamp
   */
  QUILL_ATTRIBUTE_HOT void write(fmt::memory_buffer const& formatted_log_message,
                                 std::chrono::nanoseconds log_message_timestamp,
                                 LogLevel log_message_severity) override;

private:
  /**
   * Rotate to a new file
   */
  QUILL_ATTRIBUTE_COLD void _rotate();

private:
  size_t _current_size{0};
  size_t _max_bytes{0};
  uint32_t _backup_count{0};
  uint32_t _current_index{0};
  bool _overwrite_oldest_files{true};
};

} // namespace quill