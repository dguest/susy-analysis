#include "HistBuilder.hh"
#include "HistBuilderFlags.hh"
#include "systematic_defs.hh"
#include "RegionConfig.hh"
#include <string> 

int run(const char* input_file) { 
  std::vector<RegionConfig> regions; 

  RegionConfig test; 
  test.name = "test-histmill"; 
  test.jet_tag_requirements = {btag::JFC_MEDIUM, btag::JFC_LOOSE, btag::JFC_LOOSE}; 
  test.leading_jet_pt = 1000; 
  test.met = 1000; 
  test.systematic = syst::NONE; 
  test.output_name = "stant-alone-out.h5"; 
  test.type = reg::CONTROL; 
  test.hists = reg::HISTMILL; 
  test.tagger = btag::JFC; 
  regions.push_back(test); 

  int syst_num = 0; 
  for (auto systematic: {syst::ELUP, syst::CUP}) { 
    for (auto hists: {reg::HISTMILL, reg::KINEMATIC_STAT}) { 
      for (auto tagger: {btag::JFC, btag::CNN}) { 
	for (auto assignment: {btag::PT_ORDERED, btag::TAG_ORDERED}) { 
	  RegionConfig syst_reg = test; 
	  syst_reg.name = "syst" + std::to_string(syst_num); 
	  syst_reg.systematic = systematic; 
	  syst_reg.hists = hists; 
	  syst_reg.tagger = tagger; 
	  syst_reg.jet_tag_assignment = assignment; 
	  if (tagger == btag::CNN) { 
	    syst_reg.jet_tag_requirements = {btag::MEDIUM, btag::LOOSE}; 
	  }
	  regions.push_back(syst_reg); 
	  syst_num++; 
	}
      }
    }
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
