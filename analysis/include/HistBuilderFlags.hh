#ifndef HIST_BUILDER_FLAGS_H
#define HIST_BUILDER_FLAGS_H

namespace buildflag {
  const unsigned verbose    = 1u << 0;
  const unsigned fill_truth = 1u << 1;
  const unsigned is_data    = 1u << 2; // set internally
  const unsigned short_run  = 1u << 3;
  const unsigned ttbar_rw   = 1u << 4; // set externally
}

#endif
