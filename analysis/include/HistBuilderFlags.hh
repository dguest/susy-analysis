#ifndef HIST_BUILDER_FLAGS_H
#define HIST_BUILDER_FLAGS_H

namespace buildflag {
  const unsigned verbose    = 1u << 0; // external ('v' flag)
  const unsigned fill_truth = 1u << 1; // external ('t' flag)
  const unsigned is_data    = 1u << 2; // internal (if no mc branches)
  const unsigned short_run  = 1u << 3; // debugging
  const unsigned ttbar_rw   = 1u << 4; // external ('w' flag)
}

#endif
