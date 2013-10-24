#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "systematic_defs.hh"
#include "RegionConfig.hh"
#include <string> 

int main(int narg, char* argv[]) { 
  const char* input_file = "py-output.root"; 
  std::vector<RegionConfig> regions; 
  const char* flags = ""; 

  RegionConfig test; 
  test.name = "test-histmill"; 
  test.jet_tag_requirements = {btag::JFC_MEDIUM, btag::JFC_LOOSE, btag::JFC_LOOSE}; 
  test.leading_jet_pt = 1000; 
  test.met = 1000; 
  test.region_bits = reg::electron_jet; 
  test.systematic = syst::NONE; 
  test.output_name = "stant-alone-out.h5"; 
  test.type = reg::CONTROL; 
  test.hists = reg::HISTMILL; 
  test.tagger = btag::JFC; 
  regions.push_back(test); 

  RegionConfig test2 = test; 
  test2.name = "test-kinstat"; 
  test2.hists = reg::KINEMATIC_STAT; 
  regions.push_back(test2); 

  int syst_num = 0; 
  for (auto systematic: {syst::METUP, syst::ELUP, syst::CUP}) { 
    RegionConfig syst_reg = test; 
    syst_reg.name = "syst" + std::to_string(syst_num); 
    syst_reg.systematic = systematic; 
    regions.push_back(syst_reg); 
    syst_num++; 
  }

  HistBuilder builder(input_file, buildflag::verbose); 
  for (auto itr = regions.begin(); itr != regions.end(); itr++) {
    builder.add_region(*itr); 
  }
  builder.build(); 
  builder.save(); 


}
