#include "utils.h"


// Assumes 0 <= max <= RAND_MAX
// Returns in the closed interval [0, max]
mint random_at_most(mint max) {
  mint
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = max + 1,
    num_rand = (mint) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  mint x;
  do {
   x = random();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (mint)x);

  // Truncated division is intentional
  return x/bin_size;
}
