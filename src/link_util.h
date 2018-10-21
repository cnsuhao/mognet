#pragma once

// Linking utilities

void hybris_hook(const char* name, void* func);
void* hybris_hook_callback(const char* name, const char* requester);
void* load_os_library(const char* path, const char** symbols);
