#include "BosonPtReweighter.hh"
#include "boson_truth_tools.hh"
#include "SusyBuffer.h"

namespace { 

  // pulled from Mainz repo: http://butler.physik.uni-mainz.de/viewcvs/SUSYMainzAna/GlobalAna.cpp?view=markup&revision=1.378&root=ATLAS+Mainz+CVS+Repository#l757
  const float fac=98064.772811754417/95042.398788381863;

  float ReweightWZ(float bosonpt){
    //This function needs the truthBosonPt of the W/Z sample and is only made for Sherpa samples
    if(bosonpt>0. && bosonpt<=50) return fac*2.4562e+00;
    if(bosonpt>50. && bosonpt<=100.) return fac*1.3512e+00;
    if(bosonpt>100. && bosonpt<=150.) return fac*9.4623e-01;
    if(bosonpt>150. && bosonpt<=200.) return fac*1.0176e+00;
    if(bosonpt>200. && bosonpt<=250.) return fac*9.5426e-01;
    if(bosonpt>250. && bosonpt<=300.) return fac*9.9511e-01;
    if(bosonpt>300. && bosonpt<=350.) return fac*7.9143e-01;
    if(bosonpt>350. && bosonpt<=400.) return fac*9.0767e-01;
    if(bosonpt>400.) return fac*7.1171e-01;
    return 1.0;
  }
}

float BosonPtReweighter::get_boson_weight(const SusyBuffer* buff) { 
  float pt_gev = get_boson_truth_pt(buff) / 1e3; 
  return ReweightWZ(pt_gev); 
}

float BosonPtReweighter::get_boson_pt(const SusyBuffer* buffer) { 
  return get_boson_truth_pt(buffer); 
}
