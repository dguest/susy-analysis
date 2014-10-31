#ifndef RUN_BITS_H
#define RUN_BITS_H

namespace cutflag {
  const unsigned verbose            = 1u << 0;
  const unsigned is_data            = 1u << 1;
  const unsigned save_all_events    = 1u << 2;
  const unsigned boson_pt_reweight  = 1u << 3;
  const unsigned debug_susy         = 1u << 4;
  const unsigned all_trig_branches  = 1u << 5;
  const unsigned get_branches       = 1u << 6;
  const unsigned cutflow            = 1u << 7;
  // const unsigned vector_output    = 1u << 8;
  const unsigned maximum_compression = 1u << 9;
  // const unsigned no_jet_recal     = 1u << 10;
  const unsigned is_atlfast       = 1u << 11;
  const unsigned debug_cutflow    = 1u << 12;
  const unsigned generate_pileup  = 1u << 13;

  const unsigned disable_sf_err       = 1u << 21;
  const unsigned spartid          = 1u << 22;
  const unsigned truth            = 1u << 23;
  const unsigned truth_ttbar_pt   = 1u << 24;

  // const unsigned can_set_at_runtime = (spartid | truth);

  const unsigned unleash_sharktopus = 1u << 31;
}

#endif //RUN_BITS_H
