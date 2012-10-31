#include "HistBuilder.hh" 
#include "HistBuilderFlags.hh"
#include "JetFactory.hh"
#include "Histogram.hh"
#include "ObjKinematics.hh"
#include "MaskedHistArray.hh"
#include "PhysicalConstants.hh"
#include "H5Cpp.h"

#include <string> 
#include <stdexcept>
#include <boost/format.hpp>
#include <algorithm>
#include <iostream> 
#include <ostream>
#include <fstream>

#include "TVector2.h"

HistBuilder::HistBuilder(std::string input, const unsigned flags): 
  m_flags(flags)
{ 
  m_factory = new JetFactory(input); 
  
  m_jet1_hists = new Jet1DHists(1e3*GeV); 
  m_jet2_hists = new Jet1DHists(1e3*GeV); 
  m_jet3_hists = new Jet1DHists(1e3*GeV); 

  m_met = new MaskedHistArray(Histogram(100, 0.0, 1e3*GeV)); 
  m_min_dphi = new MaskedHistArray(Histogram(100, 0.0, M_PI)); 
  m_j2_met_dphi = new MaskedHistArray(Histogram(100, 0.0, M_PI)); 
  m_mttop = new MaskedHistArray(Histogram(100, 0.0, 1e3*GeV)); 
  m_n_good_jets = new MaskedHistArray(Histogram(11, -0.5, 10.5)); 

}

HistBuilder::~HistBuilder() { 
  delete m_factory; 
  m_factory = 0; 

  delete m_jet1_hists; 
  delete m_jet2_hists; 
  delete m_jet3_hists; 

  delete m_met; 
  delete m_min_dphi; 
  delete m_j2_met_dphi; 
  delete m_mttop; 
  delete m_n_good_jets; 
}

void HistBuilder::add_cut_mask(std::string name, unsigned bits)
{
  CutMasks::const_iterator pos = m_cut_masks.find(name); 
  if (pos != m_cut_masks.end()) { 
    throw std::runtime_error("tried to overwrite " + name); 
  }
  m_cut_masks[name] = bits; 
  m_jet1_hists->add_mask(bits, name); 
  m_jet2_hists->add_mask(bits, name); 
  m_jet3_hists->add_mask(bits, name); 

  m_met->add_mask(bits, name); 
  m_min_dphi->add_mask(bits, name); 
  m_j2_met_dphi->add_mask(bits, name); 
  m_mttop->add_mask(bits, name); 
  m_n_good_jets->add_mask(bits, name); 
}

void HistBuilder::build() { 

  typedef std::vector<Jet> Jets; 
  const int n_entries = m_factory->entries(); 
  const int one_percent = n_entries / 100; 

  std::ofstream nullstream("/dev/null"); 
  std::streambuf* out_buffer = nullstream.rdbuf(); 
  if (m_flags & buildflag::verbose) { 
    out_buffer = std::cout.rdbuf(); 
  }
  std::ostream outstream(out_buffer); 

  for (int entry = 0; entry < n_entries; entry++) { 

    if (entry % one_percent == 0 || entry == n_entries - 1 ) { 
      outstream << boost::format("\r%i of %i (%.1f%%) processed") % 
	entry % n_entries % ( entry / one_percent); 
      outstream.flush(); 
    }

    m_factory->entry(entry); 
    const Jets jets = m_factory->jets(); 
    const unsigned mask = m_factory->bits(); 
    const TVector2 met = m_factory->met(); 
    const TLorentzVector met4(met.Px(), met.Py(), 0, 0); 

    m_met->fill(met.Mod(), mask); 

    if (jets.size() >= 1) { 
      m_jet1_hists->fill(jets.at(0),mask); 
    }
    if (jets.size() >= 2) { 
      const Jet& jet = jets.at(1); 
      m_jet2_hists->fill(jet,mask); 
      double dphi = met4.DeltaPhi(jet); 
      m_j2_met_dphi->fill(fabs(dphi), mask); 
    }
    if (jets.size() >= 3) { 
      m_jet3_hists->fill(jets.at(2), mask); 
      double mttop = get_mttop(std::vector<Jet>(jets.begin(), 
						jets.begin() + 3), met); 
      m_mttop->fill(mttop, mask); 
    }

    double min_jetmet_dphi = 10; 
    for (Jets::const_iterator itr = jets.begin(); itr != jets.end(); itr++) { 
      min_jetmet_dphi = std::min(min_jetmet_dphi, 
				 fabs(met4.DeltaPhi(*itr))); 
    }
    m_min_dphi->fill(min_jetmet_dphi, mask); 
    
    m_n_good_jets->fill(m_factory->n_good(), mask); 

  }
  outstream << std::endl;

}

void HistBuilder::save(std::string output) { 
  if (output.size() == 0) { 
    return; 
  }
  using namespace H5; 
  H5File file(output, H5F_ACC_TRUNC); 

  Group jet1(file.createGroup("/jet1")); 
  m_jet1_hists->write_to(jet1); 
  Group jet2(file.createGroup("/jet2")); 
  m_jet2_hists->write_to(jet2); 
  Group jet3(file.createGroup("/jet3")); 
  m_jet3_hists->write_to(jet3); 

  Group met(file.createGroup("/met")); 
  m_met->write_to(met);
  Group min_dphi(file.createGroup("/minDphi")); 
  m_min_dphi->write_to(min_dphi); 
  Group j2_met_dphi(file.createGroup("/j2MetDphi")); 
  m_j2_met_dphi->write_to(j2_met_dphi); 
  Group mttop(file.createGroup("/mttop")); 
  m_mttop->write_to(mttop); 

  Group nGoodJets(file.createGroup("/nGoodJets")); 
  m_n_good_jets->write_to(nGoodJets); 

}

double HistBuilder::get_mttop(const std::vector<Jet>& jets, TVector2 met) 
{
  assert(jets.size() == 3); 
  std::map<double, std::pair<unsigned, unsigned> > mass_pairs; 
  for (unsigned iii = 0; iii < jets.size(); iii++) { 
    for (unsigned jjj = 0; jjj < iii; jjj++) { 
      double mass = (jets.at(iii) + jets.at(jjj)).Mag(); 
      mass_pairs[mass] = std::make_pair(iii,jjj); 
    }
  }
  std::pair<unsigned,unsigned> lowest = mass_pairs.begin()->second; 
  for (unsigned iii = 0; iii < jets.size(); iii++) { 
    if (iii != lowest.first && iii != lowest.second) { 
      return get_mttop(jets.at(iii), met); 
    }
  }
  assert(false); 
}
double HistBuilder::get_mttop(const Jet& jet, TVector2 met) { 
  TVector2 jet_pt(jet.Px(), jet.Py()); 
  return sqrt( pow( jet_pt.Mod() + sqrt(met.Mod2() + pow(W_MASS,2) ) , 2) - 
	       (jet_pt + met).Mod2() ); 
}
