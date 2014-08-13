#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "systematic_defs.hh"
#include "RegionConfig.hh"
#include <string>

int run(const char* input_file) {
  std::vector<RegionConfig> regions;

  RegionConfig test;
  test.name = "test-histmill";
  test.leading_jet_pt = 1000;
  test.met = 1000;
  test.systematic = syst::NONE;
  test.output_name = "stand-alone-out.h5";
  test.type = reg::CONTROL;
  test.hists = reg::NMINUS;
  test.selection = reg::Selection::SIGNAL;
  test.stream = reg::Stream::SIMULATED;
  test.tagger = btag::Tagger::JFC;
  test.max_signal_jets = 999;
  regions.push_back(test);

  int syst_num = 0;
  for (auto systematic: {syst::ELUP, syst::CUP}) {
    // for (auto hists: {reg::NMINUS}) {
    RegionConfig syst_reg = test;
    syst_reg.name = "syst" + std::to_string(syst_num);
    syst_reg.systematic = systematic;
    // syst_reg.hists = hists;
    regions.push_back(syst_reg);
    syst_num++;
    // }
  }

  HistBuilder builder(input_file, buildflag::verbose | buildflag::short_run);
  for (auto itr = regions.begin(); itr != regions.end(); itr++) {
    builder.add_region(*itr);
  }
  builder.build();
  builder.save();
  return 0;
}

int main(int narg, char* argv[]) {
  if (narg < 2) abort();
  for (int iii = 0; iii < 3; iii++) {
    run(argv[1]);
  }
  return 0;
}
