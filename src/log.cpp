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
