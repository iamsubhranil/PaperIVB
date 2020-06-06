#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "display.h"
#include "histogram.h"
#include "utils.h"

typedef struct {
	siz         index;
	const char *color;
} CustomColor;

static const char *title[] = {"SampleTitle"};

void histo_set_title(const char *tie) {
	title[0] = tie;
}

static Tuple find_max_min(i64 *arr, siz n) {
	i64 min = arr[0];
	i64 max = arr[0];
	for(siz i = 1; i < n; i++) {
		if(arr[i] > max)
			max = arr[i];
		else if(arr[i] < min)
			min = arr[i];
	}
	Tuple t = {max, min};
	return t;
}

#define NUM_STEPS 30

static i64 lastMax = 0;

void histo_draw(i64 *arr, siz n, i64 colorCount, ...) {
	CustomColor colors[colorCount];
	va_list     args;
	va_start(args, colorCount);
	for(i64 i = 0; i < colorCount; i++) {
		colors[i].index = va_arg(args, siz);
		colors[i].color = va_arg(args, const char *);
	}
	va_end(args);
	Tuple  max_min = find_max_min(arr, n);
	double max     = max_min.a;
	while(lastMax++ < 40) {
		printf("\n");
		fflush(stdout);
	}
	double min       = max_min.b - 1;
	double steps     = (double)(max - min) / NUM_STEPS;
	i64    stepCount = 1, lastStepPrint = 1;
	steps = !steps ? 1 : steps;
	if(strcmp(title[0], "SampleTitle"))
		printf(ANSI_FONT_BOLD "Histogram : %s\n\n" ANSI_COLOR_RESET, title[0]);
	for(double i = max; i >= min; i -= steps, stepCount++) {
		if(stepCount == 1 || (stepCount - lastStepPrint > 1 &&
		                      ((i - steps) < min || stepCount % 5 == 0))) {
			printf("%-4.0f", i);
			lastStepPrint = stepCount;
		} else
			printf("    ");
		for(siz j = 0; j < n; j++) {
			if(arr[j] >= i) {
				if(colorCount > 0) {
					for(i64 k = 0; k < colorCount; k++) {
						if(colors[k].index == j) {
							printf("%s", colors[k].color);
							printf("    \u2584" ANSI_COLOR_RESET);
							goto colorend;
						}
					}
				}
				printf("    \u2588");
			} else
				printf("     ");
		colorend:;
		}
		printf("\n");
	}
	printf("\n    ");
	for(siz i = 0; i < n; i++) {
		printf("%5" Pi64, arr[i]);
	}
	// while(stepCount++ < 40)
	printf("\n");
	lastMax = stepCount + 1;
	fflush(stdout);
	struct timespec t;
	// t.tv_nsec = 1000 * 1000 * 1000;
	t.tv_nsec = 1000 * 1000 * 90;
	t.tv_sec  = 0;
	nanosleep(&t, NULL);
}
