#pragma once

#include <stdarg.h>

#include "arr.h"

void histo_set_title(const char *title);
void histo_draw(mint *arr, midx n, mint colorCount, ...);
