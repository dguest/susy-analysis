#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "systematic_defs.hh"
#include "RegionConfig.hh"

int main(int narg, char* argv[]) { 
  const char* input_file = "py-output.root"; 
  std::vector<RegionConfig> regions; 
  const char* flags = ""; 

  RegionConfig test; 
  test.name = "test-reg"; 
  test.jet_tag_requirements = {btag::JFC_MEDIUM, btag::JFC_LOOSE, btag::JFC_LOOSE}; 
  test.leading_jet_pt = 1000; 
  test.met = 1000; 
  test.region_bits = reg::electron_jet; 
  test.systematic = syst::METUP; 
  test.output_name = "stant-alone-out.h5"; 
  test.type = reg::CONTROL; 
  test.hists = reg::HISTMILL; 
  test.tagger = btag::JFC; 

  regions.push_back(test); 

  HistBuilder builder(input_file, buildflag::verbose); 
  for (auto itr = regions.begin(); itr != regions.end(); itr++) {
    builder.add_region(*itr); 
  }
  builder.build(); 
  builder.save(); 


}
