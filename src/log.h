/*
 * Optical Character Recognition for Latin-based languages
 * Copyright (C) 2018  meme
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include <cstdarg>
#include <cstdlib>
#include <vector>

#define LOG_VARIANT(name, ll)                                                  \
  static void name(const char* tag, const char* text, ...) {                   \
    va_list args;                                                              \
    va_start(args, text);                                                      \
    vlog(ll, tag, text, args);                                                 \
    va_end(args);                                                              \
  }

enum class LogLevel { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR };

class Log {
public:
  static inline const char* getLogLevelString(LogLevel lvl) {
    if (lvl == LogLevel::LOG_TRACE)
      return "Trace";
    if (lvl == LogLevel::LOG_DEBUG)
      return "Debug";
    if (lvl == LogLevel::LOG_INFO)
      return "Info";
    if (lvl == LogLevel::LOG_WARN)
      return "Warn";
    if (lvl == LogLevel::LOG_ERROR)
      return "Error";

    return "?";
  }

  static void vlog(LogLevel level, const char* tag, const char* text,
                   va_list args);

  static void log(LogLevel level, const char* tag, const char* text, ...) {
    va_list args;
    va_start(args, text);
    vlog(level, tag, text, args);
    va_end(args);
  }

  LOG_VARIANT(trace, LogLevel::LOG_TRACE)
  LOG_VARIANT(debug, LogLevel::LOG_DEBUG)
  LOG_VARIANT(info, LogLevel::LOG_INFO)
  LOG_VARIANT(warn, LogLevel::LOG_WARN)
  LOG_VARIANT(error, LogLevel::LOG_ERROR)
};

#undef LOG_VARIANT
