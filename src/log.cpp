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

#include "log.h"

#include <cstdio>
#include <cstring>
#include <ctime>

void Log::vlog(LogLevel level, const char* tag, const char* text,
               va_list args) {
// Optionally disable Android output
#ifndef ENABLE_NATIVE_OUTPUT
  if (strcmp(tag, "native") == 0)
    return;
#endif

  char buffer[4096];
  vsnprintf(buffer, sizeof(buffer), text, args);

  char tbuf[128];
  tbuf[0] = '\0';

  time_t t = time(nullptr);
  tm tm;
  localtime_r(&t, &tm);
  strftime(tbuf, sizeof(tbuf), "%H:%M:%S", &tm);
  printf("%s %s [%s] %s\n", tbuf, getLogLevelString(level), tag, buffer);
  fflush(stdout);
}
