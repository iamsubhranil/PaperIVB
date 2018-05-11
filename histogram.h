#pragma once

#include <stdarg.h>

#include "common.h"

void histo_set_title(const char *title);
void histo_draw(i64 *arr, siz n, i64 colorCount, ...);
