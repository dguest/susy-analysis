//////////////////////// -*-c++-*-//////////////////////////
// This class has been automatically generated on
// Wed Jun 20 17:06:57 2012 by ROOT version 5.30/00
// from TTree susy/susy
// found on file: file1.root
//////////////////////////////////////////////////////////

#ifndef susy_h
#define susy_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector> 

using namespace std; 

class susy {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Bool_t          EF_2MUL1_l2j30_HV;
   Bool_t          EF_2MUL1_l2j30_HV_AllMS;
   Bool_t          EF_2MUL1_l2j30_HV_cosmic;
   Bool_t          EF_2MUL1_l2j30_HV_firstempty;
   Bool_t          EF_2MUL1_l2j30_HV_unpaired_iso;
   Bool_t          EF_2MUL1_l2j30_HV_unpaired_noniso;
   Bool_t          EF_2b10_medium_3L1J10;
   Bool_t          EF_2b10_medium_4L1J10;
   Bool_t          EF_2b10_medium_4j30_a4tc_EFFS;
   Bool_t          EF_2b10_medium_L1JE100;
   Bool_t          EF_2b10_medium_L1JE140;
   Bool_t          EF_2b10_medium_L1_2J10J50;
   Bool_t          EF_2b10_medium_j100_j30_a4tc_EFFS;
   Bool_t          EF_2b10_medium_j75_j30_a4tc_EFFS;
   Bool_t          EF_2b10_tight_4j30_a4tc_EFFS;
   Bool_t          EF_2b15_medium_3L1J15;
   Bool_t          EF_2b15_medium_3j40_a4tc_EFFS;
   Bool_t          EF_2b15_medium_j75_j40_a4tc_EFFS;
   Bool_t          EF_2b20_medium_3L1J20;
   Bool_t          EF_2b20_medium_3j45_a4tc_EFFS;
   Bool_t          EF_2b20_medium_j75_j45_a4tc_EFFS;
   Bool_t          EF_2e10_medium;
   Bool_t          EF_2e10_medium_mu6;
   Bool_t          EF_2e11T_medium;
   Bool_t          EF_2e12T_medium;
   Bool_t          EF_2e12_medium;
   Bool_t          EF_2e15_loose;
   Bool_t          EF_2e15_medium;
   Bool_t          EF_2e5_medium;
   Bool_t          EF_2e5_medium_SiTrk;
   Bool_t          EF_2e5_medium_TRT;
   Bool_t          EF_2e5_tight;
   Bool_t          EF_2e5_tight_Jpsi;
   Bool_t          EF_2fj100_a4tc_EFFS_deta50_FB;
   Bool_t          EF_2fj30_a4tc_EFFS_deta50_FB;
   Bool_t          EF_2fj30_a4tc_EFFS_deta50_FC;
   Bool_t          EF_2fj55_a4tc_EFFS_deta50_FB;
   Bool_t          EF_2fj55_a4tc_EFFS_deta50_FC;
   Bool_t          EF_2fj75_a4tc_EFFS_deta50_FB;
   Bool_t          EF_2fj75_a4tc_EFFS_deta50_FC;
   Bool_t          EF_2g10_loose;
   Bool_t          EF_2g15_loose;
   Bool_t          EF_2g20_loose;
   Bool_t          EF_2g50_etcut;
   Bool_t          EF_2j100_a4tc_EFFS_deta35_FC;
   Bool_t          EF_2j135_a4tc_EFFS_deta35_FC;
   Bool_t          EF_2j180_a4tc_EFFS_deta35_FC;
   Bool_t          EF_2j240_a4tc_EFFS_deta35_FC;
   Bool_t          EF_2j45_a4tc_EFFS_leadingmct100_xe40_medium_noMu;
   Bool_t          EF_2j55_a4tc_EFFS_leadingmct100_xe40_medium_noMu;
   Bool_t          EF_2mu10;
   Bool_t          EF_2mu10_NoAlg;
   Bool_t          EF_2mu10_empty;
   Bool_t          EF_2mu10_loose;
   Bool_t          EF_2mu10_loose1;
   Bool_t          EF_2mu10_loose_empty;
   Bool_t          EF_2mu10_loose_noOvlpRm;
   Bool_t          EF_2mu10_noOvlpRm;
   Bool_t          EF_2mu13_Zmumu_IDTrkNoCut;
   Bool_t          EF_2mu4;
   Bool_t          EF_2mu4_Bmumu;
   Bool_t          EF_2mu4_Bmumux;
   Bool_t          EF_2mu4_DiMu;
   Bool_t          EF_2mu4_DiMu_DY;
   Bool_t          EF_2mu4_DiMu_DY20;
   Bool_t          EF_2mu4_DiMu_DY_noVtx_noOS;
   Bool_t          EF_2mu4_DiMu_SiTrk;
   Bool_t          EF_2mu4_DiMu_noVtx;
   Bool_t          EF_2mu4_DiMu_noVtx_noOS;
   Bool_t          EF_2mu4_EFxe20_noMu;
   Bool_t          EF_2mu4_Jpsimumu;
   Bool_t          EF_2mu4_Jpsimumu_IDTrkNoCut;
   Bool_t          EF_2mu4_Upsimumu;
   Bool_t          EF_2mu4_xe30_noMu;
   Bool_t          EF_2mu4_xe40_noMu;
   Bool_t          EF_2mu4i_DiMu_DY;
   Bool_t          EF_2mu4i_DiMu_DY_noVtx_noOS;
   Bool_t          EF_2mu6;
   Bool_t          EF_2mu6_Bmumu;
   Bool_t          EF_2mu6_Bmumux;
   Bool_t          EF_2mu6_DiMu;
   Bool_t          EF_2mu6_Jpsimumu;
   Bool_t          EF_2mu6_MSonly_g10_loose;
   Bool_t          EF_2mu6_MSonly_g10_loose_noOvlpRm;
   Bool_t          EF_2mu6_MSonly_g10_loose_nonfilled;
   Bool_t          EF_2mu6_NL;
   Bool_t          EF_2mu6_Upsimumu;
   Bool_t          EF_2mu6_e10_loose;
   Bool_t          EF_2mu6_g10_loose;
   Bool_t          EF_2tau16_loose;
   Bool_t          EF_2tau29T_medium1;
   Bool_t          EF_2tau29_loose;
   Bool_t          EF_2tau29_loose1;
   Bool_t          EF_2tau29_loose1_EFxe15_noMu;
   Bool_t          EF_2tau29_loose1_EFxe40_noMu;
   Bool_t          EF_2tau29_medium1;
   Bool_t          EF_2tau29i_medium1;
   Bool_t          EF_2tau38_loose;
   Bool_t          EF_3b10_loose_4L1J10;
   Bool_t          EF_3b10_medium_4j30_a4tc_EFFS;
   Bool_t          EF_3b15_loose_4L1J15;
   Bool_t          EF_3b15_medium_4j40_a4tc_EFFS;
   Bool_t          EF_3e10_medium;
   Bool_t          EF_3j100_a4tc_EFFS;
   Bool_t          EF_3j100_a4tc_EFFS_L1J75;
   Bool_t          EF_3j30_a4tc_EFFS;
   Bool_t          EF_3j40_a4tc_EFFS;
   Bool_t          EF_3j45_a4tc_EFFS;
   Bool_t          EF_3j75_a4tc_EFFS;
   Bool_t          EF_3mu4;
   Bool_t          EF_3mu4_DiMu;
   Bool_t          EF_3mu4_Jpsi;
   Bool_t          EF_3mu4_Tau;
   Bool_t          EF_3mu4_Upsi;
   Bool_t          EF_3mu6;
   Bool_t          EF_3mu6_MSonly;
   Bool_t          EF_3mu6_MSonly_noOvlpRm;
   Bool_t          EF_3mu6_MSonly_nonfilled;
   Bool_t          EF_4j30_a4tc_EFFS;
   Bool_t          EF_4j40_a4tc_EFFS;
   Bool_t          EF_4j40_a4tc_EFFS_ht350;
   Bool_t          EF_4j40_a4tc_EFFS_ht400;
   Bool_t          EF_4j45_a4tc_EFFS;
   Bool_t          EF_4j55_a4tc_EFFS;
   Bool_t          EF_4j60_a4tc_EFFS;
   Bool_t          EF_4mu4;
   Bool_t          EF_5j30_a4tc_EFFS;
   Bool_t          EF_5j40_a4tc_EFFS;
   Bool_t          EF_5j45_a4tc_EFFS;
   Bool_t          EF_6j30_a4tc_EFFS;
   Bool_t          EF_6j30_a4tc_EFFS_L15J10;
   Bool_t          EF_6j40_a4tc_EFFS;
   Bool_t          EF_6j45_a4tc_EFFS;
   Bool_t          EF_7j30_a4tc_EFFS_L15J10;
   Bool_t          EF_7j30_a4tc_EFFS_L16J10;
   Bool_t          EF_ALFA_Phys;
   Bool_t          EF_Background;
   Bool_t          EF_CosmicsAllTeEFID;
   Bool_t          EF_CosmicsAllTeIDSCAN_AllPhysics_4Hits_TrkHypo;
   Bool_t          EF_CosmicsAllTeIDSCAN_AllPhysics_TrkHypo;
   Bool_t          EF_CosmicsAllTeSiTrack_AllPhysics_4Hits_TrkHypo;
   Bool_t          EF_CosmicsAllTeSiTrack_AllPhysics_TrkHypo;
   Bool_t          EF_CosmicsAllTeTRTxK_TRTTrkHypo_AllPhysics;
   Bool_t          EF_CosmicsAllTeTRTxK_TRTTrkHypo_Pixel;
   Bool_t          EF_Cosmics_MU11_TRTxK_TRTTrkHypo_AllID;
   Bool_t          EF_Cosmics_MU11_TRTxK_TRTTrkHypo_Pixel;
   Bool_t          EF_HLTTimeBurner;
   Bool_t          EF_InDetMon_FS;
   Bool_t          EF_InDetMon_L2Star_FS;
   Bool_t          EF_L1Calo_NoAlg;
   Bool_t          EF_L1FJ10_empty_NoAlg;
   Bool_t          EF_L1FJ75_NoAlg;
   Bool_t          EF_L1J10_empty_NoAlg;
   Bool_t          EF_L1J10_firstempty_NoAlg;
   Bool_t          EF_L1J10_unpaired_iso_NoAlg;
   Bool_t          EF_L1J10_unpaired_noniso_NoAlg;
   Bool_t          EF_L1J175_NoAlg;
   Bool_t          EF_L1J250_NoAlg;
   Bool_t          EF_L1J30_empty_NoAlg;
   Bool_t          EF_L1J30_firstempty_NoAlg;
   Bool_t          EF_L1J30_unpaired_iso_NoAlg;
   Bool_t          EF_L1J30_unpaired_noniso_NoAlg;
   Bool_t          EF_L1MU10_firstempty_NoAlg;
   Bool_t          EF_L1MU11_NoAlg;
   Bool_t          EF_L1MU15_NoAlg;
   Bool_t          EF_L1MU20_NoAlg;
   Bool_t          EF_L1MinBias_NoAlg;
   Bool_t          EF_L1Muon_NoAlg;
   Bool_t          EF_L1TAU30_NoAlg;
   Bool_t          EF_L1TRT_NoAlg;
   Bool_t          EF_MU0_j45_a4tc_EFFS_xe45_loose_noMu;
   Bool_t          EF_PreS_L1EM3_empty;
   Bool_t          EF_PreS_L1EM5_empty;
   Bool_t          EF_PreS_L1J10_empty;
   Bool_t          EF_PreS_L1J10_firstempty;
   Bool_t          EF_PreS_L1J30_empty;
   Bool_t          EF_PreS_L1J30_firstempty;
   Bool_t          EF_PreS_L1TAU8_empty;
   Bool_t          EF_ROBRequestMon;
   Bool_t          EF_SeededStreamerL1CaloXS;
   Bool_t          EF_Standby;
   Bool_t          EF_TileCalib_laser;
   Bool_t          EF_allL1_NoAlg;
   Bool_t          EF_b10_EFj10_a4tc_EFFS_IDTrkNoCut;
   Bool_t          EF_b10_IDTrkNoCut;
   Bool_t          EF_b10_L2Star_IDTrkNoCut;
   Bool_t          EF_b10_medium_4L1J10;
   Bool_t          EF_b10_medium_4j30_a4tc_EFFS;
   Bool_t          EF_b10_medium_EFxe25_noMu_L1JE100;
   Bool_t          EF_b10_medium_EFxe25_noMu_L1JE140;
   Bool_t          EF_b10_medium_EFxe25_noMu_L1_2J10J50;
   Bool_t          EF_b10_medium_j75_j55_2j30_a4tc_EFFS;
   Bool_t          EF_b10_tight_4L1J10;
   Bool_t          EF_b10_tight_4j30_a4tc_EFFS;
   Bool_t          EF_b10_tight_L1JE100;
   Bool_t          EF_b10_tight_L1JE140;
   Bool_t          EF_b10_tight_j75_j55_2j30_a4tc_EFFS;
   Bool_t          EF_b15_IDTrkNoCut;
   Bool_t          EF_b20_IDTrkNoCut;
   Bool_t          EF_cost_monitor;
   Bool_t          EF_e10_NoCut;
   Bool_t          EF_e10_loose;
   Bool_t          EF_e10_loose_mu10;
   Bool_t          EF_e10_loose_mu6;
   Bool_t          EF_e10_medium;
   Bool_t          EF_e10_medium_2mu6;
   Bool_t          EF_e10_medium_mu10;
   Bool_t          EF_e10_medium_mu6;
   Bool_t          EF_e10_medium_mu6_topo_medium;
   Bool_t          EF_e11T_medium;
   Bool_t          EF_e11T_medium_2e6T_medium;
   Bool_t          EF_e11_etcut;
   Bool_t          EF_e12T_medium_mu6_topo_medium;
   Bool_t          EF_e12_medium;
   Bool_t          EF_e13_etcutTRT_xs60_noMu;
   Bool_t          EF_e13_etcut_xs60_noMu;
   Bool_t          EF_e13_etcut_xs70_noMu;
   Bool_t          EF_e15_HLTtighter;
   Bool_t          EF_e15_loose;
   Bool_t          EF_e15_medium;
   Bool_t          EF_e15_medium_e12_medium;
   Bool_t          EF_e15_medium_mu6;
   Bool_t          EF_e15_medium_xe30_noMu;
   Bool_t          EF_e15_medium_xe35_loose_noMu;
   Bool_t          EF_e15_medium_xe35_noMu;
   Bool_t          EF_e15_medium_xe40_loose_noMu;
   Bool_t          EF_e15_medium_xe40_noMu;
   Bool_t          EF_e15_medium_xe50_noMu;
   Bool_t          EF_e15_tight;
   Bool_t          EF_e15_tight1;
   Bool_t          EF_e15_tight_xe30_noMu;
   Bool_t          EF_e18_medium;
   Bool_t          EF_e18_medium_xe30_noMu;
   Bool_t          EF_e18_tight;
   Bool_t          EF_e20_L2Star_IDTrkNoCut;
   Bool_t          EF_e20_etcut_xs60_noMu;
   Bool_t          EF_e20_loose;
   Bool_t          EF_e20_loose1;
   Bool_t          EF_e20_looseTrk;
   Bool_t          EF_e20_loose_2j10_a4tc_EFFS;
   Bool_t          EF_e20_loose_3j10_a4tc_EFFS;
   Bool_t          EF_e20_loose_4j10_a4tc_EFFS;
   Bool_t          EF_e20_loose_IDTrkNoCut;
   Bool_t          EF_e20_loose_TRT;
   Bool_t          EF_e20_loose_xe20;
   Bool_t          EF_e20_loose_xe20_noMu;
   Bool_t          EF_e20_loose_xe30;
   Bool_t          EF_e20_loose_xe30_noMu;
   Bool_t          EF_e20_medium;
   Bool_t          EF_e20_medium1;
   Bool_t          EF_e20_medium2;
   Bool_t          EF_e20_medium_IDTrkNoCut;
   Bool_t          EF_e20_medium_SiTrk;
   Bool_t          EF_e20_medium_TRT;
   Bool_t          EF_e20_tight;
   Bool_t          EF_e20_tight_e15_NoCut_Zee;
   Bool_t          EF_e22_etcut_xs60_noMu;
   Bool_t          EF_e22_loose;
   Bool_t          EF_e22_loose1;
   Bool_t          EF_e22_looseTrk;
   Bool_t          EF_e22_medium;
   Bool_t          EF_e22_medium1;
   Bool_t          EF_e22_medium2;
   Bool_t          EF_e22_medium_IDTrkNoCut;
   Bool_t          EF_e22_medium_SiTrk;
   Bool_t          EF_e22_medium_TRT;
   Bool_t          EF_e22_tight;
   Bool_t          EF_e25_loose;
   Bool_t          EF_e25_medium;
   Bool_t          EF_e30_loose;
   Bool_t          EF_e30_medium;
   Bool_t          EF_e33_medium;
   Bool_t          EF_e35_medium;
   Bool_t          EF_e40_medium;
   Bool_t          EF_e45_medium;
   Bool_t          EF_e45_medium1;
   Bool_t          EF_e5_NoCut_L2SW;
   Bool_t          EF_e5_NoCut_Ringer;
   Bool_t          EF_e5_NoCut_firstempty;
   Bool_t          EF_e5_medium_mu4;
   Bool_t          EF_e5_medium_mu6;
   Bool_t          EF_e5_medium_mu6_topo_loose;
   Bool_t          EF_e5_medium_mu6_topo_medium;
   Bool_t          EF_e5_tight;
   Bool_t          EF_e5_tight_TRT;
   Bool_t          EF_e5_tight_e14_etcut_Jpsi;
   Bool_t          EF_e5_tight_e4_etcut_Jpsi;
   Bool_t          EF_e5_tight_e4_etcut_Jpsi_SiTrk;
   Bool_t          EF_e5_tight_e4_etcut_Jpsi_TRT;
   Bool_t          EF_e5_tight_e5_NoCut;
   Bool_t          EF_e5_tight_e5_NoCut_Jpsi;
   Bool_t          EF_e5_tight_e7_etcut_Jpsi;
   Bool_t          EF_e5_tight_e9_etcut_Jpsi;
   Bool_t          EF_e60_loose;
   Bool_t          EF_e6T_medium;
   Bool_t          EF_e7_tight_e14_etcut_Jpsi;
   Bool_t          EF_e9_tight_e5_tight_Jpsi;
   Bool_t          EF_eb_physics;
   Bool_t          EF_eb_physics_empty;
   Bool_t          EF_eb_physics_firstempty;
   Bool_t          EF_eb_physics_noL1PS;
   Bool_t          EF_eb_physics_unpaired_iso;
   Bool_t          EF_eb_physics_unpaired_noniso;
   Bool_t          EF_eb_random;
   Bool_t          EF_eb_random_empty;
   Bool_t          EF_eb_random_firstempty;
   Bool_t          EF_eb_random_unpaired_iso;
   Bool_t          EF_em105_passHLT;
   Bool_t          EF_fj100_a4tc_EFFS;
   Bool_t          EF_fj10_a4tc_EFFS;
   Bool_t          EF_fj10_a4tc_EFFS_1vx;
   Bool_t          EF_fj135_a4tc_EFFS;
   Bool_t          EF_fj15_a4tc_EFFS;
   Bool_t          EF_fj20_a4tc_EFFS;
   Bool_t          EF_fj30_a4tc_EFFS;
   Bool_t          EF_fj30_a4tc_EFFS_l2cleanph;
   Bool_t          EF_fj55_a4tc_EFFS;
   Bool_t          EF_fj75_a4tc_EFFS;
   Bool_t          EF_g100_etcut_g50_etcut;
   Bool_t          EF_g100_loose;
   Bool_t          EF_g10_NoCut_cosmic;
   Bool_t          EF_g11_etcut;
   Bool_t          EF_g11_etcut_larcalib;
   Bool_t          EF_g13_etcut_xs60_noMu;
   Bool_t          EF_g150_etcut;
   Bool_t          EF_g15_loose;
   Bool_t          EF_g15_loose_larcalib;
   Bool_t          EF_g17_etcut;
   Bool_t          EF_g17_etcut_EFxe20_noMu;
   Bool_t          EF_g17_etcut_EFxe30_noMu;
   Bool_t          EF_g20_etcut;
   Bool_t          EF_g20_etcut_xe30_noMu;
   Bool_t          EF_g20_loose;
   Bool_t          EF_g20_loose_cnv;
   Bool_t          EF_g20_loose_larcalib;
   Bool_t          EF_g20_loose_xe20_noMu;
   Bool_t          EF_g20_loose_xe30_noMu;
   Bool_t          EF_g20_tight;
   Bool_t          EF_g22_hiptrt;
   Bool_t          EF_g25_loose_xe30_noMu;
   Bool_t          EF_g27_etcut;
   Bool_t          EF_g27_etcut_EFxe30_noMu;
   Bool_t          EF_g30_loose_g20_loose;
   Bool_t          EF_g40_loose;
   Bool_t          EF_g40_loose_EFxe35_noMu;
   Bool_t          EF_g40_loose_EFxe40_noMu;
   Bool_t          EF_g40_loose_b10_medium;
   Bool_t          EF_g40_loose_larcalib;
   Bool_t          EF_g40_loose_xe40_noMu;
   Bool_t          EF_g40_loose_xe45_medium_noMu;
   Bool_t          EF_g40_loose_xe55_medium_noMu;
   Bool_t          EF_g40_loose_xs35_noMu;
   Bool_t          EF_g40_tight;
   Bool_t          EF_g40_tight_b10_medium;
   Bool_t          EF_g40_tight_b15_medium;
   Bool_t          EF_g50_loose;
   Bool_t          EF_g5_NoCut_cosmic;
   Bool_t          EF_g60_loose;
   Bool_t          EF_g60_loose_larcalib;
   Bool_t          EF_g80_loose;
   Bool_t          EF_g80_loose_larcalib;
   Bool_t          EF_hadCalib_trk9;
   Bool_t          EF_hadCalib_trk9phi;
   Bool_t          EF_high_eb_physics;
   Bool_t          EF_ht350_a4tc_EFFS_L2je255;
   Bool_t          EF_j100_a4tc_EFFS;
   Bool_t          EF_j100_a4tc_EFFS_ht350;
   Bool_t          EF_j100_a4tc_EFFS_ht400;
   Bool_t          EF_j100_a4tc_EFFS_ht500;
   Bool_t          EF_j100_j30_a4tc_EFFS_L2dphi04;
   Bool_t          EF_j10_a4tc_EFFS;
   Bool_t          EF_j10_a4tc_EFFS_1vx;
   Bool_t          EF_j135_a4tc_EFFS;
   Bool_t          EF_j135_a4tc_EFFS_ht500;
   Bool_t          EF_j135_j30_a4tc_EFFS_L2dphi04;
   Bool_t          EF_j135_j30_a4tc_EFFS_dphi04;
   Bool_t          EF_j15_a4tc_EFFS;
   Bool_t          EF_j180_a4tc_EFFS;
   Bool_t          EF_j180_j30_a4tc_EFFS_dphi04;
   Bool_t          EF_j20_a4tc_EFFS;
   Bool_t          EF_j240_a10tc_EFFS;
   Bool_t          EF_j240_a4tc_EFFS;
   Bool_t          EF_j240_a4tc_EFFS_l2cleanph;
   Bool_t          EF_j30_a4tc_EFFS;
   Bool_t          EF_j30_a4tc_EFFS_l2cleanph;
   Bool_t          EF_j30_cosmic;
   Bool_t          EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_empty;
   Bool_t          EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_firstempty;
   Bool_t          EF_j30_firstempty;
   Bool_t          EF_j30_fj30_a4tc_EFFS;
   Bool_t          EF_j320_a10tc_EFFS;
   Bool_t          EF_j320_a4tc_EFFS;
   Bool_t          EF_j35_a4tc_EFFS;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HV;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HV_cosmic;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HV_firstempty;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_iso;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_noniso;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HVtrk;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HVtrk_LOF;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HVtrk_cosmic;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HVtrk_firstempty;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_iso;
   Bool_t          EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_noniso;
   Bool_t          EF_j40_a4tc_EFFS;
   Bool_t          EF_j40_fj40_a4tc_EFFS;
   Bool_t          EF_j425_a10tc_EFFS;
   Bool_t          EF_j425_a4tc_EFFS;
   Bool_t          EF_j45_a4tc_EFFS;
   Bool_t          EF_j50_a4tc_EFFS;
   Bool_t          EF_j50_cosmic;
   Bool_t          EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_empty;
   Bool_t          EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_firstempty;
   Bool_t          EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_empty;
   Bool_t          EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_firstempty;
   Bool_t          EF_j50_firstempty;
   Bool_t          EF_j55_a4tc_EFFS;
   Bool_t          EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10;
   Bool_t          EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10_l2cleancons;
   Bool_t          EF_j55_fj55_a4tc_EFFS;
   Bool_t          EF_j65_a4tc_EFFS_xe65_noMu_dphi2j30xe10;
   Bool_t          EF_j70_j25_dphi03_NoEF;
   Bool_t          EF_j75_2j30_a4tc_EFFS_ht350;
   Bool_t          EF_j75_a4tc_EFFS;
   Bool_t          EF_j75_a4tc_EFFS_xe40_loose_noMu;
   Bool_t          EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03;
   Bool_t          EF_j75_a4tc_EFFS_xe45_loose_noMu;
   Bool_t          EF_j75_a4tc_EFFS_xe55_loose_noMu;
   Bool_t          EF_j75_a4tc_EFFS_xe55_noMu;
   Bool_t          EF_j75_a4tc_EFFS_xe55_noMu_l2cleancons;
   Bool_t          EF_j75_a4tc_EFFS_xs35_noMu;
   Bool_t          EF_j75_fj75_a4tc_EFFS;
   Bool_t          EF_j75_j30_a4tc_EFFS;
   Bool_t          EF_j75_j30_a4tc_EFFS_L2anymct100;
   Bool_t          EF_j75_j30_a4tc_EFFS_L2anymct150;
   Bool_t          EF_j75_j30_a4tc_EFFS_L2anymct175;
   Bool_t          EF_j75_j30_a4tc_EFFS_L2dphi04;
   Bool_t          EF_j75_j30_a4tc_EFFS_anymct150;
   Bool_t          EF_j75_j30_a4tc_EFFS_anymct175;
   Bool_t          EF_j75_j30_a4tc_EFFS_leadingmct150;
   Bool_t          EF_j80_a4tc_EFFS_xe60_noMu;
   Bool_t          EF_je195_NoEF;
   Bool_t          EF_je255_NoEF;
   Bool_t          EF_je300_NoEF;
   Bool_t          EF_je350_NoEF;
   Bool_t          EF_je420_NoEF;
   Bool_t          EF_je500_NoEF;
   Bool_t          EF_l2j30_Trackless_HV;
   Bool_t          EF_l2j30_Trackless_HV_L1MU10;
   Bool_t          EF_l2j30_Trackless_HV_cosmic;
   Bool_t          EF_l2j30_Trackless_HV_firstempty;
   Bool_t          EF_l2j30_Trackless_HV_unpaired_iso;
   Bool_t          EF_l2j30_Trackless_HV_unpaired_noniso;
   Bool_t          EF_mbLucid_eff;
   Bool_t          EF_mbMbts_2_eff_unpaired_iso;
   Bool_t          EF_mbRd0_eff_empty;
   Bool_t          EF_mbRd0_eff_unpaired_iso;
   Bool_t          EF_mbRd1_eff;
   Bool_t          EF_mbRmMbts_1_NoCut_Time;
   Bool_t          EF_mbSpBg_unpaired_iso;
   Bool_t          EF_mbSpTrk;
   Bool_t          EF_mbSpTrkVtxMh;
   Bool_t          EF_mbSpTrkVtxMh_eff;
   Bool_t          EF_mbSpTrkVtxMh_medium;
   Bool_t          EF_mbSpTrkVtxMh_medium_eff;
   Bool_t          EF_mbSpTrkVtxMh_tight;
   Bool_t          EF_mbSpTrkVtxMh_tight_eff;
   Bool_t          EF_mbSpTrk_unpaired_iso;
   Bool_t          EF_mbTE20_eff;
   Bool_t          EF_mbZdc_eff;
   Bool_t          EF_mb_BCM_Wide_eff;
   Bool_t          EF_mu0_empty_NoAlg;
   Bool_t          EF_mu0_firstempty_NoAlg;
   Bool_t          EF_mu0_missingRoi;
   Bool_t          EF_mu0_unpaired_iso_NoAlg;
   Bool_t          EF_mu0_unpaired_noniso_NoAlg;
   Bool_t          EF_mu10;
   Bool_t          EF_mu100_MSonly;
   Bool_t          EF_mu10_Jpsimumu;
   Bool_t          EF_mu10_MG;
   Bool_t          EF_mu10_NL;
   Bool_t          EF_mu10_Upsimumu_FS;
   Bool_t          EF_mu10_Upsimumu_tight_FS;
   Bool_t          EF_mu10_loose;
   Bool_t          EF_mu10_muCombTag_NoEF;
   Bool_t          EF_mu11_empty_NoAlg;
   Bool_t          EF_mu13;
   Bool_t          EF_mu13_MG;
   Bool_t          EF_mu13_muCombTag_NoEF;
   Bool_t          EF_mu15;
   Bool_t          EF_mu15_EFxe45;
   Bool_t          EF_mu15_MG;
   Bool_t          EF_mu15_j90_a4tc_EFFS;
   Bool_t          EF_mu15_medium_EFxs65_noMu;
   Bool_t          EF_mu15_medium_EFxs70_noMu;
   Bool_t          EF_mu15_medium_EFxs75_noMu;
   Bool_t          EF_mu15_mu10_EFFS;
   Bool_t          EF_mu15_mu10_EFFS_medium;
   Bool_t          EF_mu15_mu10_EFFS_tight;
   Bool_t          EF_mu15_mu7_EFFS;
   Bool_t          EF_mu15_xe30_noMu;
   Bool_t          EF_mu15_xe35_noMu;
   Bool_t          EF_mu15i;
   Bool_t          EF_mu15i_medium;
   Bool_t          EF_mu15ii_debug;
   Bool_t          EF_mu18;
   Bool_t          EF_mu18_L1J10;
   Bool_t          EF_mu18_MG;
   Bool_t          EF_mu18_MG_L1J10;
   Bool_t          EF_mu18_MG_medium;
   Bool_t          EF_mu18_medium;
   Bool_t          EF_mu20;
   Bool_t          EF_mu20_IDTrkNoCut;
   Bool_t          EF_mu20_IDTrkNoCut_ManyVx;
   Bool_t          EF_mu20_L2Star_IDTrkNoCut;
   Bool_t          EF_mu20_MG;
   Bool_t          EF_mu20_MG_medium;
   Bool_t          EF_mu20_MSonly;
   Bool_t          EF_mu20_empty;
   Bool_t          EF_mu20_medium;
   Bool_t          EF_mu20_mu10_EFFS_tight;
   Bool_t          EF_mu20_mu7_EFFS;
   Bool_t          EF_mu20_muCombTag_NoEF;
   Bool_t          EF_mu20i;
   Bool_t          EF_mu20i_medium;
   Bool_t          EF_mu22;
   Bool_t          EF_mu22_MG;
   Bool_t          EF_mu22_MG_medium;
   Bool_t          EF_mu22_medium;
   Bool_t          EF_mu24_MG_medium;
   Bool_t          EF_mu24_MG_tight;
   Bool_t          EF_mu24_medium;
   Bool_t          EF_mu24_tight;
   Bool_t          EF_mu30_MG_medium;
   Bool_t          EF_mu30_MG_tight;
   Bool_t          EF_mu30_medium;
   Bool_t          EF_mu30_tight;
   Bool_t          EF_mu4;
   Bool_t          EF_mu40_MSonly;
   Bool_t          EF_mu40_MSonly_barrel;
   Bool_t          EF_mu40_MSonly_barrel_medium;
   Bool_t          EF_mu40_MSonly_empty;
   Bool_t          EF_mu40_MSonly_tight;
   Bool_t          EF_mu40_MSonly_tight_L1MU11;
   Bool_t          EF_mu40_MSonly_tighter;
   Bool_t          EF_mu40_slow;
   Bool_t          EF_mu40_slow_empty;
   Bool_t          EF_mu40_slow_medium;
   Bool_t          EF_mu40_slow_outOfTime;
   Bool_t          EF_mu40_slow_outOfTime_medium;
   Bool_t          EF_mu4_Bmumu;
   Bool_t          EF_mu4_BmumuX;
   Bool_t          EF_mu4_DiMu;
   Bool_t          EF_mu4_DiMu_FS;
   Bool_t          EF_mu4_DiMu_FS_noOS;
   Bool_t          EF_mu4_DiMu_MG;
   Bool_t          EF_mu4_DiMu_MG_FS;
   Bool_t          EF_mu4_DiMu_SiTrk;
   Bool_t          EF_mu4_DiMu_SiTrk_FS;
   Bool_t          EF_mu4_DiMu_noOS;
   Bool_t          EF_mu4_Jpsimumu;
   Bool_t          EF_mu4_Jpsimumu_FS;
   Bool_t          EF_mu4_Jpsimumu_tight;
   Bool_t          EF_mu4_L1J10_matched;
   Bool_t          EF_mu4_L1J15_matched;
   Bool_t          EF_mu4_L1J20_matched;
   Bool_t          EF_mu4_L1J30_matched;
   Bool_t          EF_mu4_L1J50_matched;
   Bool_t          EF_mu4_L1J75_matched;
   Bool_t          EF_mu4_L1MU11_MSonly_cosmic;
   Bool_t          EF_mu4_L1MU11_cosmic;
   Bool_t          EF_mu4_MSonly_cosmic;
   Bool_t          EF_mu4_Trk_Jpsi;
   Bool_t          EF_mu4_Trk_Jpsi_loose;
   Bool_t          EF_mu4_Trk_Jpsi_looser;
   Bool_t          EF_mu4_Trk_Upsi_FS;
   Bool_t          EF_mu4_Upsimumu_FS;
   Bool_t          EF_mu4_Upsimumu_SiTrk_FS;
   Bool_t          EF_mu4_Upsimumu_tight_FS;
   Bool_t          EF_mu4_cosmic;
   Bool_t          EF_mu4_firstempty;
   Bool_t          EF_mu4_j10_a4tc_EFFS;
   Bool_t          EF_mu4_j10_a4tc_EFFS_matched;
   Bool_t          EF_mu4_j135_a4tc_EFFS_L1matched;
   Bool_t          EF_mu4_j180_a4tc_EFFS_L1matched;
   Bool_t          EF_mu4_j20_a4tc_EFFS_matched;
   Bool_t          EF_mu4_j30;
   Bool_t          EF_mu4_j45_a4tc_EFFS_xe45_loose_noMu;
   Bool_t          EF_mu4_mu6;
   Bool_t          EF_mu4_unpaired_iso;
   Bool_t          EF_mu4imu6i_DiMu_DY;
   Bool_t          EF_mu4imu6i_DiMu_DY14_noVtx_noOS;
   Bool_t          EF_mu4imu6i_DiMu_DY_noVtx_noOS;
   Bool_t          EF_mu4mu6_Bmumu;
   Bool_t          EF_mu4mu6_Bmumux;
   Bool_t          EF_mu4mu6_DiMu;
   Bool_t          EF_mu4mu6_DiMu_DY;
   Bool_t          EF_mu4mu6_DiMu_DY20;
   Bool_t          EF_mu4mu6_DiMu_DY_noVtx_noOS;
   Bool_t          EF_mu4mu6_DiMu_noVtx_noOS;
   Bool_t          EF_mu4mu6_Jpsimumu;
   Bool_t          EF_mu4mu6_Jpsimumu_IDTrkNoCut;
   Bool_t          EF_mu4mu6_Upsimumu;
   Bool_t          EF_mu6;
   Bool_t          EF_mu60_MSonly;
   Bool_t          EF_mu6_Bmumu;
   Bool_t          EF_mu6_BmumuX;
   Bool_t          EF_mu6_DiMu;
   Bool_t          EF_mu6_DiMu_noOS;
   Bool_t          EF_mu6_Jpsimumu;
   Bool_t          EF_mu6_Jpsimumu_SiTrk;
   Bool_t          EF_mu6_Jpsimumu_tight;
   Bool_t          EF_mu6_Trk_Jpsi_loose;
   Bool_t          EF_mu6_Upsimumu_FS;
   Bool_t          EF_mu6_Upsimumu_tight_FS;
   Bool_t          EF_mu80_MSonly;
   Bool_t          EF_rd0_empty_NoAlg;
   Bool_t          EF_rd0_filled_NoAlg;
   Bool_t          EF_rd1_empty_NoAlg;
   Bool_t          EF_tau100_loose;
   Bool_t          EF_tau100_medium;
   Bool_t          EF_tau125_loose;
   Bool_t          EF_tau125_medium;
   Bool_t          EF_tau125_medium1;
   Bool_t          EF_tau16_IDTrkNoCut;
   Bool_t          EF_tau16_L2Star_IDTrkNoCut;
   Bool_t          EF_tau16_j75_a4tc_EFFS_xe40_loose_noMu;
   Bool_t          EF_tau16_loose;
   Bool_t          EF_tau16_loose_e10_loose;
   Bool_t          EF_tau16_loose_e15_loose;
   Bool_t          EF_tau16_loose_e15_medium;
   Bool_t          EF_tau16_loose_e15_tight;
   Bool_t          EF_tau16_loose_mu10;
   Bool_t          EF_tau16_loose_mu15;
   Bool_t          EF_tau16_loose_xe20_noMu;
   Bool_t          EF_tau16_loose_xe25_noMu;
   Bool_t          EF_tau16_loose_xe30_noMu;
   Bool_t          EF_tau16_medium;
   Bool_t          EF_tau16_medium_e15_medium;
   Bool_t          EF_tau16_medium_e15_tight;
   Bool_t          EF_tau16_medium_mu10;
   Bool_t          EF_tau16_medium_mu15;
   Bool_t          EF_tau16_medium_xe22_noMu;
   Bool_t          EF_tau16_medium_xe25_noMu;
   Bool_t          EF_tau16_medium_xe25_tight_noMu;
   Bool_t          EF_tau20T_medium;
   Bool_t          EF_tau20T_medium_e15_medium;
   Bool_t          EF_tau20T_medium_e15_tight;
   Bool_t          EF_tau20T_medium_mu15;
   Bool_t          EF_tau20_loose;
   Bool_t          EF_tau20_loose_e15_medium;
   Bool_t          EF_tau20_loose_mu15;
   Bool_t          EF_tau20_loose_tau29_loose;
   Bool_t          EF_tau20_medium;
   Bool_t          EF_tau20_medium1;
   Bool_t          EF_tau20_medium_e15_medium;
   Bool_t          EF_tau20_medium_e15_tight;
   Bool_t          EF_tau20_medium_mu15;
   Bool_t          EF_tau29T_loose_xs75_noMu;
   Bool_t          EF_tau29T_medium;
   Bool_t          EF_tau29T_medium1_tau20T_medium1;
   Bool_t          EF_tau29T_medium1_xs45_loose_noMu_3L1J10;
   Bool_t          EF_tau29T_medium_xs70_noMu;
   Bool_t          EF_tau29T_medium_xs75_loose_noMu;
   Bool_t          EF_tau29T_medium_xs75_noMu;
   Bool_t          EF_tau29_loose;
   Bool_t          EF_tau29_loose1;
   Bool_t          EF_tau29_loose1_xs45_loose_noMu_3L1J10;
   Bool_t          EF_tau29_loose_e15_tight;
   Bool_t          EF_tau29_loose_j45_a4tc_EFFS_xe40_noMu;
   Bool_t          EF_tau29_loose_mu15;
   Bool_t          EF_tau29_loose_tau38_loose;
   Bool_t          EF_tau29_loose_tau50_loose;
   Bool_t          EF_tau29_loose_xs45_loose_noMu_3L1J10;
   Bool_t          EF_tau29_loose_xs60_loose_noMu_3L1J10;
   Bool_t          EF_tau29_loose_xs65_loose_noMu;
   Bool_t          EF_tau29_loose_xs70_loose_noMu;
   Bool_t          EF_tau29_loose_xs75_loose_noMu;
   Bool_t          EF_tau29_loose_xs75_noMu;
   Bool_t          EF_tau29_loose_xs80_loose_noMu;
   Bool_t          EF_tau29_medium;
   Bool_t          EF_tau29_medium1;
   Bool_t          EF_tau29_medium1_tau20_loose_1prong;
   Bool_t          EF_tau29_medium1_tau20_medium1;
   Bool_t          EF_tau29_medium1_xs45_loose_noMu_3L1J10;
   Bool_t          EF_tau29_medium_tau20_loose_1prong;
   Bool_t          EF_tau29_medium_tau38_loose;
   Bool_t          EF_tau29_medium_tau38_medium;
   Bool_t          EF_tau29_medium_xe35_noMu;
   Bool_t          EF_tau29_medium_xe40_loose_noMu;
   Bool_t          EF_tau29_medium_xs45_loose_noMu_3L1J10;
   Bool_t          EF_tau29_medium_xs70_noMu;
   Bool_t          EF_tau29_medium_xs75_noMu;
   Bool_t          EF_tau29_medium_xs80_loose_noMu;
   Bool_t          EF_tau38T_medium1_tau29T_medium1;
   Bool_t          EF_tau38_loose;
   Bool_t          EF_tau38_medium;
   Bool_t          EF_tau38_medium1_tau29_medium1;
   Bool_t          EF_tau50_IDTrkNoCut;
   Bool_t          EF_tau50_loose;
   Bool_t          EF_tau50_loose_IdScan;
   Bool_t          EF_tau50_medium;
   Bool_t          EF_tau84_loose;
   Bool_t          EF_tau84_medium;
   Bool_t          EF_tauNoCut;
   Bool_t          EF_tauNoCut_L1TAU50;
   Bool_t          EF_tauNoCut_cosmic;
   Bool_t          EF_tauNoCut_firstempty;
   Bool_t          EF_tauNoCut_unpaired_iso;
   Bool_t          EF_tauNoCut_unpaired_noniso;
   Bool_t          EF_te1000;
   Bool_t          EF_te1100;
   Bool_t          EF_te1200;
   Bool_t          EF_te550;
   Bool_t          EF_te700;
   Bool_t          EF_te900;
   Bool_t          EF_xe20_noMu;
   Bool_t          EF_xe30_noMu;
   Bool_t          EF_xe35_noMu;
   Bool_t          EF_xe40_noMu;
   Bool_t          EF_xe50_noMu;
   Bool_t          EF_xe60_noMu;
   Bool_t          EF_xe60_tight_noMu;
   Bool_t          EF_xe60_verytight_noMu;
   Bool_t          EF_xe70_noMu;
   Bool_t          EF_xe70_tight_noMu;
   Bool_t          EF_xe80_noMu;
   Bool_t          EF_xe90_noMu;
   Bool_t          EF_xe_NoCut_allL1;
   Bool_t          EF_xs100_noMu;
   Bool_t          EF_xs30_noMu;
   Bool_t          EF_xs45_noMu;
   Bool_t          EF_xs60_noMu;
   Bool_t          EF_xs60_noMu_L1EM10XS45;
   Bool_t          EF_xs70_noMu;
   Bool_t          EF_xs70_tight_noMu;
   Bool_t          EF_xs75_noMu;
   Bool_t          EF_xs75_noMu_L1EM10XS50;
   Bool_t          EF_xs90_noMu;
   Bool_t          EF_zerobias_NoAlg;
   Bool_t          L1_2EM10;
   Bool_t          L1_2EM14;
   Bool_t          L1_2EM3;
   Bool_t          L1_2EM3_EM7;
   Bool_t          L1_2EM5;
   Bool_t          L1_2EM7;
   Bool_t          L1_2J10_J30_JE140;
   Bool_t          L1_2J10_J50;
   Bool_t          L1_2J10_J75;
   Bool_t          L1_2J20_XE20;
   Bool_t          L1_2J30_XE20;
   Bool_t          L1_2MU0;
   Bool_t          L1_2MU0_XE20;
   Bool_t          L1_2MU0_XE25;
   Bool_t          L1_2MU10;
   Bool_t          L1_2MU6;
   Bool_t          L1_2TAU11;
   Bool_t          L1_2TAU11I;
   Bool_t          L1_2TAU11_EM10;
   Bool_t          L1_2TAU11_TAU15;
   Bool_t          L1_2TAU15;
   Bool_t          L1_2TAU15_TAU20;
   Bool_t          L1_2TAU6;
   Bool_t          L1_2TAU6_EM10;
   Bool_t          L1_2TAU6_EM5;
   Bool_t          L1_2TAU8_EM10;
   Bool_t          L1_2TAU8_TAU11;
   Bool_t          L1_3EM5;
   Bool_t          L1_3J10;
   Bool_t          L1_3J10_J50;
   Bool_t          L1_3J15;
   Bool_t          L1_3J50;
   Bool_t          L1_4J10;
   Bool_t          L1_EM10;
   Bool_t          L1_EM10_XE20;
   Bool_t          L1_EM10_XE30;
   Bool_t          L1_EM12;
   Bool_t          L1_EM14;
   Bool_t          L1_EM14_XE10;
   Bool_t          L1_EM14_XE20;
   Bool_t          L1_EM16;
   Bool_t          L1_EM30;
   Bool_t          L1_J10;
   Bool_t          L1_J15;
   Bool_t          L1_J175;
   Bool_t          L1_J20;
   Bool_t          L1_J250;
   Bool_t          L1_J30;
   Bool_t          L1_J30_XE35;
   Bool_t          L1_J30_XE40;
   Bool_t          L1_J50;
   Bool_t          L1_J50_XE20;
   Bool_t          L1_J50_XE30;
   Bool_t          L1_J50_XE35;
   Bool_t          L1_J50_XE40;
   Bool_t          L1_J75;
   Bool_t          L1_JE100;
   Bool_t          L1_JE140;
   Bool_t          L1_JE200;
   Bool_t          L1_MU0;
   Bool_t          L1_MU0_J10;
   Bool_t          L1_MU0_J20_XE20;
   Bool_t          L1_MU10;
   Bool_t          L1_MU10_XE20;
   Bool_t          L1_MU11;
   Bool_t          L1_MU15;
   Bool_t          L1_MU20;
   Bool_t          L1_MU6;
   Bool_t          L1_TAU11;
   Bool_t          L1_TAU11_2J10_J20_XE20;
   Bool_t          L1_TAU11_MU10;
   Bool_t          L1_TAU11_XE10_3J10;
   Bool_t          L1_TAU11_XE20;
   Bool_t          L1_TAU11_XS30;
   Bool_t          L1_TAU11_XS35;
   Bool_t          L1_TAU15;
   Bool_t          L1_TAU15_XE10_3J10;
   Bool_t          L1_TAU15_XE20;
   Bool_t          L1_TAU15_XS35;
   Bool_t          L1_TAU20;
   Bool_t          L1_TAU30;
   Bool_t          L1_TAU50;
   Bool_t          L1_TAU6;
   Bool_t          L1_TAU6_J50_XE20;
   Bool_t          L1_TAU6_MU10;
   Bool_t          L1_TAU6_XE10;
   Bool_t          L1_TAU8;
   Bool_t          L1_TAU8_EMPTY;
   Bool_t          L1_TAU8_FIRSTEMPTY;
   Bool_t          L1_TAU8_MU10;
   Bool_t          L1_TAU8_UNPAIRED_ISO;
   Bool_t          L1_TAU8_UNPAIRED_NONISO;
   Bool_t          L1_XE10;
   Bool_t          L1_XE20;
   Bool_t          L1_XE25;
   Bool_t          L1_XE30;
   Bool_t          L1_XE35;
   Bool_t          L1_XE40;
   Bool_t          L1_XE50;
   Bool_t          L1_XE60;
   Bool_t          L1_XS15;
   Bool_t          L1_XS20;
   Bool_t          L1_XS25;
   Bool_t          L1_XS30;
   Bool_t          L1_XS35;
   Bool_t          L1_XS40;
   Bool_t          L1_XS45;
   Bool_t          L1_XS50;
   Bool_t          L2_2b10_medium_3L1J10;
   Bool_t          L2_2b10_medium_4L1J10;
   Bool_t          L2_2b10_medium_4j25;
   Bool_t          L2_2b10_medium_L1JE100;
   Bool_t          L2_2b10_medium_L1JE140;
   Bool_t          L2_2b10_medium_L1_2J10J50;
   Bool_t          L2_2b10_medium_j70_j25;
   Bool_t          L2_2b10_medium_j95_j25;
   Bool_t          L2_2b10_tight_4j25;
   Bool_t          L2_2b15_medium_3L1J15;
   Bool_t          L2_2b15_medium_3j35;
   Bool_t          L2_2b15_medium_j70_j35;
   Bool_t          L2_2b20_medium_3L1J20;
   Bool_t          L2_2b20_medium_3j40;
   Bool_t          L2_2b20_medium_j70_j40;
   Bool_t          L2_2e10_medium;
   Bool_t          L2_2e12_medium;
   Bool_t          L2_2g15_loose;
   Bool_t          L2_2g20_loose;
   Bool_t          L2_2j25_j70_dphi03;
   Bool_t          L2_2j40_anymct100_xe20_medium_noMu;
   Bool_t          L2_2j50_anymct100_xe20_medium_noMu;
   Bool_t          L2_2mu10;
   Bool_t          L2_2mu4;
   Bool_t          L2_2mu4_xe20_noMu;
   Bool_t          L2_2mu4_xe25_noMu;
   Bool_t          L2_2tau16_loose;
   Bool_t          L2_2tau29T_medium1;
   Bool_t          L2_2tau29_loose;
   Bool_t          L2_2tau29_loose1;
   Bool_t          L2_2tau29_loose1_EFxe15_noMu;
   Bool_t          L2_2tau29_loose1_EFxe20_noMu;
   Bool_t          L2_2tau29_medium1;
   Bool_t          L2_2tau29i_medium1;
   Bool_t          L2_2tau38_loose;
   Bool_t          L2_3b10_loose_4L1J10;
   Bool_t          L2_3b10_medium_4j25;
   Bool_t          L2_3b15_loose_4L1J15;
   Bool_t          L2_3b15_medium_4j35;
   Bool_t          L2_3e10_medium;
   Bool_t          L2_3j25;
   Bool_t          L2_3j70;
   Bool_t          L2_4j25;
   Bool_t          L2_5j25;
   Bool_t          L2_6j25;
   Bool_t          L2_MU0_j40_xe20_loose_noMu;
   Bool_t          L2_b10_IDTrkNoCut;
   Bool_t          L2_b10_L2Star_IDTrkNoCut;
   Bool_t          L2_b10_medium_4L1J10;
   Bool_t          L2_b10_medium_4j25;
   Bool_t          L2_b10_medium_EFxe25_noMu_L1JE100;
   Bool_t          L2_b10_medium_EFxe25_noMu_L1JE140;
   Bool_t          L2_b10_medium_EFxe25_noMu_L1_2J10J50;
   Bool_t          L2_b10_medium_j70_2j50_4j25;
   Bool_t          L2_b10_tight_4L1J10;
   Bool_t          L2_b10_tight_4j25;
   Bool_t          L2_b10_tight_L1JE100;
   Bool_t          L2_b10_tight_L1JE140;
   Bool_t          L2_b10_tight_j70_2j50_4j25;
   Bool_t          L2_b15_IDTrkNoCut;
   Bool_t          L2_b20_IDTrkNoCut;
   Bool_t          L2_e10_medium;
   Bool_t          L2_e15_medium;
   Bool_t          L2_e15_medium_xe20_noMu;
   Bool_t          L2_e15_medium_xe25_noMu;
   Bool_t          L2_e15_medium_xe30_noMu;
   Bool_t          L2_e15_medium_xe35_noMu;
   Bool_t          L2_e15_medium_xe40_loose_noMu;
   Bool_t          L2_e15_tight;
   Bool_t          L2_e15_tight_xe30_noMu;
   Bool_t          L2_e18_medium_xe20_noMu;
   Bool_t          L2_e20_loose;
   Bool_t          L2_e20_loose_xe20;
   Bool_t          L2_e20_loose_xe20_noMu;
   Bool_t          L2_e20_loose_xe30;
   Bool_t          L2_e20_loose_xe30_noMu;
   Bool_t          L2_e20_medium;
   Bool_t          L2_e20_medium1;
   Bool_t          L2_e22_medium;
   Bool_t          L2_e60_loose;
   Bool_t          L2_g11_etcut;
   Bool_t          L2_g150_etcut;
   Bool_t          L2_g15_loose;
   Bool_t          L2_g20_etcut;
   Bool_t          L2_g20_etcut_xe30_noMu;
   Bool_t          L2_g20_loose;
   Bool_t          L2_g20_loose_xe20_noMu;
   Bool_t          L2_g20_loose_xe30_noMu;
   Bool_t          L2_g25_loose_xe30_noMu;
   Bool_t          L2_g40_loose;
   Bool_t          L2_g40_loose_xe30_medium_noMu;
   Bool_t          L2_g40_loose_xe35_medium_noMu;
   Bool_t          L2_g40_loose_xe40_noMu;
   Bool_t          L2_g60_loose;
   Bool_t          L2_g80_loose;
   Bool_t          L2_j25;
   Bool_t          L2_j35;
   Bool_t          L2_j50;
   Bool_t          L2_j50_xe35_medium_noMu;
   Bool_t          L2_j50_xe35_medium_noMu_l2cleancons;
   Bool_t          L2_j60_xe45_noMu;
   Bool_t          L2_j70;
   Bool_t          L2_j70_xe20_loose_noMu;
   Bool_t          L2_j70_xe25_loose_noMu;
   Bool_t          L2_j70_xe35_noMu;
   Bool_t          L2_j70_xe35_noMu_l2cleancons;
   Bool_t          L2_j75_xe40_noMu;
   Bool_t          L2_j95;
   Bool_t          L2_je255;
   Bool_t          L2_mu10;
   Bool_t          L2_mu13;
   Bool_t          L2_mu13_MG;
   Bool_t          L2_mu15;
   Bool_t          L2_mu15_xe20_noMu;
   Bool_t          L2_mu18_MG;
   Bool_t          L2_mu20;
   Bool_t          L2_mu20_MG;
   Bool_t          L2_mu20_MSonly;
   Bool_t          L2_mu22_MG;
   Bool_t          L2_mu4;
   Bool_t          L2_mu4_L1J10_matched;
   Bool_t          L2_mu4_L1J20_matched;
   Bool_t          L2_mu4_L1J30_matched;
   Bool_t          L2_mu4_L1J50_matched;
   Bool_t          L2_mu4_L1J75_matched;
   Bool_t          L2_mu4_j10_a4tc_EFFS;
   Bool_t          L2_mu4_j40_xe20_loose_noMu;
   Bool_t          L2_mu6;
   Bool_t          L2_tau100_loose;
   Bool_t          L2_tau100_medium;
   Bool_t          L2_tau125_loose;
   Bool_t          L2_tau125_medium;
   Bool_t          L2_tau125_medium1;
   Bool_t          L2_tau16_IDTrkNoCut;
   Bool_t          L2_tau16_L2Star_IDTrkNoCut;
   Bool_t          L2_tau16_j70_xe20_loose_noMu;
   Bool_t          L2_tau16_loose;
   Bool_t          L2_tau16_loose_e10_loose;
   Bool_t          L2_tau16_loose_e15_loose;
   Bool_t          L2_tau16_loose_e15_medium;
   Bool_t          L2_tau16_loose_e15_tight;
   Bool_t          L2_tau16_loose_mu10;
   Bool_t          L2_tau16_loose_mu15;
   Bool_t          L2_tau16_loose_xe20_noMu;
   Bool_t          L2_tau16_loose_xe25_noMu;
   Bool_t          L2_tau16_loose_xe30_noMu;
   Bool_t          L2_tau16_medium;
   Bool_t          L2_tau16_medium_e15_medium;
   Bool_t          L2_tau16_medium_e15_tight;
   Bool_t          L2_tau16_medium_mu10;
   Bool_t          L2_tau16_medium_mu15;
   Bool_t          L2_tau16_medium_xe22_noMu;
   Bool_t          L2_tau16_medium_xe25_noMu;
   Bool_t          L2_tau16_medium_xe25_tight_noMu;
   Bool_t          L2_tau20T_medium;
   Bool_t          L2_tau20T_medium_e15_medium;
   Bool_t          L2_tau20T_medium_e15_tight;
   Bool_t          L2_tau20T_medium_mu15;
   Bool_t          L2_tau20_loose;
   Bool_t          L2_tau20_loose_e15_medium;
   Bool_t          L2_tau20_loose_mu15;
   Bool_t          L2_tau20_loose_tau29_loose;
   Bool_t          L2_tau20_medium;
   Bool_t          L2_tau20_medium1;
   Bool_t          L2_tau20_medium_e15_medium;
   Bool_t          L2_tau20_medium_e15_tight;
   Bool_t          L2_tau20_medium_mu15;
   Bool_t          L2_tau29T_loose_xs50_noMu;
   Bool_t          L2_tau29T_medium;
   Bool_t          L2_tau29T_medium1_tau20T_medium1;
   Bool_t          L2_tau29T_medium1_xs20_noMu_3L1J10;
   Bool_t          L2_tau29T_medium_xs35_noMu;
   Bool_t          L2_tau29T_medium_xs50_noMu;
   Bool_t          L2_tau29T_medium_xs50_noMu_1;
   Bool_t          L2_tau29_loose;
   Bool_t          L2_tau29_loose1;
   Bool_t          L2_tau29_loose1_xs20_noMu_3L1J10;
   Bool_t          L2_tau29_loose_e15_tight;
   Bool_t          L2_tau29_loose_j40_xe20_noMu;
   Bool_t          L2_tau29_loose_mu15;
   Bool_t          L2_tau29_loose_tau38_loose;
   Bool_t          L2_tau29_loose_tau50_loose;
   Bool_t          L2_tau29_loose_xs20_noMu_3L1J10;
   Bool_t          L2_tau29_loose_xs35_noMu;
   Bool_t          L2_tau29_loose_xs40_noMu;
   Bool_t          L2_tau29_loose_xs50_noMu;
   Bool_t          L2_tau29_medium;
   Bool_t          L2_tau29_medium1;
   Bool_t          L2_tau29_medium1_tau20_loose_1prong;
   Bool_t          L2_tau29_medium1_tau20_medium1;
   Bool_t          L2_tau29_medium1_xs20_noMu_3L1J10;
   Bool_t          L2_tau29_medium_tau20_loose_1prong;
   Bool_t          L2_tau29_medium_tau38_loose;
   Bool_t          L2_tau29_medium_tau38_medium;
   Bool_t          L2_tau29_medium_xe25_noMu;
   Bool_t          L2_tau29_medium_xe30_loose_noMu;
   Bool_t          L2_tau29_medium_xs20_noMu_3L1J10;
   Bool_t          L2_tau29_medium_xs50_noMu;
   Bool_t          L2_tau29_medium_xs50_noMu_1;
   Bool_t          L2_tau38T_medium1_tau29T_medium1;
   Bool_t          L2_tau38_loose;
   Bool_t          L2_tau38_medium;
   Bool_t          L2_tau38_medium1_tau29_medium1;
   Bool_t          L2_tau50_IDTrkNoCut;
   Bool_t          L2_tau50_loose;
   Bool_t          L2_tau50_loose_IdScan;
   Bool_t          L2_tau50_medium;
   Bool_t          L2_tau84_loose;
   Bool_t          L2_tau84_medium;
   Bool_t          L2_tau8_empty_larcalib;
   Bool_t          L2_tauNoCut;
   Bool_t          L2_tauNoCut_L1TAU50;
   Bool_t          L2_tauNoCut_cosmic;
   Bool_t          L2_tauNoCut_firstempty;
   Bool_t          L2_tauNoCut_unpaired_iso;
   Bool_t          L2_tauNoCut_unpaired_noniso;
   Bool_t          L2_xe10_noMu;
   Bool_t          L2_xe20_noMu;
   Bool_t          L2_xe25_noMu;
   Bool_t          L2_xe30_noMu;
   Bool_t          L2_xe35_noMu;
   Bool_t          L2_xe40_noMu;
   Bool_t          L2_xe45_noMu;
   Bool_t          L2_xe50_noMu;
   Bool_t          L2_xe55_noMu;
   Bool_t          L2_xe60_noMu;
   Bool_t          L2_xe70_noMu;
   vector<int>     *trig_EF_el_EF_2e10_medium;
   vector<int>     *trig_EF_el_EF_2e10_medium_mu6;
   vector<int>     *trig_EF_el_EF_2e11T_medium;
   vector<int>     *trig_EF_el_EF_2e12T_medium;
   vector<int>     *trig_EF_el_EF_2e12_medium;
   vector<int>     *trig_EF_el_EF_2e15_loose;
   vector<int>     *trig_EF_el_EF_2e15_medium;
   vector<int>     *trig_EF_el_EF_2e5_medium;
   vector<int>     *trig_EF_el_EF_2e5_medium_SiTrk;
   vector<int>     *trig_EF_el_EF_2e5_medium_TRT;
   vector<int>     *trig_EF_el_EF_2e5_tight;
   vector<int>     *trig_EF_el_EF_2e5_tight_Jpsi;
   vector<int>     *trig_EF_el_EF_e10_NoCut;
   vector<int>     *trig_EF_el_EF_e10_loose;
   vector<int>     *trig_EF_el_EF_e10_loose_mu10;
   vector<int>     *trig_EF_el_EF_e10_loose_mu6;
   vector<int>     *trig_EF_el_EF_e10_medium;
   vector<int>     *trig_EF_el_EF_e10_medium_2mu6;
   vector<int>     *trig_EF_el_EF_e10_medium_mu10;
   vector<int>     *trig_EF_el_EF_e10_medium_mu6;
   vector<int>     *trig_EF_el_EF_e10_medium_mu6_topo_medium;
   vector<int>     *trig_EF_el_EF_e11T_medium;
   vector<int>     *trig_EF_el_EF_e11T_medium_2e6T_medium;
   vector<int>     *trig_EF_el_EF_e11_etcut;
   vector<int>     *trig_EF_el_EF_e12T_medium_mu6_topo_medium;
   vector<int>     *trig_EF_el_EF_e12_medium;
   vector<int>     *trig_EF_el_EF_e13_etcutTRT_xs60_noMu;
   vector<int>     *trig_EF_el_EF_e13_etcut_xs60_noMu;
   vector<int>     *trig_EF_el_EF_e13_etcut_xs70_noMu;
   vector<int>     *trig_EF_el_EF_e15_HLTtighter;
   vector<int>     *trig_EF_el_EF_e15_loose;
   vector<int>     *trig_EF_el_EF_e15_medium;
   vector<int>     *trig_EF_el_EF_e15_medium_e12_medium;
   vector<int>     *trig_EF_el_EF_e15_medium_mu6;
   vector<int>     *trig_EF_el_EF_e15_medium_xe30_noMu;
   vector<int>     *trig_EF_el_EF_e15_medium_xe35_loose_noMu;
   vector<int>     *trig_EF_el_EF_e15_medium_xe35_noMu;
   vector<int>     *trig_EF_el_EF_e15_medium_xe40_loose_noMu;
   vector<int>     *trig_EF_el_EF_e15_medium_xe40_noMu;
   vector<int>     *trig_EF_el_EF_e15_medium_xe50_noMu;
   vector<int>     *trig_EF_el_EF_e15_tight;
   vector<int>     *trig_EF_el_EF_e15_tight1;
   vector<int>     *trig_EF_el_EF_e15_tight_xe30_noMu;
   vector<int>     *trig_EF_el_EF_e18_medium;
   vector<int>     *trig_EF_el_EF_e18_medium_xe30_noMu;
   vector<int>     *trig_EF_el_EF_e18_tight;
   vector<int>     *trig_EF_el_EF_e20_L2Star_IDTrkNoCut;
   vector<int>     *trig_EF_el_EF_e20_etcut_xs60_noMu;
   vector<int>     *trig_EF_el_EF_e20_loose;
   vector<int>     *trig_EF_el_EF_e20_loose1;
   vector<int>     *trig_EF_el_EF_e20_looseTrk;
   vector<int>     *trig_EF_el_EF_e20_loose_2j10_a4tc_EFFS;
   vector<int>     *trig_EF_el_EF_e20_loose_3j10_a4tc_EFFS;
   vector<int>     *trig_EF_el_EF_e20_loose_4j10_a4tc_EFFS;
   vector<int>     *trig_EF_el_EF_e20_loose_IDTrkNoCut;
   vector<int>     *trig_EF_el_EF_e20_loose_TRT;
   vector<int>     *trig_EF_el_EF_e20_loose_xe20;
   vector<int>     *trig_EF_el_EF_e20_loose_xe20_noMu;
   vector<int>     *trig_EF_el_EF_e20_loose_xe30;
   vector<int>     *trig_EF_el_EF_e20_loose_xe30_noMu;
   vector<int>     *trig_EF_el_EF_e20_medium;
   vector<int>     *trig_EF_el_EF_e20_medium1;
   vector<int>     *trig_EF_el_EF_e20_medium2;
   vector<int>     *trig_EF_el_EF_e20_medium_IDTrkNoCut;
   vector<int>     *trig_EF_el_EF_e20_medium_SiTrk;
   vector<int>     *trig_EF_el_EF_e20_medium_TRT;
   vector<int>     *trig_EF_el_EF_e20_tight;
   vector<int>     *trig_EF_el_EF_e20_tight_e15_NoCut_Zee;
   vector<int>     *trig_EF_el_EF_e22_etcut_xs60_noMu;
   vector<int>     *trig_EF_el_EF_e22_loose;
   vector<int>     *trig_EF_el_EF_e22_loose1;
   vector<int>     *trig_EF_el_EF_e22_looseTrk;
   vector<int>     *trig_EF_el_EF_e22_medium;
   vector<int>     *trig_EF_el_EF_e22_medium1;
   vector<int>     *trig_EF_el_EF_e22_medium2;
   vector<int>     *trig_EF_el_EF_e22_medium_IDTrkNoCut;
   vector<int>     *trig_EF_el_EF_e22_medium_SiTrk;
   vector<int>     *trig_EF_el_EF_e22_medium_TRT;
   vector<int>     *trig_EF_el_EF_e22_tight;
   vector<int>     *trig_EF_el_EF_e25_loose;
   vector<int>     *trig_EF_el_EF_e25_medium;
   vector<int>     *trig_EF_el_EF_e30_loose;
   vector<int>     *trig_EF_el_EF_e30_medium;
   vector<int>     *trig_EF_el_EF_e33_medium;
   vector<int>     *trig_EF_el_EF_e35_medium;
   vector<int>     *trig_EF_el_EF_e40_medium;
   vector<int>     *trig_EF_el_EF_e45_medium;
   vector<int>     *trig_EF_el_EF_e45_medium1;
   vector<int>     *trig_EF_el_EF_e5_NoCut_L2SW;
   vector<int>     *trig_EF_el_EF_e5_NoCut_Ringer;
   vector<int>     *trig_EF_el_EF_e5_NoCut_firstempty;
   vector<int>     *trig_EF_el_EF_e5_medium_mu4;
   vector<int>     *trig_EF_el_EF_e5_medium_mu6;
   vector<int>     *trig_EF_el_EF_e5_medium_mu6_topo_loose;
   vector<int>     *trig_EF_el_EF_e5_medium_mu6_topo_medium;
   vector<int>     *trig_EF_el_EF_e5_tight;
   vector<int>     *trig_EF_el_EF_e5_tight_TRT;
   vector<int>     *trig_EF_el_EF_e5_tight_e14_etcut_Jpsi;
   vector<int>     *trig_EF_el_EF_e5_tight_e4_etcut_Jpsi;
   vector<int>     *trig_EF_el_EF_e5_tight_e4_etcut_Jpsi_SiTrk;
   vector<int>     *trig_EF_el_EF_e5_tight_e4_etcut_Jpsi_TRT;
   vector<int>     *trig_EF_el_EF_e5_tight_e5_NoCut;
   vector<int>     *trig_EF_el_EF_e5_tight_e5_NoCut_Jpsi;
   vector<int>     *trig_EF_el_EF_e5_tight_e7_etcut_Jpsi;
   vector<int>     *trig_EF_el_EF_e5_tight_e9_etcut_Jpsi;
   vector<int>     *trig_EF_el_EF_e60_loose;
   vector<int>     *trig_EF_el_EF_e6T_medium;
   vector<int>     *trig_EF_el_EF_e7_tight_e14_etcut_Jpsi;
   vector<int>     *trig_EF_el_EF_e9_tight_e5_tight_Jpsi;
   vector<int>     *trig_EF_el_EF_eb_physics;
   vector<int>     *trig_EF_el_EF_eb_physics_empty;
   vector<int>     *trig_EF_el_EF_eb_physics_firstempty;
   vector<int>     *trig_EF_el_EF_eb_physics_noL1PS;
   vector<int>     *trig_EF_el_EF_eb_physics_unpaired_iso;
   vector<int>     *trig_EF_el_EF_eb_physics_unpaired_noniso;
   vector<int>     *trig_EF_el_EF_eb_random;
   vector<int>     *trig_EF_el_EF_eb_random_empty;
   vector<int>     *trig_EF_el_EF_eb_random_firstempty;
   vector<int>     *trig_EF_el_EF_eb_random_unpaired_iso;
   vector<int>     *trig_EF_el_EF_em105_passHLT;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10_NoAlg;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10_empty;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10_loose;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10_loose1;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10_loose_empty;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10_loose_noOvlpRm;
   vector<int>     *trig_EF_trigmuonef_EF_2mu10_noOvlpRm;
   vector<int>     *trig_EF_trigmuonef_EF_2mu13_Zmumu_IDTrkNoCut;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_Bmumu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_Bmumux;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_DiMu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_DiMu_DY;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_DiMu_DY20;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_DiMu_SiTrk;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_DiMu_noVtx;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_DiMu_noVtx_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_EFxe20_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_Jpsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_Jpsimumu_IDTrkNoCut;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_Upsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_xe30_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4_xe40_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4i_DiMu_DY;
   vector<int>     *trig_EF_trigmuonef_EF_2mu4i_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_Bmumu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_Bmumux;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_DiMu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_Jpsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_MSonly_g10_loose;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_MSonly_g10_loose_noOvlpRm;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_MSonly_g10_loose_nonfilled;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_NL;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_Upsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_e10_loose;
   vector<int>     *trig_EF_trigmuonef_EF_2mu6_g10_loose;
   vector<int>     *trig_EF_trigmuonef_EF_mu0_empty_NoAlg;
   vector<int>     *trig_EF_trigmuonef_EF_mu0_firstempty_NoAlg;
   vector<int>     *trig_EF_trigmuonef_EF_mu0_missingRoi;
   vector<int>     *trig_EF_trigmuonef_EF_mu0_unpaired_iso_NoAlg;
   vector<int>     *trig_EF_trigmuonef_EF_mu0_unpaired_noniso_NoAlg;
   vector<int>     *trig_EF_trigmuonef_EF_mu10;
   vector<int>     *trig_EF_trigmuonef_EF_mu100_MSonly;
   vector<int>     *trig_EF_trigmuonef_EF_mu10_Jpsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu10_MG;
   vector<int>     *trig_EF_trigmuonef_EF_mu10_NL;
   vector<int>     *trig_EF_trigmuonef_EF_mu10_Upsimumu_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu10_Upsimumu_tight_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu10_loose;
   vector<int>     *trig_EF_trigmuonef_EF_mu10_muCombTag_NoEF;
   vector<int>     *trig_EF_trigmuonef_EF_mu11_empty_NoAlg;
   vector<int>     *trig_EF_trigmuonef_EF_mu13;
   vector<int>     *trig_EF_trigmuonef_EF_mu13_MG;
   vector<int>     *trig_EF_trigmuonef_EF_mu13_muCombTag_NoEF;
   vector<int>     *trig_EF_trigmuonef_EF_mu15;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_EFxe45;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_MG;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_j90_a4tc_EFFS;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_medium_EFxs65_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_medium_EFxs70_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_medium_EFxs75_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_mu10_EFFS;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_mu10_EFFS_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_mu10_EFFS_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_mu7_EFFS;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_xe30_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu15_xe35_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu15i;
   vector<int>     *trig_EF_trigmuonef_EF_mu15i_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu15ii_debug;
   vector<int>     *trig_EF_trigmuonef_EF_mu18;
   vector<int>     *trig_EF_trigmuonef_EF_mu18_L1J10;
   vector<int>     *trig_EF_trigmuonef_EF_mu18_MG;
   vector<int>     *trig_EF_trigmuonef_EF_mu18_MG_L1J10;
   vector<int>     *trig_EF_trigmuonef_EF_mu18_MG_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu18_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu20;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_IDTrkNoCut;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_IDTrkNoCut_ManyVx;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_L2Star_IDTrkNoCut;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_MG;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_MG_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_MSonly;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_empty;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_mu10_EFFS_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_mu7_EFFS;
   vector<int>     *trig_EF_trigmuonef_EF_mu20_muCombTag_NoEF;
   vector<int>     *trig_EF_trigmuonef_EF_mu20i;
   vector<int>     *trig_EF_trigmuonef_EF_mu20i_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu22;
   vector<int>     *trig_EF_trigmuonef_EF_mu22_MG;
   vector<int>     *trig_EF_trigmuonef_EF_mu22_MG_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu22_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu24_MG_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu24_MG_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu24_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu24_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu30_MG_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu30_MG_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu30_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu30_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu4;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_MSonly;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_MSonly_barrel;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_MSonly_barrel_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_MSonly_empty;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_MSonly_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_MSonly_tight_L1MU11;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_MSonly_tighter;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_slow;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_slow_empty;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_slow_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_slow_outOfTime;
   vector<int>     *trig_EF_trigmuonef_EF_mu40_slow_outOfTime_medium;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Bmumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_BmumuX;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu_FS_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu_MG;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu_MG_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu_SiTrk;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu_SiTrk_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_DiMu_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Jpsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Jpsimumu_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Jpsimumu_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1J10_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1J15_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1J20_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1J30_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1J50_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1J75_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1MU11_MSonly_cosmic;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_L1MU11_cosmic;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_MSonly_cosmic;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Trk_Jpsi;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Trk_Jpsi_loose;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Trk_Jpsi_looser;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Trk_Upsi_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Upsimumu_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Upsimumu_SiTrk_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_Upsimumu_tight_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_cosmic;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_firstempty;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_j10_a4tc_EFFS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_j10_a4tc_EFFS_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_j135_a4tc_EFFS_L1matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_j180_a4tc_EFFS_L1matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_j20_a4tc_EFFS_matched;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_j30;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_j45_a4tc_EFFS_xe45_loose_noMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_mu6;
   vector<int>     *trig_EF_trigmuonef_EF_mu4_unpaired_iso;
   vector<int>     *trig_EF_trigmuonef_EF_mu4imu6i_DiMu_DY;
   vector<int>     *trig_EF_trigmuonef_EF_mu4imu6i_DiMu_DY14_noVtx_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4imu6i_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_Bmumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_Bmumux;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_DiMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_DiMu_DY;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_DiMu_DY20;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_DiMu_noVtx_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_Jpsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_Jpsimumu_IDTrkNoCut;
   vector<int>     *trig_EF_trigmuonef_EF_mu4mu6_Upsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu6;
   vector<int>     *trig_EF_trigmuonef_EF_mu60_MSonly;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_Bmumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_BmumuX;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_DiMu;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_DiMu_noOS;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_Jpsimumu;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_Jpsimumu_SiTrk;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_Jpsimumu_tight;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_Trk_Jpsi_loose;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_Upsimumu_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu6_Upsimumu_tight_FS;
   vector<int>     *trig_EF_trigmuonef_EF_mu80_MSonly;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10_NoAlg;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10_empty;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10_loose;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10_loose1;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10_loose_empty;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10_loose_noOvlpRm;
   vector<int>     *trig_EF_trigmugirl_EF_2mu10_noOvlpRm;
   vector<int>     *trig_EF_trigmugirl_EF_2mu13_Zmumu_IDTrkNoCut;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_Bmumu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_Bmumux;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_DiMu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_DiMu_DY;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_DiMu_DY20;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_DiMu_SiTrk;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_DiMu_noVtx;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_DiMu_noVtx_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_EFxe20_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_Jpsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_Jpsimumu_IDTrkNoCut;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_Upsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_xe30_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4_xe40_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4i_DiMu_DY;
   vector<int>     *trig_EF_trigmugirl_EF_2mu4i_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_Bmumu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_Bmumux;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_DiMu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_Jpsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_MSonly_g10_loose;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_MSonly_g10_loose_noOvlpRm;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_MSonly_g10_loose_nonfilled;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_NL;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_Upsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_e10_loose;
   vector<int>     *trig_EF_trigmugirl_EF_2mu6_g10_loose;
   vector<int>     *trig_EF_trigmugirl_EF_mu0_empty_NoAlg;
   vector<int>     *trig_EF_trigmugirl_EF_mu0_firstempty_NoAlg;
   vector<int>     *trig_EF_trigmugirl_EF_mu0_missingRoi;
   vector<int>     *trig_EF_trigmugirl_EF_mu0_unpaired_iso_NoAlg;
   vector<int>     *trig_EF_trigmugirl_EF_mu0_unpaired_noniso_NoAlg;
   vector<int>     *trig_EF_trigmugirl_EF_mu10;
   vector<int>     *trig_EF_trigmugirl_EF_mu100_MSonly;
   vector<int>     *trig_EF_trigmugirl_EF_mu10_Jpsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu10_MG;
   vector<int>     *trig_EF_trigmugirl_EF_mu10_NL;
   vector<int>     *trig_EF_trigmugirl_EF_mu10_Upsimumu_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu10_Upsimumu_tight_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu10_loose;
   vector<int>     *trig_EF_trigmugirl_EF_mu10_muCombTag_NoEF;
   vector<int>     *trig_EF_trigmugirl_EF_mu11_empty_NoAlg;
   vector<int>     *trig_EF_trigmugirl_EF_mu13;
   vector<int>     *trig_EF_trigmugirl_EF_mu13_MG;
   vector<int>     *trig_EF_trigmugirl_EF_mu13_muCombTag_NoEF;
   vector<int>     *trig_EF_trigmugirl_EF_mu15;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_EFxe45;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_MG;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_j90_a4tc_EFFS;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_medium_EFxs65_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_medium_EFxs70_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_medium_EFxs75_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_mu10_EFFS;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_mu10_EFFS_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_mu10_EFFS_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_mu7_EFFS;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_xe30_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu15_xe35_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu15i;
   vector<int>     *trig_EF_trigmugirl_EF_mu15i_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu15ii_debug;
   vector<int>     *trig_EF_trigmugirl_EF_mu18;
   vector<int>     *trig_EF_trigmugirl_EF_mu18_L1J10;
   vector<int>     *trig_EF_trigmugirl_EF_mu18_MG;
   vector<int>     *trig_EF_trigmugirl_EF_mu18_MG_L1J10;
   vector<int>     *trig_EF_trigmugirl_EF_mu18_MG_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu18_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu20;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_IDTrkNoCut;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_IDTrkNoCut_ManyVx;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_L2Star_IDTrkNoCut;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_MG;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_MG_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_MSonly;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_empty;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_mu10_EFFS_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_mu7_EFFS;
   vector<int>     *trig_EF_trigmugirl_EF_mu20_muCombTag_NoEF;
   vector<int>     *trig_EF_trigmugirl_EF_mu20i;
   vector<int>     *trig_EF_trigmugirl_EF_mu20i_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu22;
   vector<int>     *trig_EF_trigmugirl_EF_mu22_MG;
   vector<int>     *trig_EF_trigmugirl_EF_mu22_MG_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu22_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu24_MG_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu24_MG_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu24_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu24_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu30_MG_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu30_MG_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu30_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu30_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu4;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_MSonly;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_MSonly_barrel;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_MSonly_barrel_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_MSonly_empty;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_MSonly_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_MSonly_tight_L1MU11;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_MSonly_tighter;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_slow;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_slow_empty;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_slow_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_slow_outOfTime;
   vector<int>     *trig_EF_trigmugirl_EF_mu40_slow_outOfTime_medium;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Bmumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_BmumuX;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu_FS_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu_MG;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu_MG_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu_SiTrk;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu_SiTrk_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_DiMu_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Jpsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Jpsimumu_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Jpsimumu_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1J10_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1J15_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1J20_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1J30_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1J50_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1J75_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1MU11_MSonly_cosmic;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_L1MU11_cosmic;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_MSonly_cosmic;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Trk_Jpsi;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Trk_Jpsi_loose;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Trk_Jpsi_looser;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Trk_Upsi_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Upsimumu_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Upsimumu_SiTrk_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_Upsimumu_tight_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_cosmic;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_firstempty;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_j10_a4tc_EFFS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_j10_a4tc_EFFS_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_j135_a4tc_EFFS_L1matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_j180_a4tc_EFFS_L1matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_j20_a4tc_EFFS_matched;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_j30;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_j45_a4tc_EFFS_xe45_loose_noMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_mu6;
   vector<int>     *trig_EF_trigmugirl_EF_mu4_unpaired_iso;
   vector<int>     *trig_EF_trigmugirl_EF_mu4imu6i_DiMu_DY;
   vector<int>     *trig_EF_trigmugirl_EF_mu4imu6i_DiMu_DY14_noVtx_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4imu6i_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_Bmumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_Bmumux;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_DiMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_DiMu_DY;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_DiMu_DY20;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_DiMu_DY_noVtx_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_DiMu_noVtx_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_Jpsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_Jpsimumu_IDTrkNoCut;
   vector<int>     *trig_EF_trigmugirl_EF_mu4mu6_Upsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu6;
   vector<int>     *trig_EF_trigmugirl_EF_mu60_MSonly;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_Bmumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_BmumuX;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_DiMu;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_DiMu_noOS;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_Jpsimumu;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_Jpsimumu_SiTrk;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_Jpsimumu_tight;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_Trk_Jpsi_loose;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_Upsimumu_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu6_Upsimumu_tight_FS;
   vector<int>     *trig_EF_trigmugirl_EF_mu80_MSonly;
   vector<int>     *trig_L2_jet_L2_2fj25;
   vector<int>     *trig_L2_jet_L2_2fj50;
   vector<int>     *trig_L2_jet_L2_2fj70;
   vector<int>     *trig_L2_jet_L2_2j25_j70_dphi03;
   vector<int>     *trig_L2_jet_L2_2j40_anymct100_xe20_medium_noMu;
   vector<int>     *trig_L2_jet_L2_2j50_anymct100_xe20_medium_noMu;
   vector<int>     *trig_L2_jet_L2_3j25;
   vector<int>     *trig_L2_jet_L2_3j35;
   vector<int>     *trig_L2_jet_L2_3j40;
   vector<int>     *trig_L2_jet_L2_3j70;
   vector<int>     *trig_L2_jet_L2_3j95;
   vector<int>     *trig_L2_jet_L2_4j25;
   vector<int>     *trig_L2_jet_L2_4j35;
   vector<int>     *trig_L2_jet_L2_4j40;
   vector<int>     *trig_L2_jet_L2_4j50;
   vector<int>     *trig_L2_jet_L2_5j25;
   vector<int>     *trig_L2_jet_L2_5j35;
   vector<int>     *trig_L2_jet_L2_5j40;
   vector<int>     *trig_L2_jet_L2_6j25;
   vector<int>     *trig_L2_jet_L2_fj10_empty_larcalib;
   vector<int>     *trig_L2_jet_L2_fj25;
   vector<int>     *trig_L2_jet_L2_fj25_l2cleanph;
   vector<int>     *trig_L2_jet_L2_fj25_larcalib;
   vector<int>     *trig_L2_jet_L2_fj50;
   vector<int>     *trig_L2_jet_L2_fj50_larcalib;
   vector<int>     *trig_L2_jet_L2_fj70;
   vector<int>     *trig_L2_jet_L2_fj95;
   vector<int>     *trig_L2_jet_L2_j10_empty_larcalib;
   vector<int>     *trig_L2_jet_L2_j25;
   vector<int>     *trig_L2_jet_L2_j25_cosmic;
   vector<int>     *trig_L2_jet_L2_j25_firstempty;
   vector<int>     *trig_L2_jet_L2_j25_fj25;
   vector<int>     *trig_L2_jet_L2_j25_l2cleanph;
   vector<int>     *trig_L2_jet_L2_j25_larcalib;
   vector<int>     *trig_L2_jet_L2_j30;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HV;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HV_cosmic;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HV_firstempty;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HV_unpaired_iso;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HV_unpaired_noniso;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HVtrk;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HVtrk_LOF;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HVtrk_cosmic;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HVtrk_firstempty;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HVtrk_unpaired_iso;
   vector<int>     *trig_L2_jet_L2_j30_L1TAU_HVtrk_unpaired_noniso;
   vector<int>     *trig_L2_jet_L2_j30_Trackless_HV;
   vector<int>     *trig_L2_jet_L2_j30_Trackless_HV_L1MU10;
   vector<int>     *trig_L2_jet_L2_j30_Trackless_HV_cosmic;
   vector<int>     *trig_L2_jet_L2_j30_Trackless_HV_firstempty;
   vector<int>     *trig_L2_jet_L2_j30_Trackless_HV_unpaired_iso;
   vector<int>     *trig_L2_jet_L2_j30_Trackless_HV_unpaired_noniso;
   vector<int>     *trig_L2_jet_L2_j30_eta13_a4tc_EFFS_EFxe30_noMu_empty;
   vector<int>     *trig_L2_jet_L2_j30_eta13_a4tc_EFFS_EFxe30_noMu_firstempty;
   vector<int>     *trig_L2_jet_L2_j35;
   vector<int>     *trig_L2_jet_L2_j35_fj35;
   vector<int>     *trig_L2_jet_L2_j40;
   vector<int>     *trig_L2_jet_L2_j45;
   vector<int>     *trig_L2_jet_L2_j45_cosmic;
   vector<int>     *trig_L2_jet_L2_j45_firstempty;
   vector<int>     *trig_L2_jet_L2_j50;
   vector<int>     *trig_L2_jet_L2_j50_eta13_a4tc_EFFS_EFxe50_noMu_empty;
   vector<int>     *trig_L2_jet_L2_j50_eta13_a4tc_EFFS_EFxe50_noMu_firstempty;
   vector<int>     *trig_L2_jet_L2_j50_eta25_a4tc_EFFS_EFxe50_noMu_empty;
   vector<int>     *trig_L2_jet_L2_j50_eta25_a4tc_EFFS_EFxe50_noMu_firstempty;
   vector<int>     *trig_L2_jet_L2_j50_fj50;
   vector<int>     *trig_L2_jet_L2_j50_larcalib;
   vector<int>     *trig_L2_jet_L2_j50_xe35_medium_noMu;
   vector<int>     *trig_L2_jet_L2_j50_xe35_medium_noMu_l2cleancons;
   vector<int>     *trig_L2_jet_L2_j60_xe45_noMu;
   vector<int>     *trig_L2_jet_L2_j70;
   vector<int>     *trig_L2_jet_L2_j70_2j25;
   vector<int>     *trig_L2_jet_L2_j70_2j25_L2anymct100;
   vector<int>     *trig_L2_jet_L2_j70_2j25_anymct100;
   vector<int>     *trig_L2_jet_L2_j70_2j25_anymct150;
   vector<int>     *trig_L2_jet_L2_j70_2j25_anymct175;
   vector<int>     *trig_L2_jet_L2_j70_2j25_dphi04;
   vector<int>     *trig_L2_jet_L2_j70_3j25;
   vector<int>     *trig_L2_jet_L2_j70_fj70;
   vector<int>     *trig_L2_jet_L2_j70_xe20_loose_noMu;
   vector<int>     *trig_L2_jet_L2_j70_xe25_loose_noMu;
   vector<int>     *trig_L2_jet_L2_j70_xe35_noMu;
   vector<int>     *trig_L2_jet_L2_j70_xe35_noMu_l2cleancons;
   vector<int>     *trig_L2_jet_L2_j70_xs25_noMu;
   vector<int>     *trig_L2_jet_L2_j75_xe40_noMu;
   vector<int>     *trig_L2_jet_L2_j95;
   vector<int>     *trig_L2_jet_L2_j95_2j25_dphi04;
   vector<int>     *trig_L2_jet_L2_j95_l2cleanph;
   vector<int>     *trig_L2_jet_L2_j95_larcalib;
   vector<int>     *trig_L2_jet_L2_je195;
   vector<int>     *trig_L2_jet_L2_je255;
   vector<int>     *trig_L2_jet_L2_je300;
   vector<int>     *trig_L2_jet_L2_je350;
   vector<int>     *trig_L2_jet_L2_je420;
   vector<int>     *trig_L2_jet_L2_je500;
   vector<int>     *trig_EF_jet_EF_2fj100_a4tc_EFFS_deta50_FB;
   vector<int>     *trig_EF_jet_EF_2fj30_a4tc_EFFS_deta50_FB;
   vector<int>     *trig_EF_jet_EF_2fj30_a4tc_EFFS_deta50_FC;
   vector<int>     *trig_EF_jet_EF_2fj55_a4tc_EFFS_deta50_FB;
   vector<int>     *trig_EF_jet_EF_2fj55_a4tc_EFFS_deta50_FC;
   vector<int>     *trig_EF_jet_EF_2fj75_a4tc_EFFS_deta50_FB;
   vector<int>     *trig_EF_jet_EF_2fj75_a4tc_EFFS_deta50_FC;
   vector<int>     *trig_EF_jet_EF_2j100_a4tc_EFFS_deta35_FC;
   vector<int>     *trig_EF_jet_EF_2j135_a4tc_EFFS_deta35_FC;
   vector<int>     *trig_EF_jet_EF_2j180_a4tc_EFFS_deta35_FC;
   vector<int>     *trig_EF_jet_EF_2j240_a4tc_EFFS_deta35_FC;
   vector<int>     *trig_EF_jet_EF_2j45_a4tc_EFFS_leadingmct100_xe40_medium_noMu;
   vector<int>     *trig_EF_jet_EF_2j55_a4tc_EFFS_leadingmct100_xe40_medium_noMu;
   vector<int>     *trig_EF_jet_EF_3j100_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_3j100_a4tc_EFFS_L1J75;
   vector<int>     *trig_EF_jet_EF_3j30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_3j40_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_3j45_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_3j75_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_4j30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_4j40_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_4j40_a4tc_EFFS_ht350;
   vector<int>     *trig_EF_jet_EF_4j40_a4tc_EFFS_ht400;
   vector<int>     *trig_EF_jet_EF_4j45_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_4j55_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_4j60_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_5j30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_5j40_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_5j45_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_6j30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_6j30_a4tc_EFFS_L15J10;
   vector<int>     *trig_EF_jet_EF_6j40_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_6j45_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_7j30_a4tc_EFFS_L15J10;
   vector<int>     *trig_EF_jet_EF_7j30_a4tc_EFFS_L16J10;
   vector<int>     *trig_EF_jet_EF_fj100_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_fj10_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_fj10_a4tc_EFFS_1vx;
   vector<int>     *trig_EF_jet_EF_fj135_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_fj15_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_fj20_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_fj30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_fj30_a4tc_EFFS_l2cleanph;
   vector<int>     *trig_EF_jet_EF_fj55_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_fj75_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j100_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j100_a4tc_EFFS_ht350;
   vector<int>     *trig_EF_jet_EF_j100_a4tc_EFFS_ht400;
   vector<int>     *trig_EF_jet_EF_j100_a4tc_EFFS_ht500;
   vector<int>     *trig_EF_jet_EF_j100_j30_a4tc_EFFS_L2dphi04;
   vector<int>     *trig_EF_jet_EF_j10_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j10_a4tc_EFFS_1vx;
   vector<int>     *trig_EF_jet_EF_j135_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j135_a4tc_EFFS_ht500;
   vector<int>     *trig_EF_jet_EF_j135_j30_a4tc_EFFS_L2dphi04;
   vector<int>     *trig_EF_jet_EF_j135_j30_a4tc_EFFS_dphi04;
   vector<int>     *trig_EF_jet_EF_j15_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j180_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j180_j30_a4tc_EFFS_dphi04;
   vector<int>     *trig_EF_jet_EF_j20_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j240_a10tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j240_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j240_a4tc_EFFS_l2cleanph;
   vector<int>     *trig_EF_jet_EF_j30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j30_a4tc_EFFS_l2cleanph;
   vector<int>     *trig_EF_jet_EF_j30_cosmic;
   vector<int>     *trig_EF_jet_EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_empty;
   vector<int>     *trig_EF_jet_EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_firstempty;
   vector<int>     *trig_EF_jet_EF_j30_firstempty;
   vector<int>     *trig_EF_jet_EF_j30_fj30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j320_a10tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j320_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_cosmic;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_firstempty;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_iso;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_noniso;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_LOF;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_cosmic;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_firstempty;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_iso;
   vector<int>     *trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_noniso;
   vector<int>     *trig_EF_jet_EF_j40_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j40_fj40_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j425_a10tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j425_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j45_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j50_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j50_cosmic;
   vector<int>     *trig_EF_jet_EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_empty;
   vector<int>     *trig_EF_jet_EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_firstempty;
   vector<int>     *trig_EF_jet_EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_empty;
   vector<int>     *trig_EF_jet_EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_firstempty;
   vector<int>     *trig_EF_jet_EF_j50_firstempty;
   vector<int>     *trig_EF_jet_EF_j55_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10;
   vector<int>     *trig_EF_jet_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10_l2cleancons;
   vector<int>     *trig_EF_jet_EF_j55_fj55_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j65_a4tc_EFFS_xe65_noMu_dphi2j30xe10;
   vector<int>     *trig_EF_jet_EF_j70_j25_dphi03_NoEF;
   vector<int>     *trig_EF_jet_EF_j75_2j30_a4tc_EFFS_ht350;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS_xe40_loose_noMu;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS_xe45_loose_noMu;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS_xe55_loose_noMu;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS_xe55_noMu;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS_xe55_noMu_l2cleancons;
   vector<int>     *trig_EF_jet_EF_j75_a4tc_EFFS_xs35_noMu;
   vector<int>     *trig_EF_jet_EF_j75_fj75_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2anymct100;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2anymct150;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2anymct175;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2dphi04;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS_anymct150;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS_anymct175;
   vector<int>     *trig_EF_jet_EF_j75_j30_a4tc_EFFS_leadingmct150;
   vector<int>     *trig_EF_jet_EF_j80_a4tc_EFFS_xe60_noMu;
   vector<int>     *trig_EF_jet_EF_je195_NoEF;
   vector<int>     *trig_EF_jet_EF_je255_NoEF;
   vector<int>     *trig_EF_jet_EF_je300_NoEF;
   vector<int>     *trig_EF_jet_EF_je350_NoEF;
   vector<int>     *trig_EF_jet_EF_je420_NoEF;
   vector<int>     *trig_EF_jet_EF_je500_NoEF;
   vector<string>  *SkimDecision_NTPSUSY_name;
   vector<int>     *SkimDecision_NTPSUSY_accepted;
   vector<string>  *SkimDecision_DAODJETS_name;
   vector<int>     *SkimDecision_DAODJETS_accepted;
   vector<string>  *SkimDecision_DAODMUONS_name;
   vector<int>     *SkimDecision_DAODMUONS_accepted;
   vector<string>  *SkimDecision_DAODEGAMMA_name;
   vector<int>     *SkimDecision_DAODEGAMMA_accepted;
   UInt_t          RunNumber;
   UInt_t          EventNumber;
   UInt_t          timestamp;
   UInt_t          timestamp_ns;
   UInt_t          lbn;
   UInt_t          bcid;
   UInt_t          detmask0;
   UInt_t          detmask1;
   Float_t         actualIntPerXing;
   Float_t         averageIntPerXing;
   UInt_t          mc_channel_number;
   UInt_t          mc_event_number;
   Float_t         mc_event_weight;
   UInt_t          pixelFlags;
   UInt_t          sctFlags;
   UInt_t          trtFlags;
   UInt_t          larFlags;
   UInt_t          tileFlags;
   UInt_t          muonFlags;
   UInt_t          fwdFlags;
   UInt_t          coreFlags;
   UInt_t          pixelError;
   UInt_t          sctError;
   UInt_t          trtError;
   UInt_t          larError;
   UInt_t          tileError;
   UInt_t          muonError;
   UInt_t          fwdError;
   UInt_t          coreError;
   Bool_t          isSimulation;
   Bool_t          isCalibration;
   Bool_t          isTestBeam;
   Int_t           bb_decision;
   Int_t           bb_numSegment;
   Int_t           bb_numSegmentEarly;
   Int_t           bb_numSegmentAC;
   Int_t           bb_numMatched;
   Int_t           bb_numNoTimeLoose;
   Int_t           bb_numNoTimeMedium;
   Int_t           bb_numNoTimeTight;
   Int_t           bb_numOneSidedLoose;
   Int_t           bb_numOneSidedMedium;
   Int_t           bb_numOneSidedTight;
   Int_t           bb_numTwoSided;
   Int_t           bb_numClusterShape;
   Int_t           bb_cl_n;
   vector<int>     *bb_cl_bbDecision;
   vector<float>   *bb_cl_drdz;
   vector<float>   *bb_cl_pt;
   vector<float>   *bb_cl_eta;
   vector<float>   *bb_cl_phi;
   vector<float>   *bb_cl_E_em;
   vector<float>   *bb_cl_E_had;
   vector<float>   *bb_cl_firstEdens;
   vector<float>   *bb_cl_cellmaxfrac;
   vector<float>   *bb_cl_longitudinal;
   vector<float>   *bb_cl_secondlambda;
   vector<float>   *bb_cl_lateral;
   vector<float>   *bb_cl_secondR;
   vector<float>   *bb_cl_centerlambda;
   vector<float>   *bb_cl_centermag;
   vector<float>   *bb_cl_deltaTheta;
   vector<float>   *bb_cl_deltaPhi;
   vector<float>   *bb_cl_time;
   Int_t           bb_jet_n;
   Int_t           el_n;
   vector<float>   *el_E;
   vector<float>   *el_Et;
   vector<float>   *el_pt;
   vector<float>   *el_m;
   vector<float>   *el_eta;
   vector<float>   *el_phi;
   vector<float>   *el_px;
   vector<float>   *el_py;
   vector<float>   *el_pz;
   vector<float>   *el_charge;
   vector<int>     *el_author;
   vector<unsigned int> *el_isEM;
   vector<unsigned int> *el_isEMLoose;
   vector<unsigned int> *el_isEMMedium;
   vector<unsigned int> *el_isEMTight;
   vector<unsigned int> *el_OQ;
   vector<int>     *el_convFlag;
   vector<int>     *el_isConv;
   vector<int>     *el_nConv;
   vector<int>     *el_nSingleTrackConv;
   vector<int>     *el_nDoubleTrackConv;
   vector<unsigned int> *el_OQRecalc;
   vector<int>     *el_type;
   vector<int>     *el_origin;
   vector<int>     *el_typebkg;
   vector<int>     *el_originbkg;
   vector<float>   *el_truth_E;
   vector<float>   *el_truth_pt;
   vector<float>   *el_truth_eta;
   vector<float>   *el_truth_phi;
   vector<int>     *el_truth_type;
   vector<int>     *el_truth_status;
   vector<int>     *el_truth_barcode;
   vector<int>     *el_truth_mothertype;
   vector<int>     *el_truth_motherbarcode;
   vector<int>     *el_truth_hasHardBrem;
   vector<int>     *el_truth_index;
   vector<int>     *el_truth_matched;
   vector<int>     *el_mediumWithoutTrack;
   vector<int>     *el_mediumIsoWithoutTrack;
   vector<int>     *el_tightWithoutTrack;
   vector<int>     *el_tightIsoWithoutTrack;
   vector<int>     *el_loose;
   vector<int>     *el_looseIso;
   vector<int>     *el_medium;
   vector<int>     *el_mediumIso;
   vector<int>     *el_tight;
   vector<int>     *el_tightIso;
   vector<int>     *el_loosePP;
   vector<int>     *el_loosePPIso;
   vector<int>     *el_mediumPP;
   vector<int>     *el_mediumPPIso;
   vector<int>     *el_tightPP;
   vector<int>     *el_tightPPIso;
   vector<int>     *el_goodOQ;
   vector<float>   *el_Ethad;
   vector<float>   *el_Ethad1;
   vector<float>   *el_f1;
   vector<float>   *el_f1core;
   vector<float>   *el_Emins1;
   vector<float>   *el_fside;
   vector<float>   *el_Emax2;
   vector<float>   *el_ws3;
   vector<float>   *el_wstot;
   vector<float>   *el_emaxs1;
   vector<float>   *el_deltaEs;
   vector<float>   *el_E233;
   vector<float>   *el_E237;
   vector<float>   *el_E277;
   vector<float>   *el_weta2;
   vector<float>   *el_f3;
   vector<float>   *el_f3core;
   vector<float>   *el_rphiallcalo;
   vector<float>   *el_Etcone45;
   vector<float>   *el_Etcone15;
   vector<float>   *el_Etcone20;
   vector<float>   *el_Etcone25;
   vector<float>   *el_Etcone30;
   vector<float>   *el_Etcone35;
   vector<float>   *el_Etcone40;
   vector<float>   *el_ptcone20;
   vector<float>   *el_ptcone30;
   vector<float>   *el_ptcone40;
   vector<float>   *el_nucone20;
   vector<float>   *el_nucone30;
   vector<float>   *el_nucone40;
   vector<float>   *el_convanglematch;
   vector<float>   *el_convtrackmatch;
   vector<int>     *el_hasconv;
   vector<float>   *el_convvtxx;
   vector<float>   *el_convvtxy;
   vector<float>   *el_convvtxz;
   vector<float>   *el_Rconv;
   vector<float>   *el_zconv;
   vector<float>   *el_convvtxchi2;
   vector<float>   *el_pt1conv;
   vector<int>     *el_convtrk1nBLHits;
   vector<int>     *el_convtrk1nPixHits;
   vector<int>     *el_convtrk1nSCTHits;
   vector<int>     *el_convtrk1nTRTHits;
   vector<float>   *el_pt2conv;
   vector<int>     *el_convtrk2nBLHits;
   vector<int>     *el_convtrk2nPixHits;
   vector<int>     *el_convtrk2nSCTHits;
   vector<int>     *el_convtrk2nTRTHits;
   vector<float>   *el_ptconv;
   vector<float>   *el_pzconv;
   vector<float>   *el_pos7;
   vector<float>   *el_etacorrmag;
   vector<float>   *el_deltaeta1;
   vector<float>   *el_deltaeta2;
   vector<float>   *el_deltaphi2;
   vector<float>   *el_deltaphiRescaled;
   vector<float>   *el_deltaPhiRot;
   vector<float>   *el_expectHitInBLayer;
   vector<float>   *el_trackd0_physics;
   vector<float>   *el_etaSampling1;
   vector<float>   *el_reta;
   vector<float>   *el_rphi;
   vector<float>   *el_zvertex;
   vector<float>   *el_errz;
   vector<float>   *el_etap;
   vector<float>   *el_depth;
   vector<int>     *el_refittedTrack_n;
   vector<float>   *el_Es0;
   vector<float>   *el_etas0;
   vector<float>   *el_phis0;
   vector<float>   *el_Es1;
   vector<float>   *el_etas1;
   vector<float>   *el_phis1;
   vector<float>   *el_Es2;
   vector<float>   *el_etas2;
   vector<float>   *el_phis2;
   vector<float>   *el_Es3;
   vector<float>   *el_etas3;
   vector<float>   *el_phis3;
   vector<float>   *el_cl_E;
   vector<float>   *el_cl_pt;
   vector<float>   *el_cl_eta;
   vector<float>   *el_cl_phi;
   vector<float>   *el_firstEdens;
   vector<float>   *el_cellmaxfrac;
   vector<float>   *el_longitudinal;
   vector<float>   *el_secondlambda;
   vector<float>   *el_lateral;
   vector<float>   *el_secondR;
   vector<float>   *el_centerlambda;
   vector<float>   *el_cl_time;
   vector<float>   *el_trackd0;
   vector<float>   *el_trackz0;
   vector<float>   *el_trackphi;
   vector<float>   *el_tracktheta;
   vector<float>   *el_trackqoverp;
   vector<float>   *el_trackpt;
   vector<float>   *el_tracketa;
   vector<int>     *el_nBLHits;
   vector<int>     *el_nPixHits;
   vector<int>     *el_nSCTHits;
   vector<int>     *el_nTRTHits;
   vector<int>     *el_nTRTHighTHits;
   vector<int>     *el_nPixHoles;
   vector<int>     *el_nSCTHoles;
   vector<int>     *el_nTRTHoles;
   vector<int>     *el_nBLSharedHits;
   vector<int>     *el_nPixSharedHits;
   vector<int>     *el_nSCTSharedHits;
   vector<int>     *el_nBLayerOutliers;
   vector<int>     *el_nPixelOutliers;
   vector<int>     *el_nSCTOutliers;
   vector<int>     *el_nTRTOutliers;
   vector<int>     *el_nTRTHighTOutliers;
   vector<int>     *el_expectBLayerHit;
   vector<int>     *el_nSiHits;
   vector<float>   *el_TRTHighTHitsRatio;
   vector<float>   *el_TRTHighTOutliersRatio;
   vector<float>   *el_pixeldEdx;
   vector<int>     *el_nGoodHitsPixeldEdx;
   vector<float>   *el_massPixeldEdx;
   vector<vector<float> > *el_likelihoodsPixeldEdx;
   vector<float>   *el_vertweight;
   vector<int>     *el_hastrack;
   vector<float>   *el_Etcone40_ED_corrected;
   vector<float>   *el_Etcone40_corrected;
   vector<float>   *el_EF_dr;
   vector<int>     *el_EF_index;
   Int_t           ph_n;
   vector<float>   *ph_E;
   vector<float>   *ph_Et;
   vector<float>   *ph_pt;
   vector<float>   *ph_m;
   vector<float>   *ph_eta;
   vector<float>   *ph_phi;
   vector<float>   *ph_px;
   vector<float>   *ph_py;
   vector<float>   *ph_pz;
   vector<int>     *ph_author;
   vector<int>     *ph_isRecovered;
   vector<unsigned int> *ph_isEM;
   vector<unsigned int> *ph_isEMLoose;
   vector<unsigned int> *ph_isEMMedium;
   vector<unsigned int> *ph_isEMTight;
   vector<unsigned int> *ph_OQ;
   vector<unsigned int> *ph_OQRecalc;
   vector<int>     *ph_convFlag;
   vector<int>     *ph_isConv;
   vector<int>     *ph_nConv;
   vector<int>     *ph_nSingleTrackConv;
   vector<int>     *ph_nDoubleTrackConv;
   vector<int>     *ph_type;
   vector<int>     *ph_origin;
   vector<float>   *ph_truth_deltaRRecPhoton;
   vector<float>   *ph_truth_E;
   vector<float>   *ph_truth_pt;
   vector<float>   *ph_truth_eta;
   vector<float>   *ph_truth_phi;
   vector<int>     *ph_truth_type;
   vector<int>     *ph_truth_status;
   vector<int>     *ph_truth_barcode;
   vector<int>     *ph_truth_mothertype;
   vector<int>     *ph_truth_motherbarcode;
   vector<int>     *ph_truth_index;
   vector<int>     *ph_truth_matched;
   vector<int>     *ph_loose;
   vector<int>     *ph_looseIso;
   vector<int>     *ph_tight;
   vector<int>     *ph_tightIso;
   vector<int>     *ph_looseAR;
   vector<int>     *ph_looseARIso;
   vector<int>     *ph_tightAR;
   vector<int>     *ph_tightARIso;
   vector<int>     *ph_goodOQ;
   vector<float>   *ph_Ethad;
   vector<float>   *ph_Ethad1;
   vector<float>   *ph_E033;
   vector<float>   *ph_f1;
   vector<float>   *ph_f1core;
   vector<float>   *ph_Emins1;
   vector<float>   *ph_fside;
   vector<float>   *ph_Emax2;
   vector<float>   *ph_ws3;
   vector<float>   *ph_wstot;
   vector<float>   *ph_E132;
   vector<float>   *ph_E1152;
   vector<float>   *ph_emaxs1;
   vector<float>   *ph_deltaEs;
   vector<float>   *ph_E233;
   vector<float>   *ph_E237;
   vector<float>   *ph_E277;
   vector<float>   *ph_weta2;
   vector<float>   *ph_f3;
   vector<float>   *ph_f3core;
   vector<float>   *ph_rphiallcalo;
   vector<float>   *ph_Etcone45;
   vector<float>   *ph_Etcone15;
   vector<float>   *ph_Etcone20;
   vector<float>   *ph_Etcone25;
   vector<float>   *ph_Etcone30;
   vector<float>   *ph_Etcone35;
   vector<float>   *ph_Etcone40;
   vector<float>   *ph_ptcone20;
   vector<float>   *ph_ptcone30;
   vector<float>   *ph_ptcone40;
   vector<float>   *ph_nucone20;
   vector<float>   *ph_nucone30;
   vector<float>   *ph_nucone40;
   vector<float>   *ph_convanglematch;
   vector<float>   *ph_convtrackmatch;
   vector<int>     *ph_hasconv;
   vector<float>   *ph_convvtxx;
   vector<float>   *ph_convvtxy;
   vector<float>   *ph_convvtxz;
   vector<float>   *ph_Rconv;
   vector<float>   *ph_zconv;
   vector<float>   *ph_convvtxchi2;
   vector<float>   *ph_pt1conv;
   vector<int>     *ph_convtrk1nBLHits;
   vector<int>     *ph_convtrk1nPixHits;
   vector<int>     *ph_convtrk1nSCTHits;
   vector<int>     *ph_convtrk1nTRTHits;
   vector<float>   *ph_pt2conv;
   vector<int>     *ph_convtrk2nBLHits;
   vector<int>     *ph_convtrk2nPixHits;
   vector<int>     *ph_convtrk2nSCTHits;
   vector<int>     *ph_convtrk2nTRTHits;
   vector<float>   *ph_ptconv;
   vector<float>   *ph_pzconv;
   vector<float>   *ph_reta;
   vector<float>   *ph_rphi;
   vector<float>   *ph_zvertex;
   vector<float>   *ph_errz;
   vector<float>   *ph_etap;
   vector<float>   *ph_depth;
   vector<float>   *ph_cl_E;
   vector<float>   *ph_cl_pt;
   vector<float>   *ph_cl_eta;
   vector<float>   *ph_cl_phi;
   vector<float>   *ph_Es0;
   vector<float>   *ph_etas0;
   vector<float>   *ph_phis0;
   vector<float>   *ph_Es1;
   vector<float>   *ph_etas1;
   vector<float>   *ph_phis1;
   vector<float>   *ph_Es2;
   vector<float>   *ph_etas2;
   vector<float>   *ph_phis2;
   vector<float>   *ph_Es3;
   vector<float>   *ph_etas3;
   vector<float>   *ph_phis3;
   vector<float>   *ph_cl_time;
   vector<int>     *ph_truth_isConv;
   vector<int>     *ph_truth_isBrem;
   vector<int>     *ph_truth_isFromHardProc;
   vector<int>     *ph_truth_isPhotonFromHardProc;
   vector<float>   *ph_truth_Rconv;
   vector<float>   *ph_truth_zconv;
   vector<float>   *ph_deltaEmax2;
   vector<float>   *ph_jet_dr;
   vector<int>     *ph_jet_matched;
   vector<float>   *ph_convIP;
   vector<float>   *ph_convIPRev;
   vector<float>   *ph_ptIsolationCone;
   vector<float>   *ph_ptIsolationConePhAngle;
   vector<float>   *ph_Etcone40_ED_corrected;
   vector<float>   *ph_Etcone40_corrected;
   Int_t           mu_staco_n;
   vector<float>   *mu_staco_E;
   vector<float>   *mu_staco_pt;
   vector<float>   *mu_staco_m;
   vector<float>   *mu_staco_eta;
   vector<float>   *mu_staco_phi;
   vector<float>   *mu_staco_px;
   vector<float>   *mu_staco_py;
   vector<float>   *mu_staco_pz;
   vector<float>   *mu_staco_charge;
   vector<unsigned short> *mu_staco_allauthor;
   vector<int>     *mu_staco_author;
   vector<float>   *mu_staco_matchchi2;
   vector<int>     *mu_staco_matchndof;
   vector<float>   *mu_staco_etcone20;
   vector<float>   *mu_staco_etcone30;
   vector<float>   *mu_staco_etcone40;
   vector<float>   *mu_staco_nucone20;
   vector<float>   *mu_staco_nucone30;
   vector<float>   *mu_staco_nucone40;
   vector<float>   *mu_staco_ptcone20;
   vector<float>   *mu_staco_ptcone30;
   vector<float>   *mu_staco_ptcone40;
   vector<float>   *mu_staco_energyLossPar;
   vector<float>   *mu_staco_energyLossErr;
   vector<float>   *mu_staco_etCore;
   vector<float>   *mu_staco_energyLossType;
   vector<unsigned short> *mu_staco_caloMuonIdTag;
   vector<double>  *mu_staco_caloLRLikelihood;
   vector<int>     *mu_staco_bestMatch;
   vector<int>     *mu_staco_isStandAloneMuon;
   vector<int>     *mu_staco_isCombinedMuon;
   vector<int>     *mu_staco_isLowPtReconstructedMuon;
   vector<int>     *mu_staco_isSegmentTaggedMuon;
   vector<int>     *mu_staco_isCaloMuonId;
   vector<int>     *mu_staco_alsoFoundByLowPt;
   vector<int>     *mu_staco_alsoFoundByCaloMuonId;
   vector<int>     *mu_staco_loose;
   vector<int>     *mu_staco_medium;
   vector<int>     *mu_staco_tight;
   vector<float>   *mu_staco_d0_exPV;
   vector<float>   *mu_staco_z0_exPV;
   vector<float>   *mu_staco_phi_exPV;
   vector<float>   *mu_staco_theta_exPV;
   vector<float>   *mu_staco_qoverp_exPV;
   vector<float>   *mu_staco_cb_d0_exPV;
   vector<float>   *mu_staco_cb_z0_exPV;
   vector<float>   *mu_staco_cb_phi_exPV;
   vector<float>   *mu_staco_cb_theta_exPV;
   vector<float>   *mu_staco_cb_qoverp_exPV;
   vector<float>   *mu_staco_id_d0_exPV;
   vector<float>   *mu_staco_id_z0_exPV;
   vector<float>   *mu_staco_id_phi_exPV;
   vector<float>   *mu_staco_id_theta_exPV;
   vector<float>   *mu_staco_id_qoverp_exPV;
   vector<float>   *mu_staco_me_d0_exPV;
   vector<float>   *mu_staco_me_z0_exPV;
   vector<float>   *mu_staco_me_phi_exPV;
   vector<float>   *mu_staco_me_theta_exPV;
   vector<float>   *mu_staco_me_qoverp_exPV;
   vector<float>   *mu_staco_ie_d0_exPV;
   vector<float>   *mu_staco_ie_z0_exPV;
   vector<float>   *mu_staco_ie_phi_exPV;
   vector<float>   *mu_staco_ie_theta_exPV;
   vector<float>   *mu_staco_ie_qoverp_exPV;
   vector<float>   *mu_staco_cov_d0_exPV;
   vector<float>   *mu_staco_cov_z0_exPV;
   vector<float>   *mu_staco_cov_phi_exPV;
   vector<float>   *mu_staco_cov_theta_exPV;
   vector<float>   *mu_staco_cov_qoverp_exPV;
   vector<float>   *mu_staco_cov_d0_z0_exPV;
   vector<float>   *mu_staco_cov_d0_phi_exPV;
   vector<float>   *mu_staco_cov_d0_theta_exPV;
   vector<float>   *mu_staco_cov_d0_qoverp_exPV;
   vector<float>   *mu_staco_cov_z0_phi_exPV;
   vector<float>   *mu_staco_cov_z0_theta_exPV;
   vector<float>   *mu_staco_cov_z0_qoverp_exPV;
   vector<float>   *mu_staco_cov_phi_theta_exPV;
   vector<float>   *mu_staco_cov_phi_qoverp_exPV;
   vector<float>   *mu_staco_cov_theta_qoverp_exPV;
   vector<float>   *mu_staco_id_cov_d0_exPV;
   vector<float>   *mu_staco_id_cov_z0_exPV;
   vector<float>   *mu_staco_id_cov_phi_exPV;
   vector<float>   *mu_staco_id_cov_theta_exPV;
   vector<float>   *mu_staco_id_cov_qoverp_exPV;
   vector<float>   *mu_staco_id_cov_d0_z0_exPV;
   vector<float>   *mu_staco_id_cov_d0_phi_exPV;
   vector<float>   *mu_staco_id_cov_d0_theta_exPV;
   vector<float>   *mu_staco_id_cov_d0_qoverp_exPV;
   vector<float>   *mu_staco_id_cov_z0_phi_exPV;
   vector<float>   *mu_staco_id_cov_z0_theta_exPV;
   vector<float>   *mu_staco_id_cov_z0_qoverp_exPV;
   vector<float>   *mu_staco_id_cov_phi_theta_exPV;
   vector<float>   *mu_staco_id_cov_phi_qoverp_exPV;
   vector<float>   *mu_staco_id_cov_theta_qoverp_exPV;
   vector<float>   *mu_staco_ms_d0;
   vector<float>   *mu_staco_ms_z0;
   vector<float>   *mu_staco_ms_phi;
   vector<float>   *mu_staco_ms_theta;
   vector<float>   *mu_staco_ms_qoverp;
   vector<float>   *mu_staco_id_d0;
   vector<float>   *mu_staco_id_z0;
   vector<float>   *mu_staco_id_phi;
   vector<float>   *mu_staco_id_theta;
   vector<float>   *mu_staco_id_qoverp;
   vector<float>   *mu_staco_me_d0;
   vector<float>   *mu_staco_me_z0;
   vector<float>   *mu_staco_me_phi;
   vector<float>   *mu_staco_me_theta;
   vector<float>   *mu_staco_me_qoverp;
   vector<float>   *mu_staco_ie_d0;
   vector<float>   *mu_staco_ie_z0;
   vector<float>   *mu_staco_ie_phi;
   vector<float>   *mu_staco_ie_theta;
   vector<float>   *mu_staco_ie_qoverp;
   vector<int>     *mu_staco_nOutliersOnTrack;
   vector<int>     *mu_staco_nBLHits;
   vector<int>     *mu_staco_nPixHits;
   vector<int>     *mu_staco_nSCTHits;
   vector<int>     *mu_staco_nTRTHits;
   vector<int>     *mu_staco_nTRTHighTHits;
   vector<int>     *mu_staco_nBLSharedHits;
   vector<int>     *mu_staco_nPixSharedHits;
   vector<int>     *mu_staco_nPixHoles;
   vector<int>     *mu_staco_nSCTSharedHits;
   vector<int>     *mu_staco_nSCTHoles;
   vector<int>     *mu_staco_nTRTOutliers;
   vector<int>     *mu_staco_nTRTHighTOutliers;
   vector<int>     *mu_staco_nGangedPixels;
   vector<int>     *mu_staco_nPixelDeadSensors;
   vector<int>     *mu_staco_nSCTDeadSensors;
   vector<int>     *mu_staco_nTRTDeadStraws;
   vector<int>     *mu_staco_expectBLayerHit;
   vector<int>     *mu_staco_nMDTHits;
   vector<int>     *mu_staco_nMDTHoles;
   vector<int>     *mu_staco_nCSCEtaHits;
   vector<int>     *mu_staco_nCSCEtaHoles;
   vector<int>     *mu_staco_nCSCPhiHits;
   vector<int>     *mu_staco_nCSCPhiHoles;
   vector<int>     *mu_staco_nRPCEtaHits;
   vector<int>     *mu_staco_nRPCEtaHoles;
   vector<int>     *mu_staco_nRPCPhiHits;
   vector<int>     *mu_staco_nRPCPhiHoles;
   vector<int>     *mu_staco_nTGCEtaHits;
   vector<int>     *mu_staco_nTGCEtaHoles;
   vector<int>     *mu_staco_nTGCPhiHits;
   vector<int>     *mu_staco_nTGCPhiHoles;
   vector<int>     *mu_staco_nMDTBIHits;
   vector<int>     *mu_staco_nMDTBMHits;
   vector<int>     *mu_staco_nMDTBOHits;
   vector<int>     *mu_staco_nMDTBEEHits;
   vector<int>     *mu_staco_nMDTBIS78Hits;
   vector<int>     *mu_staco_nMDTEIHits;
   vector<int>     *mu_staco_nMDTEMHits;
   vector<int>     *mu_staco_nMDTEOHits;
   vector<int>     *mu_staco_nMDTEEHits;
   vector<int>     *mu_staco_nRPCLayer1EtaHits;
   vector<int>     *mu_staco_nRPCLayer2EtaHits;
   vector<int>     *mu_staco_nRPCLayer3EtaHits;
   vector<int>     *mu_staco_nRPCLayer1PhiHits;
   vector<int>     *mu_staco_nRPCLayer2PhiHits;
   vector<int>     *mu_staco_nRPCLayer3PhiHits;
   vector<int>     *mu_staco_nTGCLayer1EtaHits;
   vector<int>     *mu_staco_nTGCLayer2EtaHits;
   vector<int>     *mu_staco_nTGCLayer3EtaHits;
   vector<int>     *mu_staco_nTGCLayer4EtaHits;
   vector<int>     *mu_staco_nTGCLayer1PhiHits;
   vector<int>     *mu_staco_nTGCLayer2PhiHits;
   vector<int>     *mu_staco_nTGCLayer3PhiHits;
   vector<int>     *mu_staco_nTGCLayer4PhiHits;
   vector<int>     *mu_staco_barrelSectors;
   vector<int>     *mu_staco_endcapSectors;
   vector<float>   *mu_staco_trackd0;
   vector<float>   *mu_staco_trackz0;
   vector<float>   *mu_staco_trackphi;
   vector<float>   *mu_staco_tracktheta;
   vector<float>   *mu_staco_trackqoverp;
   vector<float>   *mu_staco_trackcov_d0;
   vector<float>   *mu_staco_trackcov_z0;
   vector<float>   *mu_staco_trackcov_phi;
   vector<float>   *mu_staco_trackcov_theta;
   vector<float>   *mu_staco_trackcov_qoverp;
   vector<float>   *mu_staco_trackcov_d0_z0;
   vector<float>   *mu_staco_trackcov_d0_phi;
   vector<float>   *mu_staco_trackcov_d0_theta;
   vector<float>   *mu_staco_trackcov_d0_qoverp;
   vector<float>   *mu_staco_trackcov_z0_phi;
   vector<float>   *mu_staco_trackcov_z0_theta;
   vector<float>   *mu_staco_trackcov_z0_qoverp;
   vector<float>   *mu_staco_trackcov_phi_theta;
   vector<float>   *mu_staco_trackcov_phi_qoverp;
   vector<float>   *mu_staco_trackcov_theta_qoverp;
   vector<int>     *mu_staco_hastrack;
   vector<float>   *mu_staco_truth_dr;
   vector<float>   *mu_staco_truth_E;
   vector<float>   *mu_staco_truth_pt;
   vector<float>   *mu_staco_truth_eta;
   vector<float>   *mu_staco_truth_phi;
   vector<int>     *mu_staco_truth_type;
   vector<int>     *mu_staco_truth_status;
   vector<int>     *mu_staco_truth_barcode;
   vector<int>     *mu_staco_truth_mothertype;
   vector<int>     *mu_staco_truth_motherbarcode;
   vector<int>     *mu_staco_truth_matched;
   vector<float>   *mu_staco_EFCB_dr;
   vector<int>     *mu_staco_EFCB_index;
   vector<float>   *mu_staco_EFMG_dr;
   vector<int>     *mu_staco_EFMG_index;
   vector<float>   *mu_staco_EFME_dr;
   vector<int>     *mu_staco_EFME_index;
   Int_t           mu_muid_n;
   vector<float>   *mu_muid_E;
   vector<float>   *mu_muid_pt;
   vector<float>   *mu_muid_m;
   vector<float>   *mu_muid_eta;
   vector<float>   *mu_muid_phi;
   vector<float>   *mu_muid_px;
   vector<float>   *mu_muid_py;
   vector<float>   *mu_muid_pz;
   vector<float>   *mu_muid_charge;
   vector<unsigned short> *mu_muid_allauthor;
   vector<int>     *mu_muid_author;
   vector<float>   *mu_muid_matchchi2;
   vector<int>     *mu_muid_matchndof;
   vector<float>   *mu_muid_etcone20;
   vector<float>   *mu_muid_etcone30;
   vector<float>   *mu_muid_etcone40;
   vector<float>   *mu_muid_nucone20;
   vector<float>   *mu_muid_nucone30;
   vector<float>   *mu_muid_nucone40;
   vector<float>   *mu_muid_ptcone20;
   vector<float>   *mu_muid_ptcone30;
   vector<float>   *mu_muid_ptcone40;
   vector<float>   *mu_muid_energyLossPar;
   vector<float>   *mu_muid_energyLossErr;
   vector<float>   *mu_muid_etCore;
   vector<float>   *mu_muid_energyLossType;
   vector<unsigned short> *mu_muid_caloMuonIdTag;
   vector<double>  *mu_muid_caloLRLikelihood;
   vector<int>     *mu_muid_bestMatch;
   vector<int>     *mu_muid_isStandAloneMuon;
   vector<int>     *mu_muid_isCombinedMuon;
   vector<int>     *mu_muid_isLowPtReconstructedMuon;
   vector<int>     *mu_muid_isSegmentTaggedMuon;
   vector<int>     *mu_muid_isCaloMuonId;
   vector<int>     *mu_muid_alsoFoundByLowPt;
   vector<int>     *mu_muid_alsoFoundByCaloMuonId;
   vector<int>     *mu_muid_loose;
   vector<int>     *mu_muid_medium;
   vector<int>     *mu_muid_tight;
   vector<float>   *mu_muid_d0_exPV;
   vector<float>   *mu_muid_z0_exPV;
   vector<float>   *mu_muid_phi_exPV;
   vector<float>   *mu_muid_theta_exPV;
   vector<float>   *mu_muid_qoverp_exPV;
   vector<float>   *mu_muid_cb_d0_exPV;
   vector<float>   *mu_muid_cb_z0_exPV;
   vector<float>   *mu_muid_cb_phi_exPV;
   vector<float>   *mu_muid_cb_theta_exPV;
   vector<float>   *mu_muid_cb_qoverp_exPV;
   vector<float>   *mu_muid_id_d0_exPV;
   vector<float>   *mu_muid_id_z0_exPV;
   vector<float>   *mu_muid_id_phi_exPV;
   vector<float>   *mu_muid_id_theta_exPV;
   vector<float>   *mu_muid_id_qoverp_exPV;
   vector<float>   *mu_muid_me_d0_exPV;
   vector<float>   *mu_muid_me_z0_exPV;
   vector<float>   *mu_muid_me_phi_exPV;
   vector<float>   *mu_muid_me_theta_exPV;
   vector<float>   *mu_muid_me_qoverp_exPV;
   vector<float>   *mu_muid_ie_d0_exPV;
   vector<float>   *mu_muid_ie_z0_exPV;
   vector<float>   *mu_muid_ie_phi_exPV;
   vector<float>   *mu_muid_ie_theta_exPV;
   vector<float>   *mu_muid_ie_qoverp_exPV;
   vector<float>   *mu_muid_cov_d0_exPV;
   vector<float>   *mu_muid_cov_z0_exPV;
   vector<float>   *mu_muid_cov_phi_exPV;
   vector<float>   *mu_muid_cov_theta_exPV;
   vector<float>   *mu_muid_cov_qoverp_exPV;
   vector<float>   *mu_muid_cov_d0_z0_exPV;
   vector<float>   *mu_muid_cov_d0_phi_exPV;
   vector<float>   *mu_muid_cov_d0_theta_exPV;
   vector<float>   *mu_muid_cov_d0_qoverp_exPV;
   vector<float>   *mu_muid_cov_z0_phi_exPV;
   vector<float>   *mu_muid_cov_z0_theta_exPV;
   vector<float>   *mu_muid_cov_z0_qoverp_exPV;
   vector<float>   *mu_muid_cov_phi_theta_exPV;
   vector<float>   *mu_muid_cov_phi_qoverp_exPV;
   vector<float>   *mu_muid_cov_theta_qoverp_exPV;
   vector<float>   *mu_muid_id_cov_d0_exPV;
   vector<float>   *mu_muid_id_cov_z0_exPV;
   vector<float>   *mu_muid_id_cov_phi_exPV;
   vector<float>   *mu_muid_id_cov_theta_exPV;
   vector<float>   *mu_muid_id_cov_qoverp_exPV;
   vector<float>   *mu_muid_id_cov_d0_z0_exPV;
   vector<float>   *mu_muid_id_cov_d0_phi_exPV;
   vector<float>   *mu_muid_id_cov_d0_theta_exPV;
   vector<float>   *mu_muid_id_cov_d0_qoverp_exPV;
   vector<float>   *mu_muid_id_cov_z0_phi_exPV;
   vector<float>   *mu_muid_id_cov_z0_theta_exPV;
   vector<float>   *mu_muid_id_cov_z0_qoverp_exPV;
   vector<float>   *mu_muid_id_cov_phi_theta_exPV;
   vector<float>   *mu_muid_id_cov_phi_qoverp_exPV;
   vector<float>   *mu_muid_id_cov_theta_qoverp_exPV;
   vector<float>   *mu_muid_ms_d0;
   vector<float>   *mu_muid_ms_z0;
   vector<float>   *mu_muid_ms_phi;
   vector<float>   *mu_muid_ms_theta;
   vector<float>   *mu_muid_ms_qoverp;
   vector<float>   *mu_muid_id_d0;
   vector<float>   *mu_muid_id_z0;
   vector<float>   *mu_muid_id_phi;
   vector<float>   *mu_muid_id_theta;
   vector<float>   *mu_muid_id_qoverp;
   vector<float>   *mu_muid_me_d0;
   vector<float>   *mu_muid_me_z0;
   vector<float>   *mu_muid_me_phi;
   vector<float>   *mu_muid_me_theta;
   vector<float>   *mu_muid_me_qoverp;
   vector<float>   *mu_muid_ie_d0;
   vector<float>   *mu_muid_ie_z0;
   vector<float>   *mu_muid_ie_phi;
   vector<float>   *mu_muid_ie_theta;
   vector<float>   *mu_muid_ie_qoverp;
   vector<int>     *mu_muid_nOutliersOnTrack;
   vector<int>     *mu_muid_nBLHits;
   vector<int>     *mu_muid_nPixHits;
   vector<int>     *mu_muid_nSCTHits;
   vector<int>     *mu_muid_nTRTHits;
   vector<int>     *mu_muid_nTRTHighTHits;
   vector<int>     *mu_muid_nBLSharedHits;
   vector<int>     *mu_muid_nPixSharedHits;
   vector<int>     *mu_muid_nPixHoles;
   vector<int>     *mu_muid_nSCTSharedHits;
   vector<int>     *mu_muid_nSCTHoles;
   vector<int>     *mu_muid_nTRTOutliers;
   vector<int>     *mu_muid_nTRTHighTOutliers;
   vector<int>     *mu_muid_nGangedPixels;
   vector<int>     *mu_muid_nPixelDeadSensors;
   vector<int>     *mu_muid_nSCTDeadSensors;
   vector<int>     *mu_muid_nTRTDeadStraws;
   vector<int>     *mu_muid_expectBLayerHit;
   vector<int>     *mu_muid_nMDTHits;
   vector<int>     *mu_muid_nMDTHoles;
   vector<int>     *mu_muid_nCSCEtaHits;
   vector<int>     *mu_muid_nCSCEtaHoles;
   vector<int>     *mu_muid_nCSCPhiHits;
   vector<int>     *mu_muid_nCSCPhiHoles;
   vector<int>     *mu_muid_nRPCEtaHits;
   vector<int>     *mu_muid_nRPCEtaHoles;
   vector<int>     *mu_muid_nRPCPhiHits;
   vector<int>     *mu_muid_nRPCPhiHoles;
   vector<int>     *mu_muid_nTGCEtaHits;
   vector<int>     *mu_muid_nTGCEtaHoles;
   vector<int>     *mu_muid_nTGCPhiHits;
   vector<int>     *mu_muid_nTGCPhiHoles;
   vector<int>     *mu_muid_nMDTBIHits;
   vector<int>     *mu_muid_nMDTBMHits;
   vector<int>     *mu_muid_nMDTBOHits;
   vector<int>     *mu_muid_nMDTBEEHits;
   vector<int>     *mu_muid_nMDTBIS78Hits;
   vector<int>     *mu_muid_nMDTEIHits;
   vector<int>     *mu_muid_nMDTEMHits;
   vector<int>     *mu_muid_nMDTEOHits;
   vector<int>     *mu_muid_nMDTEEHits;
   vector<int>     *mu_muid_nRPCLayer1EtaHits;
   vector<int>     *mu_muid_nRPCLayer2EtaHits;
   vector<int>     *mu_muid_nRPCLayer3EtaHits;
   vector<int>     *mu_muid_nRPCLayer1PhiHits;
   vector<int>     *mu_muid_nRPCLayer2PhiHits;
   vector<int>     *mu_muid_nRPCLayer3PhiHits;
   vector<int>     *mu_muid_nTGCLayer1EtaHits;
   vector<int>     *mu_muid_nTGCLayer2EtaHits;
   vector<int>     *mu_muid_nTGCLayer3EtaHits;
   vector<int>     *mu_muid_nTGCLayer4EtaHits;
   vector<int>     *mu_muid_nTGCLayer1PhiHits;
   vector<int>     *mu_muid_nTGCLayer2PhiHits;
   vector<int>     *mu_muid_nTGCLayer3PhiHits;
   vector<int>     *mu_muid_nTGCLayer4PhiHits;
   vector<int>     *mu_muid_barrelSectors;
   vector<int>     *mu_muid_endcapSectors;
   vector<float>   *mu_muid_trackd0;
   vector<float>   *mu_muid_trackz0;
   vector<float>   *mu_muid_trackphi;
   vector<float>   *mu_muid_tracktheta;
   vector<float>   *mu_muid_trackqoverp;
   vector<float>   *mu_muid_trackcov_d0;
   vector<float>   *mu_muid_trackcov_z0;
   vector<float>   *mu_muid_trackcov_phi;
   vector<float>   *mu_muid_trackcov_theta;
   vector<float>   *mu_muid_trackcov_qoverp;
   vector<float>   *mu_muid_trackcov_d0_z0;
   vector<float>   *mu_muid_trackcov_d0_phi;
   vector<float>   *mu_muid_trackcov_d0_theta;
   vector<float>   *mu_muid_trackcov_d0_qoverp;
   vector<float>   *mu_muid_trackcov_z0_phi;
   vector<float>   *mu_muid_trackcov_z0_theta;
   vector<float>   *mu_muid_trackcov_z0_qoverp;
   vector<float>   *mu_muid_trackcov_phi_theta;
   vector<float>   *mu_muid_trackcov_phi_qoverp;
   vector<float>   *mu_muid_trackcov_theta_qoverp;
   vector<int>     *mu_muid_hastrack;
   vector<float>   *mu_muid_truth_dr;
   vector<float>   *mu_muid_truth_E;
   vector<float>   *mu_muid_truth_pt;
   vector<float>   *mu_muid_truth_eta;
   vector<float>   *mu_muid_truth_phi;
   vector<int>     *mu_muid_truth_type;
   vector<int>     *mu_muid_truth_status;
   vector<int>     *mu_muid_truth_barcode;
   vector<int>     *mu_muid_truth_mothertype;
   vector<int>     *mu_muid_truth_motherbarcode;
   vector<int>     *mu_muid_truth_matched;
   vector<float>   *mu_muid_EFCB_dr;
   vector<int>     *mu_muid_EFCB_index;
   vector<float>   *mu_muid_EFMG_dr;
   vector<int>     *mu_muid_EFMG_index;
   vector<float>   *mu_muid_EFME_dr;
   vector<int>     *mu_muid_EFME_index;
   Int_t           trueTau_n;
   vector<float>   *trueTau_pt;
   vector<float>   *trueTau_m;
   vector<float>   *trueTau_eta;
   vector<float>   *trueTau_phi;
   vector<float>   *trueTau_vis_m;
   vector<float>   *trueTau_vis_Et;
   vector<float>   *trueTau_vis_eta;
   vector<float>   *trueTau_vis_phi;
   vector<int>     *trueTau_nProng;
   vector<int>     *trueTau_nPi0;
   vector<float>   *trueTau_charge;
   vector<float>   *trueTau_tauAssocSmall_dr;
   vector<int>     *trueTau_tauAssocSmall_index;
   vector<int>     *trueTau_tauAssocSmall_matched;
   Int_t           tau_n;
   vector<float>   *tau_Et;
   vector<float>   *tau_pt;
   vector<float>   *tau_m;
   vector<float>   *tau_eta;
   vector<float>   *tau_phi;
   vector<float>   *tau_charge;
   vector<float>   *tau_BDTEleScore;
   vector<float>   *tau_BDTJetScore;
   vector<float>   *tau_likelihood;
   vector<float>   *tau_SafeLikelihood;
   vector<int>     *tau_electronVetoLoose;
   vector<int>     *tau_electronVetoMedium;
   vector<int>     *tau_electronVetoTight;
   vector<int>     *tau_muonVeto;
   vector<int>     *tau_tauCutLoose;
   vector<int>     *tau_tauCutMedium;
   vector<int>     *tau_tauCutTight;
   vector<int>     *tau_tauLlhLoose;
   vector<int>     *tau_tauLlhMedium;
   vector<int>     *tau_tauLlhTight;
   vector<int>     *tau_JetBDTSigLoose;
   vector<int>     *tau_JetBDTSigMedium;
   vector<int>     *tau_JetBDTSigTight;
   vector<int>     *tau_EleBDTLoose;
   vector<int>     *tau_EleBDTMedium;
   vector<int>     *tau_EleBDTTight;
   vector<int>     *tau_author;
   vector<int>     *tau_RoIWord;
   vector<int>     *tau_nProng;
   vector<int>     *tau_numTrack;
   vector<int>     *tau_seedCalo_numTrack;
   vector<float>   *tau_etOverPtLeadTrk;
   vector<float>   *tau_ipZ0SinThetaSigLeadTrk;
   vector<float>   *tau_leadTrkPt;
   vector<int>     *tau_nLooseTrk;
   vector<int>     *tau_nLooseConvTrk;
   vector<int>     *tau_nProngLoose;
   vector<float>   *tau_ipSigLeadTrk;
   vector<float>   *tau_ipSigLeadLooseTrk;
   vector<float>   *tau_etOverPtLeadLooseTrk;
   vector<float>   *tau_leadLooseTrkPt;
   vector<float>   *tau_chrgLooseTrk;
   vector<float>   *tau_massTrkSys;
   vector<float>   *tau_trkWidth2;
   vector<float>   *tau_trFlightPathSig;
   vector<float>   *tau_etEflow;
   vector<float>   *tau_mEflow;
   vector<int>     *tau_nPi0;
   vector<float>   *tau_ele_E237E277;
   vector<float>   *tau_ele_PresamplerFraction;
   vector<float>   *tau_ele_ECALFirstFraction;
   vector<float>   *tau_EM_TES_scale;
   vector<float>   *tau_LC_TES_precalib;
   vector<float>   *tau_seedCalo_EMRadius;
   vector<float>   *tau_seedCalo_hadRadius;
   vector<float>   *tau_seedCalo_etEMAtEMScale;
   vector<float>   *tau_seedCalo_etHadAtEMScale;
   vector<float>   *tau_seedCalo_isolFrac;
   vector<float>   *tau_seedCalo_centFrac;
   vector<float>   *tau_seedCalo_stripWidth2;
   vector<int>     *tau_seedCalo_nStrip;
   vector<float>   *tau_seedCalo_eta;
   vector<float>   *tau_seedCalo_phi;
   vector<float>   *tau_seedCalo_nIsolLooseTrk;
   vector<float>   *tau_seedCalo_trkAvgDist;
   vector<float>   *tau_seedCalo_trkRmsDist;
   vector<float>   *tau_seedCalo_caloIso;
   vector<float>   *tau_seedCalo_caloIsoCorrected;
   vector<float>   *tau_seedCalo_dRmax;
   vector<float>   *tau_seedCalo_lead2ClusterEOverAllClusterE;
   vector<float>   *tau_seedCalo_lead3ClusterEOverAllClusterE;
   vector<int>     *tau_numTopoClusters;
   vector<float>   *tau_numEffTopoClusters;
   vector<float>   *tau_topoInvMass;
   vector<float>   *tau_effTopoInvMass;
   vector<float>   *tau_topoMeanDeltaR;
   vector<float>   *tau_effTopoMeanDeltaR;
   vector<float>   *tau_numCells;
   vector<float>   *tau_seedTrk_EMRadius;
   vector<float>   *tau_seedTrk_isolFrac;
   vector<float>   *tau_seedTrk_etChrgHadOverSumTrkPt;
   vector<float>   *tau_seedTrk_isolFracWide;
   vector<float>   *tau_seedTrk_etHadAtEMScale;
   vector<float>   *tau_seedTrk_etEMAtEMScale;
   vector<float>   *tau_seedTrk_etEMCL;
   vector<float>   *tau_seedTrk_etChrgEM;
   vector<float>   *tau_seedTrk_etNeuEM;
   vector<float>   *tau_seedTrk_etResNeuEM;
   vector<float>   *tau_seedTrk_hadLeakEt;
   vector<float>   *tau_seedTrk_sumEMCellEtOverLeadTrkPt;
   vector<float>   *tau_seedTrk_secMaxStripEt;
   vector<float>   *tau_seedTrk_stripWidth2;
   vector<int>     *tau_seedTrk_nStrip;
   vector<float>   *tau_seedTrk_etChrgHad;
   vector<int>     *tau_seedTrk_nOtherCoreTrk;
   vector<int>     *tau_seedTrk_nIsolTrk;
   vector<float>   *tau_seedTrk_etIsolEM;
   vector<float>   *tau_seedTrk_etIsolHad;
   vector<float>   *tau_calcVars_etHad_EMScale_Pt3Trks;
   vector<float>   *tau_calcVars_etEM_EMScale_Pt3Trks;
   vector<float>   *tau_calcVars_ipSigLeadLooseTrk;
   vector<float>   *tau_calcVars_drMin;
   vector<float>   *tau_calcVars_TRTHTOverLT_LeadTrk;
   vector<float>   *tau_calcVars_calRadius;
   vector<float>   *tau_calcVars_EMFractionAtEMScale;
   vector<float>   *tau_calcVars_trackIso;
   vector<float>   *tau_calcVars_ChPiEMEOverCaloEME;
   vector<float>   *tau_calcVars_PSSFraction;
   vector<float>   *tau_calcVars_EMPOverTrkSysP;
   vector<vector<float> > *tau_cluster_E;
   vector<vector<float> > *tau_cluster_eta;
   vector<vector<float> > *tau_cluster_phi;
   vector<vector<float> > *tau_cluster_PreSamplerStripF;
   vector<vector<float> > *tau_cluster_EMLayer2F;
   vector<vector<float> > *tau_cluster_EMLayer3F;
   vector<unsigned int> *tau_cluster_n;
   vector<vector<float> > *tau_Pi0Cluster_pt;
   vector<vector<float> > *tau_Pi0Cluster_eta;
   vector<vector<float> > *tau_Pi0Cluster_phi;
   vector<float>   *tau_secvtx_x;
   vector<float>   *tau_secvtx_y;
   vector<float>   *tau_secvtx_z;
   vector<float>   *tau_secvtx_chiSquared;
   vector<float>   *tau_secvtx_numberDoF;
   vector<float>   *tau_jet_Et;
   vector<float>   *tau_jet_pt;
   vector<float>   *tau_jet_m;
   vector<float>   *tau_jet_eta;
   vector<float>   *tau_jet_phi;
   vector<float>   *tau_jet_WIDTH;
   vector<float>   *tau_jet_n90;
   vector<float>   *tau_jet_n90constituents;
   vector<float>   *tau_jet_BCH_CORR_CELL;
   vector<float>   *tau_jet_BCH_CORR_JET;
   vector<float>   *tau_jet_BCH_CORR_JET_FORCELL;
   vector<float>   *tau_jet_ENG_BAD_CELLS;
   vector<float>   *tau_jet_N_BAD_CELLS;
   vector<float>   *tau_jet_N_BAD_CELLS_CORR;
   vector<float>   *tau_jet_BAD_CELLS_CORR_E;
   vector<float>   *tau_jet_Timing;
   vector<float>   *tau_jet_LArQuality;
   vector<float>   *tau_jet_nTrk;
   vector<float>   *tau_jet_sumPtTrk;
   vector<float>   *tau_jet_OriginIndex;
   vector<float>   *tau_jet_HECQuality;
   vector<float>   *tau_jet_NegativeE;
   vector<float>   *tau_jet_YFlip12;
   vector<float>   *tau_jet_YFlip23;
   vector<float>   *tau_jet_BCH_CORR_DOTX;
   vector<int>     *tau_jet_SamplingMax;
   vector<float>   *tau_jet_fracSamplingMax;
   vector<float>   *tau_jet_hecf;
   vector<float>   *tau_jet_tgap3f;
   vector<int>     *tau_jet_isUgly;
   vector<int>     *tau_jet_isBadLoose;
   vector<int>     *tau_jet_isBadMedium;
   vector<int>     *tau_jet_isBadTight;
   vector<float>   *tau_jet_emfrac;
   vector<float>   *tau_jet_GCWJES;
   vector<float>   *tau_jet_EMJES;
   vector<float>   *tau_jet_emscale_E;
   vector<float>   *tau_jet_emscale_pt;
   vector<float>   *tau_jet_emscale_m;
   vector<float>   *tau_jet_emscale_eta;
   vector<float>   *tau_jet_emscale_phi;
   vector<float>   *tau_jet_jvtx_x;
   vector<float>   *tau_jet_jvtx_y;
   vector<float>   *tau_jet_jvtx_z;
   vector<float>   *tau_jet_jvtxf;
   vector<double>  *tau_jet_flavor_weight;
   vector<double>  *tau_jet_flavor_weight_TrackCounting2D;
   vector<double>  *tau_jet_flavor_weight_JetProb;
   vector<double>  *tau_jet_flavor_weight_IP1D;
   vector<double>  *tau_jet_flavor_weight_IP2D;
   vector<double>  *tau_jet_flavor_weight_IP3D;
   vector<double>  *tau_jet_flavor_weight_SV0;
   vector<double>  *tau_jet_flavor_weight_SV1;
   vector<double>  *tau_jet_flavor_weight_SV2;
   vector<double>  *tau_jet_flavor_weight_JetFitterTag;
   vector<double>  *tau_jet_flavor_weight_JetFitterCOMB;
   vector<double>  *tau_jet_flavor_weight_JetFitterTagNN;
   vector<double>  *tau_jet_flavor_weight_JetFitterCOMBNN;
   vector<double>  *tau_jet_flavor_weight_SoftMuonTag;
   vector<double>  *tau_jet_flavor_weight_SoftElectronTag;
   vector<double>  *tau_jet_flavor_weight_IP3DSV1;
   vector<float>   *tau_jet_e_PreSamplerB;
   vector<float>   *tau_jet_e_EMB1;
   vector<float>   *tau_jet_e_EMB2;
   vector<float>   *tau_jet_e_EMB3;
   vector<float>   *tau_jet_e_PreSamplerE;
   vector<float>   *tau_jet_e_EME1;
   vector<float>   *tau_jet_e_EME2;
   vector<float>   *tau_jet_e_EME3;
   vector<float>   *tau_jet_e_HEC0;
   vector<float>   *tau_jet_e_HEC1;
   vector<float>   *tau_jet_e_HEC2;
   vector<float>   *tau_jet_e_HEC3;
   vector<float>   *tau_jet_e_TileBar0;
   vector<float>   *tau_jet_e_TileBar1;
   vector<float>   *tau_jet_e_TileBar2;
   vector<float>   *tau_jet_e_TileGap1;
   vector<float>   *tau_jet_e_TileGap2;
   vector<float>   *tau_jet_e_TileGap3;
   vector<float>   *tau_jet_e_TileExt0;
   vector<float>   *tau_jet_e_TileExt1;
   vector<float>   *tau_jet_e_TileExt2;
   vector<float>   *tau_jet_e_FCAL0;
   vector<float>   *tau_jet_e_FCAL1;
   vector<float>   *tau_jet_e_FCAL2;
   vector<vector<float> > *tau_jet_shapeBins;
   vector<int>     *tau_seedCalo_track_n;
   vector<vector<float> > *tau_seedCalo_track_d0;
   vector<vector<float> > *tau_seedCalo_track_z0;
   vector<vector<float> > *tau_seedCalo_track_phi;
   vector<vector<float> > *tau_seedCalo_track_theta;
   vector<vector<float> > *tau_seedCalo_track_qoverp;
   vector<vector<float> > *tau_seedCalo_track_pt;
   vector<vector<float> > *tau_seedCalo_track_eta;
   vector<vector<float> > *tau_seedCalo_track_atPV_d0;
   vector<vector<float> > *tau_seedCalo_track_atPV_z0;
   vector<vector<float> > *tau_seedCalo_track_atPV_phi;
   vector<vector<float> > *tau_seedCalo_track_atPV_theta;
   vector<vector<float> > *tau_seedCalo_track_atPV_qoverp;
   vector<vector<float> > *tau_seedCalo_track_atPV_pt;
   vector<vector<float> > *tau_seedCalo_track_atPV_eta;
   vector<vector<int> > *tau_seedCalo_track_nBLHits;
   vector<vector<int> > *tau_seedCalo_track_nPixHits;
   vector<vector<int> > *tau_seedCalo_track_nSCTHits;
   vector<vector<int> > *tau_seedCalo_track_nTRTHits;
   vector<vector<int> > *tau_seedCalo_track_nTRTHighTHits;
   vector<vector<int> > *tau_seedCalo_track_nPixHoles;
   vector<vector<int> > *tau_seedCalo_track_nSCTHoles;
   vector<vector<int> > *tau_seedCalo_track_nTRTHoles;
   vector<vector<int> > *tau_seedCalo_track_nBLSharedHits;
   vector<vector<int> > *tau_seedCalo_track_nPixSharedHits;
   vector<vector<int> > *tau_seedCalo_track_nSCTSharedHits;
   vector<vector<int> > *tau_seedCalo_track_nBLayerOutliers;
   vector<vector<int> > *tau_seedCalo_track_nPixelOutliers;
   vector<vector<int> > *tau_seedCalo_track_nSCTOutliers;
   vector<vector<int> > *tau_seedCalo_track_nTRTOutliers;
   vector<vector<int> > *tau_seedCalo_track_nTRTHighTOutliers;
   vector<vector<int> > *tau_seedCalo_track_nContribPixelLayers;
   vector<vector<int> > *tau_seedCalo_track_nGangedPixels;
   vector<vector<int> > *tau_seedCalo_track_nGangedFlaggedFakes;
   vector<vector<int> > *tau_seedCalo_track_nPixelDeadSensors;
   vector<vector<int> > *tau_seedCalo_track_nPixelSpoiltHits;
   vector<vector<int> > *tau_seedCalo_track_nSCTDoubleHoles;
   vector<vector<int> > *tau_seedCalo_track_nSCTDeadSensors;
   vector<vector<int> > *tau_seedCalo_track_nSCTSpoiltHits;
   vector<vector<int> > *tau_seedCalo_track_expectBLayerHit;
   vector<vector<int> > *tau_seedCalo_track_nHits;
   vector<int>     *tau_seedCalo_wideTrk_n;
   vector<vector<float> > *tau_seedCalo_wideTrk_d0;
   vector<vector<float> > *tau_seedCalo_wideTrk_z0;
   vector<vector<float> > *tau_seedCalo_wideTrk_phi;
   vector<vector<float> > *tau_seedCalo_wideTrk_theta;
   vector<vector<float> > *tau_seedCalo_wideTrk_qoverp;
   vector<vector<float> > *tau_seedCalo_wideTrk_pt;
   vector<vector<float> > *tau_seedCalo_wideTrk_eta;
   vector<vector<float> > *tau_seedCalo_wideTrk_atPV_d0;
   vector<vector<float> > *tau_seedCalo_wideTrk_atPV_z0;
   vector<vector<float> > *tau_seedCalo_wideTrk_atPV_phi;
   vector<vector<float> > *tau_seedCalo_wideTrk_atPV_theta;
   vector<vector<float> > *tau_seedCalo_wideTrk_atPV_qoverp;
   vector<vector<float> > *tau_seedCalo_wideTrk_atPV_pt;
   vector<vector<float> > *tau_seedCalo_wideTrk_atPV_eta;
   vector<vector<int> > *tau_seedCalo_wideTrk_nBLHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nPixHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nSCTHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nTRTHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nTRTHighTHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nPixHoles;
   vector<vector<int> > *tau_seedCalo_wideTrk_nSCTHoles;
   vector<vector<int> > *tau_seedCalo_wideTrk_nTRTHoles;
   vector<vector<int> > *tau_seedCalo_wideTrk_nBLSharedHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nPixSharedHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nSCTSharedHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nBLayerOutliers;
   vector<vector<int> > *tau_seedCalo_wideTrk_nPixelOutliers;
   vector<vector<int> > *tau_seedCalo_wideTrk_nSCTOutliers;
   vector<vector<int> > *tau_seedCalo_wideTrk_nTRTOutliers;
   vector<vector<int> > *tau_seedCalo_wideTrk_nTRTHighTOutliers;
   vector<vector<int> > *tau_seedCalo_wideTrk_nContribPixelLayers;
   vector<vector<int> > *tau_seedCalo_wideTrk_nGangedPixels;
   vector<vector<int> > *tau_seedCalo_wideTrk_nGangedFlaggedFakes;
   vector<vector<int> > *tau_seedCalo_wideTrk_nPixelDeadSensors;
   vector<vector<int> > *tau_seedCalo_wideTrk_nPixelSpoiltHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_nSCTDoubleHoles;
   vector<vector<int> > *tau_seedCalo_wideTrk_nSCTDeadSensors;
   vector<vector<int> > *tau_seedCalo_wideTrk_nSCTSpoiltHits;
   vector<vector<int> > *tau_seedCalo_wideTrk_expectBLayerHit;
   vector<vector<int> > *tau_seedCalo_wideTrk_nHits;
   vector<int>     *tau_otherTrk_n;
   vector<vector<float> > *tau_otherTrk_d0;
   vector<vector<float> > *tau_otherTrk_z0;
   vector<vector<float> > *tau_otherTrk_phi;
   vector<vector<float> > *tau_otherTrk_theta;
   vector<vector<float> > *tau_otherTrk_qoverp;
   vector<vector<float> > *tau_otherTrk_pt;
   vector<vector<float> > *tau_otherTrk_eta;
   vector<vector<float> > *tau_otherTrk_atPV_d0;
   vector<vector<float> > *tau_otherTrk_atPV_z0;
   vector<vector<float> > *tau_otherTrk_atPV_phi;
   vector<vector<float> > *tau_otherTrk_atPV_theta;
   vector<vector<float> > *tau_otherTrk_atPV_qoverp;
   vector<vector<float> > *tau_otherTrk_atPV_pt;
   vector<vector<float> > *tau_otherTrk_atPV_eta;
   vector<vector<int> > *tau_otherTrk_nBLHits;
   vector<vector<int> > *tau_otherTrk_nPixHits;
   vector<vector<int> > *tau_otherTrk_nSCTHits;
   vector<vector<int> > *tau_otherTrk_nTRTHits;
   vector<vector<int> > *tau_otherTrk_nTRTHighTHits;
   vector<vector<int> > *tau_otherTrk_nPixHoles;
   vector<vector<int> > *tau_otherTrk_nSCTHoles;
   vector<vector<int> > *tau_otherTrk_nTRTHoles;
   vector<vector<int> > *tau_otherTrk_nBLSharedHits;
   vector<vector<int> > *tau_otherTrk_nPixSharedHits;
   vector<vector<int> > *tau_otherTrk_nSCTSharedHits;
   vector<vector<int> > *tau_otherTrk_nBLayerOutliers;
   vector<vector<int> > *tau_otherTrk_nPixelOutliers;
   vector<vector<int> > *tau_otherTrk_nSCTOutliers;
   vector<vector<int> > *tau_otherTrk_nTRTOutliers;
   vector<vector<int> > *tau_otherTrk_nTRTHighTOutliers;
   vector<vector<int> > *tau_otherTrk_nContribPixelLayers;
   vector<vector<int> > *tau_otherTrk_nGangedPixels;
   vector<vector<int> > *tau_otherTrk_nGangedFlaggedFakes;
   vector<vector<int> > *tau_otherTrk_nPixelDeadSensors;
   vector<vector<int> > *tau_otherTrk_nPixelSpoiltHits;
   vector<vector<int> > *tau_otherTrk_nSCTDoubleHoles;
   vector<vector<int> > *tau_otherTrk_nSCTDeadSensors;
   vector<vector<int> > *tau_otherTrk_nSCTSpoiltHits;
   vector<vector<int> > *tau_otherTrk_expectBLayerHit;
   vector<vector<int> > *tau_otherTrk_nHits;
   vector<float>   *tau_EF_dr;
   vector<float>   *tau_EF_E;
   vector<float>   *tau_EF_Et;
   vector<float>   *tau_EF_pt;
   vector<float>   *tau_EF_eta;
   vector<float>   *tau_EF_phi;
   vector<int>     *tau_EF_matched;
   vector<float>   *tau_L2_dr;
   vector<float>   *tau_L2_E;
   vector<float>   *tau_L2_Et;
   vector<float>   *tau_L2_pt;
   vector<float>   *tau_L2_eta;
   vector<float>   *tau_L2_phi;
   vector<int>     *tau_L2_matched;
   vector<float>   *tau_L1_dr;
   vector<float>   *tau_L1_Et;
   vector<float>   *tau_L1_pt;
   vector<float>   *tau_L1_eta;
   vector<float>   *tau_L1_phi;
   vector<int>     *tau_L1_matched;
   vector<float>   *tau_trueTauAssocSmall_dr;
   vector<int>     *tau_trueTauAssocSmall_index;
   vector<int>     *tau_trueTauAssocSmall_matched;
   Float_t         MET_RefFinal_etx;
   Float_t         MET_RefFinal_ety;
   Float_t         MET_RefFinal_phi;
   Float_t         MET_RefFinal_et;
   Float_t         MET_RefFinal_sumet;
   Float_t         MET_RefFinal_etx_CentralReg;
   Float_t         MET_RefFinal_ety_CentralReg;
   Float_t         MET_RefFinal_sumet_CentralReg;
   Float_t         MET_RefFinal_phi_CentralReg;
   Float_t         MET_RefFinal_etx_EndcapRegion;
   Float_t         MET_RefFinal_ety_EndcapRegion;
   Float_t         MET_RefFinal_sumet_EndcapRegion;
   Float_t         MET_RefFinal_phi_EndcapRegion;
   Float_t         MET_RefFinal_etx_ForwardReg;
   Float_t         MET_RefFinal_ety_ForwardReg;
   Float_t         MET_RefFinal_sumet_ForwardReg;
   Float_t         MET_RefFinal_phi_ForwardReg;
   Float_t         MET_RefEle_etx;
   Float_t         MET_RefEle_ety;
   Float_t         MET_RefEle_sumet;
   Float_t         MET_RefJet_etx;
   Float_t         MET_RefJet_ety;
   Float_t         MET_RefJet_sumet;
   Float_t         MET_SoftJets_etx;
   Float_t         MET_SoftJets_ety;
   Float_t         MET_SoftJets_sumet;
   Float_t         MET_RefMuon_etx;
   Float_t         MET_RefMuon_ety;
   Float_t         MET_RefMuon_sumet;
   Float_t         MET_RefMuon_Staco_etx;
   Float_t         MET_RefMuon_Staco_ety;
   Float_t         MET_RefMuon_Staco_sumet;
   Float_t         MET_RefMuon_Muid_etx;
   Float_t         MET_RefMuon_Muid_ety;
   Float_t         MET_RefMuon_Muid_sumet;
   Float_t         MET_RefTau_etx;
   Float_t         MET_RefTau_ety;
   Float_t         MET_RefTau_sumet;
   Float_t         MET_CellOut_etx;
   Float_t         MET_CellOut_ety;
   Float_t         MET_CellOut_sumet;
   Float_t         MET_CellOut_etx_CentralReg;
   Float_t         MET_CellOut_ety_CentralReg;
   Float_t         MET_CellOut_sumet_CentralReg;
   Float_t         MET_CellOut_phi_CentralReg;
   Float_t         MET_CellOut_etx_EndcapRegion;
   Float_t         MET_CellOut_ety_EndcapRegion;
   Float_t         MET_CellOut_sumet_EndcapRegion;
   Float_t         MET_CellOut_phi_EndcapRegion;
   Float_t         MET_CellOut_etx_ForwardReg;
   Float_t         MET_CellOut_ety_ForwardReg;
   Float_t         MET_CellOut_sumet_ForwardReg;
   Float_t         MET_CellOut_phi_ForwardReg;
   Float_t         MET_Track_etx;
   Float_t         MET_Track_ety;
   Float_t         MET_Track_sumet;
   Float_t         MET_RefFinal_em_etx;
   Float_t         MET_RefFinal_em_ety;
   Float_t         MET_RefFinal_em_sumet;
   Float_t         MET_RefEle_em_etx;
   Float_t         MET_RefEle_em_ety;
   Float_t         MET_RefEle_em_sumet;
   Float_t         MET_RefJet_em_etx;
   Float_t         MET_RefJet_em_ety;
   Float_t         MET_RefJet_em_sumet;
   Float_t         MET_SoftJets_em_etx;
   Float_t         MET_SoftJets_em_ety;
   Float_t         MET_SoftJets_em_sumet;
   Float_t         MET_RefMuon_em_etx;
   Float_t         MET_RefMuon_em_ety;
   Float_t         MET_RefMuon_em_sumet;
   Float_t         MET_RefGamma_em_etx;
   Float_t         MET_RefGamma_em_ety;
   Float_t         MET_RefGamma_em_sumet;
   Float_t         MET_RefTau_em_etx;
   Float_t         MET_RefTau_em_ety;
   Float_t         MET_RefTau_em_sumet;
   Float_t         MET_Muon_Isol_Staco_etx;
   Float_t         MET_Muon_Isol_Staco_ety;
   Float_t         MET_Muon_Isol_Staco_sumet;
   Float_t         MET_Muon_NonIsol_Staco_etx;
   Float_t         MET_Muon_NonIsol_Staco_ety;
   Float_t         MET_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Muon_Total_Staco_etx;
   Float_t         MET_Muon_Total_Staco_ety;
   Float_t         MET_Muon_Total_Staco_sumet;
   Float_t         MET_Muon_Isol_Muid_etx;
   Float_t         MET_Muon_Isol_Muid_ety;
   Float_t         MET_Muon_Isol_Muid_sumet;
   Float_t         MET_Muon_NonIsol_Muid_etx;
   Float_t         MET_Muon_NonIsol_Muid_ety;
   Float_t         MET_Muon_NonIsol_Muid_sumet;
   Float_t         MET_Muon_Total_Muid_etx;
   Float_t         MET_Muon_Total_Muid_ety;
   Float_t         MET_Muon_Total_Muid_sumet;
   Float_t         MET_MuonBoy_etx;
   Float_t         MET_MuonBoy_ety;
   Float_t         MET_MuonBoy_sumet;
   Float_t         MET_MuonBoy_etx_CentralReg;
   Float_t         MET_MuonBoy_ety_CentralReg;
   Float_t         MET_MuonBoy_sumet_CentralReg;
   Float_t         MET_MuonBoy_phi_CentralReg;
   Float_t         MET_MuonBoy_etx_EndcapRegion;
   Float_t         MET_MuonBoy_ety_EndcapRegion;
   Float_t         MET_MuonBoy_sumet_EndcapRegion;
   Float_t         MET_MuonBoy_phi_EndcapRegion;
   Float_t         MET_MuonBoy_etx_ForwardReg;
   Float_t         MET_MuonBoy_ety_ForwardReg;
   Float_t         MET_MuonBoy_sumet_ForwardReg;
   Float_t         MET_MuonBoy_phi_ForwardReg;
   Float_t         MET_CellOut_Eflow_etx;
   Float_t         MET_CellOut_Eflow_ety;
   Float_t         MET_CellOut_Eflow_sumet;
   Float_t         MET_RefMuon_Track_etx;
   Float_t         MET_RefMuon_Track_ety;
   Float_t         MET_RefMuon_Track_sumet;
   Float_t         MET_RefMuon_Track_Staco_etx;
   Float_t         MET_RefMuon_Track_Staco_ety;
   Float_t         MET_RefMuon_Track_Staco_sumet;
   Float_t         MET_RefMuon_Track_Staco_etx_CentralReg;
   Float_t         MET_RefMuon_Track_Staco_ety_CentralReg;
   Float_t         MET_RefMuon_Track_Staco_sumet_CentralReg;
   Float_t         MET_RefMuon_Track_Staco_phi_CentralReg;
   Float_t         MET_RefMuon_Track_Staco_etx_EndcapRegion;
   Float_t         MET_RefMuon_Track_Staco_ety_EndcapRegion;
   Float_t         MET_RefMuon_Track_Staco_sumet_EndcapRegion;
   Float_t         MET_RefMuon_Track_Staco_phi_EndcapRegion;
   Float_t         MET_RefMuon_Track_Staco_etx_ForwardReg;
   Float_t         MET_RefMuon_Track_Staco_ety_ForwardReg;
   Float_t         MET_RefMuon_Track_Staco_sumet_ForwardReg;
   Float_t         MET_RefMuon_Track_Staco_phi_ForwardReg;
   Float_t         MET_RefMuon_Track_Muid_etx;
   Float_t         MET_RefMuon_Track_Muid_ety;
   Float_t         MET_RefMuon_Track_Muid_sumet;
   Float_t         MET_RefMuon_Track_Muid_etx_CentralReg;
   Float_t         MET_RefMuon_Track_Muid_ety_CentralReg;
   Float_t         MET_RefMuon_Track_Muid_sumet_CentralReg;
   Float_t         MET_RefMuon_Track_Muid_phi_CentralReg;
   Float_t         MET_RefMuon_Track_Muid_etx_EndcapRegion;
   Float_t         MET_RefMuon_Track_Muid_ety_EndcapRegion;
   Float_t         MET_RefMuon_Track_Muid_sumet_EndcapRegion;
   Float_t         MET_RefMuon_Track_Muid_phi_EndcapRegion;
   Float_t         MET_RefMuon_Track_Muid_etx_ForwardReg;
   Float_t         MET_RefMuon_Track_Muid_ety_ForwardReg;
   Float_t         MET_RefMuon_Track_Muid_sumet_ForwardReg;
   Float_t         MET_RefMuon_Track_Muid_phi_ForwardReg;
   Float_t         MET_Final_etx;
   Float_t         MET_Final_ety;
   Float_t         MET_Final_sumet;
   Float_t         MET_TopoObj_etx;
   Float_t         MET_TopoObj_ety;
   Float_t         MET_TopoObj_sumet;
   Float_t         MET_LocHadTopo_etx;
   Float_t         MET_LocHadTopo_ety;
   Float_t         MET_LocHadTopo_sumet;
   Float_t         MET_LocHadTopo_etx_CentralReg;
   Float_t         MET_LocHadTopo_ety_CentralReg;
   Float_t         MET_LocHadTopo_sumet_CentralReg;
   Float_t         MET_LocHadTopo_phi_CentralReg;
   Float_t         MET_LocHadTopo_etx_EndcapRegion;
   Float_t         MET_LocHadTopo_ety_EndcapRegion;
   Float_t         MET_LocHadTopo_sumet_EndcapRegion;
   Float_t         MET_LocHadTopo_phi_EndcapRegion;
   Float_t         MET_LocHadTopo_etx_ForwardReg;
   Float_t         MET_LocHadTopo_ety_ForwardReg;
   Float_t         MET_LocHadTopo_sumet_ForwardReg;
   Float_t         MET_LocHadTopo_phi_ForwardReg;
   Float_t         MET_Truth_NonInt_etx;
   Float_t         MET_Truth_NonInt_ety;
   Float_t         MET_Truth_NonInt_sumet;
   Float_t         MET_Truth_Int_etx;
   Float_t         MET_Truth_Int_ety;
   Float_t         MET_Truth_IntCentral_etx;
   Float_t         MET_Truth_IntCentral_ety;
   Float_t         MET_Truth_IntFwd_etx;
   Float_t         MET_Truth_IntFwd_ety;
   Float_t         MET_Truth_IntOutCover_etx;
   Float_t         MET_Truth_IntOutCover_ety;
   Float_t         MET_Truth_IntMuons_etx;
   Float_t         MET_Truth_IntMuons_ety;
   Float_t         MET_Truth_Int_sumet;
   Float_t         MET_Truth_IntCentral_sumet;
   Float_t         MET_Truth_IntFwd_sumet;
   Float_t         MET_Truth_IntOutCover_sumet;
   Float_t         MET_Truth_IntMuons_sumet;
   Float_t         MET_Simplified20_RefGamma_etx;
   Float_t         MET_Simplified20_RefGamma_ety;
   Float_t         MET_Simplified20_RefGamma_sumet;
   Float_t         MET_Simplified20_RefGamma_etx_CentralReg;
   Float_t         MET_Simplified20_RefGamma_ety_CentralReg;
   Float_t         MET_Simplified20_RefGamma_sumet_CentralReg;
   Float_t         MET_Simplified20_RefGamma_phi_CentralReg;
   Float_t         MET_Simplified20_RefGamma_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefGamma_etx_ForwardReg;
   Float_t         MET_Simplified20_RefGamma_ety_ForwardReg;
   Float_t         MET_Simplified20_RefGamma_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefGamma_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_etx;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_ety;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_sumet;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefGamma_phi_ForwardReg;
   Float_t         MET_Egamma20_RefGamma_etx;
   Float_t         MET_Egamma20_RefGamma_ety;
   Float_t         MET_Egamma20_RefGamma_sumet;
   Float_t         MET_Egamma20_RefGamma_etx_CentralReg;
   Float_t         MET_Egamma20_RefGamma_ety_CentralReg;
   Float_t         MET_Egamma20_RefGamma_sumet_CentralReg;
   Float_t         MET_Egamma20_RefGamma_phi_CentralReg;
   Float_t         MET_Egamma20_RefGamma_etx_EndcapRegion;
   Float_t         MET_Egamma20_RefGamma_ety_EndcapRegion;
   Float_t         MET_Egamma20_RefGamma_sumet_EndcapRegion;
   Float_t         MET_Egamma20_RefGamma_phi_EndcapRegion;
   Float_t         MET_Egamma20_RefGamma_etx_ForwardReg;
   Float_t         MET_Egamma20_RefGamma_ety_ForwardReg;
   Float_t         MET_Egamma20_RefGamma_sumet_ForwardReg;
   Float_t         MET_Egamma20_RefGamma_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefGamma_etx;
   Float_t         MET_Egamma20NoTau_RefGamma_ety;
   Float_t         MET_Egamma20NoTau_RefGamma_sumet;
   Float_t         MET_Egamma20NoTau_RefGamma_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_RefGamma_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_RefGamma_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_RefGamma_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_RefGamma_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefGamma_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefGamma_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefGamma_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefGamma_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefGamma_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefGamma_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefGamma_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefGamma_etx;
   Float_t         MET_Egamma10NoTau_RefGamma_ety;
   Float_t         MET_Egamma10NoTau_RefGamma_sumet;
   Float_t         MET_Egamma10NoTau_RefGamma_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_RefGamma_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_RefGamma_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_RefGamma_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_RefGamma_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefGamma_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefGamma_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefGamma_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefGamma_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefGamma_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefGamma_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefGamma_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_RefGamma_etx;
   Float_t         MET_LArFixSimp20_RefGamma_ety;
   Float_t         MET_LArFixSimp20_RefGamma_sumet;
   Float_t         MET_LArFixSimp20_RefGamma_etx_CentralReg;
   Float_t         MET_LArFixSimp20_RefGamma_ety_CentralReg;
   Float_t         MET_LArFixSimp20_RefGamma_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_RefGamma_phi_CentralReg;
   Float_t         MET_LArFixSimp20_RefGamma_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefGamma_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefGamma_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefGamma_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefGamma_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_RefGamma_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_RefGamma_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_RefGamma_phi_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_etx;
   Float_t         MET_Simplified20_RefFinal_ety;
   Float_t         MET_Simplified20_RefFinal_sumet;
   Float_t         MET_Simplified20_RefFinal_etx_CentralReg;
   Float_t         MET_Simplified20_RefFinal_ety_CentralReg;
   Float_t         MET_Simplified20_RefFinal_sumet_CentralReg;
   Float_t         MET_Simplified20_RefFinal_phi_CentralReg;
   Float_t         MET_Simplified20_RefFinal_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefFinal_etx_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_ety_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefFinal_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_etx;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_ety;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_sumet;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefFinal_phi_ForwardReg;
   Float_t         MET_Egamma20_RefFinal_etx;
   Float_t         MET_Egamma20_RefFinal_ety;
   Float_t         MET_Egamma20_RefFinal_sumet;
   Float_t         MET_Egamma20_RefFinal_etx_CentralReg;
   Float_t         MET_Egamma20_RefFinal_ety_CentralReg;
   Float_t         MET_Egamma20_RefFinal_sumet_CentralReg;
   Float_t         MET_Egamma20_RefFinal_phi_CentralReg;
   Float_t         MET_Egamma20_RefFinal_etx_EndcapRegion;
   Float_t         MET_Egamma20_RefFinal_ety_EndcapRegion;
   Float_t         MET_Egamma20_RefFinal_sumet_EndcapRegion;
   Float_t         MET_Egamma20_RefFinal_phi_EndcapRegion;
   Float_t         MET_Egamma20_RefFinal_etx_ForwardReg;
   Float_t         MET_Egamma20_RefFinal_ety_ForwardReg;
   Float_t         MET_Egamma20_RefFinal_sumet_ForwardReg;
   Float_t         MET_Egamma20_RefFinal_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefFinal_etx;
   Float_t         MET_Egamma20NoTau_RefFinal_ety;
   Float_t         MET_Egamma20NoTau_RefFinal_sumet;
   Float_t         MET_Egamma20NoTau_RefFinal_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_RefFinal_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_RefFinal_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_RefFinal_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_RefFinal_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefFinal_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefFinal_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefFinal_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefFinal_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefFinal_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefFinal_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefFinal_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefFinal_etx;
   Float_t         MET_Egamma10NoTau_RefFinal_ety;
   Float_t         MET_Egamma10NoTau_RefFinal_sumet;
   Float_t         MET_Egamma10NoTau_RefFinal_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_RefFinal_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_RefFinal_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_RefFinal_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_RefFinal_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefFinal_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefFinal_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefFinal_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefFinal_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefFinal_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefFinal_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefFinal_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_RefFinal_etx;
   Float_t         MET_LArFixSimp20_RefFinal_ety;
   Float_t         MET_LArFixSimp20_RefFinal_sumet;
   Float_t         MET_LArFixSimp20_RefFinal_etx_CentralReg;
   Float_t         MET_LArFixSimp20_RefFinal_ety_CentralReg;
   Float_t         MET_LArFixSimp20_RefFinal_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_RefFinal_phi_CentralReg;
   Float_t         MET_LArFixSimp20_RefFinal_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefFinal_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefFinal_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefFinal_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefFinal_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_RefFinal_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_RefFinal_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_RefFinal_phi_ForwardReg;
   Float_t         MET_Simplified20_RefJet_etx;
   Float_t         MET_Simplified20_RefJet_ety;
   Float_t         MET_Simplified20_RefJet_sumet;
   Float_t         MET_Simplified20_RefJet_etx_CentralReg;
   Float_t         MET_Simplified20_RefJet_ety_CentralReg;
   Float_t         MET_Simplified20_RefJet_sumet_CentralReg;
   Float_t         MET_Simplified20_RefJet_phi_CentralReg;
   Float_t         MET_Simplified20_RefJet_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefJet_etx_ForwardReg;
   Float_t         MET_Simplified20_RefJet_ety_ForwardReg;
   Float_t         MET_Simplified20_RefJet_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefJet_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_etx;
   Float_t         MET_Simplified20withTightPhotons_RefJet_ety;
   Float_t         MET_Simplified20withTightPhotons_RefJet_sumet;
   Float_t         MET_Simplified20withTightPhotons_RefJet_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefJet_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefJet_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefJet_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefJet_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefJet_phi_ForwardReg;
   Float_t         MET_Egamma20_RefJet_etx;
   Float_t         MET_Egamma20_RefJet_ety;
   Float_t         MET_Egamma20_RefJet_sumet;
   Float_t         MET_Egamma20_RefJet_etx_CentralReg;
   Float_t         MET_Egamma20_RefJet_ety_CentralReg;
   Float_t         MET_Egamma20_RefJet_sumet_CentralReg;
   Float_t         MET_Egamma20_RefJet_phi_CentralReg;
   Float_t         MET_Egamma20_RefJet_etx_EndcapRegion;
   Float_t         MET_Egamma20_RefJet_ety_EndcapRegion;
   Float_t         MET_Egamma20_RefJet_sumet_EndcapRegion;
   Float_t         MET_Egamma20_RefJet_phi_EndcapRegion;
   Float_t         MET_Egamma20_RefJet_etx_ForwardReg;
   Float_t         MET_Egamma20_RefJet_ety_ForwardReg;
   Float_t         MET_Egamma20_RefJet_sumet_ForwardReg;
   Float_t         MET_Egamma20_RefJet_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefJet_etx;
   Float_t         MET_Egamma20NoTau_RefJet_ety;
   Float_t         MET_Egamma20NoTau_RefJet_sumet;
   Float_t         MET_Egamma20NoTau_RefJet_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_RefJet_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_RefJet_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_RefJet_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_RefJet_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefJet_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefJet_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefJet_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefJet_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefJet_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefJet_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefJet_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefJet_etx;
   Float_t         MET_Egamma10NoTau_RefJet_ety;
   Float_t         MET_Egamma10NoTau_RefJet_sumet;
   Float_t         MET_Egamma10NoTau_RefJet_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_RefJet_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_RefJet_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_RefJet_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_RefJet_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefJet_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefJet_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefJet_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefJet_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefJet_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefJet_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefJet_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_RefJet_etx;
   Float_t         MET_LArFixSimp20_RefJet_ety;
   Float_t         MET_LArFixSimp20_RefJet_sumet;
   Float_t         MET_LArFixSimp20_RefJet_etx_CentralReg;
   Float_t         MET_LArFixSimp20_RefJet_ety_CentralReg;
   Float_t         MET_LArFixSimp20_RefJet_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_RefJet_phi_CentralReg;
   Float_t         MET_LArFixSimp20_RefJet_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefJet_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefJet_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefJet_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefJet_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_RefJet_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_RefJet_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_RefJet_phi_ForwardReg;
   Float_t         MET_Simplified20_RefEle_etx;
   Float_t         MET_Simplified20_RefEle_ety;
   Float_t         MET_Simplified20_RefEle_sumet;
   Float_t         MET_Simplified20_RefEle_etx_CentralReg;
   Float_t         MET_Simplified20_RefEle_ety_CentralReg;
   Float_t         MET_Simplified20_RefEle_sumet_CentralReg;
   Float_t         MET_Simplified20_RefEle_phi_CentralReg;
   Float_t         MET_Simplified20_RefEle_etx_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_ety_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_sumet_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_phi_EndcapRegion;
   Float_t         MET_Simplified20_RefEle_etx_ForwardReg;
   Float_t         MET_Simplified20_RefEle_ety_ForwardReg;
   Float_t         MET_Simplified20_RefEle_sumet_ForwardReg;
   Float_t         MET_Simplified20_RefEle_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_etx;
   Float_t         MET_Simplified20withTightPhotons_RefEle_ety;
   Float_t         MET_Simplified20withTightPhotons_RefEle_sumet;
   Float_t         MET_Simplified20withTightPhotons_RefEle_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefEle_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefEle_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefEle_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_RefEle_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_RefEle_phi_ForwardReg;
   Float_t         MET_Egamma20_RefEle_etx;
   Float_t         MET_Egamma20_RefEle_ety;
   Float_t         MET_Egamma20_RefEle_sumet;
   Float_t         MET_Egamma20_RefEle_etx_CentralReg;
   Float_t         MET_Egamma20_RefEle_ety_CentralReg;
   Float_t         MET_Egamma20_RefEle_sumet_CentralReg;
   Float_t         MET_Egamma20_RefEle_phi_CentralReg;
   Float_t         MET_Egamma20_RefEle_etx_EndcapRegion;
   Float_t         MET_Egamma20_RefEle_ety_EndcapRegion;
   Float_t         MET_Egamma20_RefEle_sumet_EndcapRegion;
   Float_t         MET_Egamma20_RefEle_phi_EndcapRegion;
   Float_t         MET_Egamma20_RefEle_etx_ForwardReg;
   Float_t         MET_Egamma20_RefEle_ety_ForwardReg;
   Float_t         MET_Egamma20_RefEle_sumet_ForwardReg;
   Float_t         MET_Egamma20_RefEle_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefEle_etx;
   Float_t         MET_Egamma20NoTau_RefEle_ety;
   Float_t         MET_Egamma20NoTau_RefEle_sumet;
   Float_t         MET_Egamma20NoTau_RefEle_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_RefEle_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_RefEle_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_RefEle_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_RefEle_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefEle_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefEle_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefEle_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_RefEle_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefEle_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefEle_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_RefEle_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefEle_etx;
   Float_t         MET_Egamma10NoTau_RefEle_ety;
   Float_t         MET_Egamma10NoTau_RefEle_sumet;
   Float_t         MET_Egamma10NoTau_RefEle_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_RefEle_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_RefEle_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_RefEle_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_RefEle_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefEle_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefEle_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefEle_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_RefEle_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefEle_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefEle_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_RefEle_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_RefEle_etx;
   Float_t         MET_LArFixSimp20_RefEle_ety;
   Float_t         MET_LArFixSimp20_RefEle_sumet;
   Float_t         MET_LArFixSimp20_RefEle_etx_CentralReg;
   Float_t         MET_LArFixSimp20_RefEle_ety_CentralReg;
   Float_t         MET_LArFixSimp20_RefEle_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_RefEle_phi_CentralReg;
   Float_t         MET_LArFixSimp20_RefEle_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefEle_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefEle_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefEle_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_RefEle_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_RefEle_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_RefEle_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_RefEle_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_etx;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_ety;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Staco_phi_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_etx;
   Float_t         MET_Egamma20_Muon_Total_Staco_ety;
   Float_t         MET_Egamma20_Muon_Total_Staco_sumet;
   Float_t         MET_Egamma20_Muon_Total_Staco_etx_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_ety_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_sumet_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_phi_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Staco_etx_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_ety_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Staco_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_etx;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_ety;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_sumet;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Staco_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_etx;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_ety;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_sumet;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Staco_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_etx;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_ety;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_sumet;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_etx_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_ety_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_phi_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Staco_phi_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_etx;
   Float_t         MET_Egamma20_Muon_Isol_Staco_ety;
   Float_t         MET_Egamma20_Muon_Isol_Staco_sumet;
   Float_t         MET_Egamma20_Muon_Isol_Staco_etx_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_ety_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_sumet_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_phi_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Staco_etx_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_ety_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Staco_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_etx;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_ety;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_sumet;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Staco_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_etx;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_ety;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_sumet;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Staco_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_etx;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_ety;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_sumet;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_etx_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_ety_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_phi_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_phi_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_etx;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_ety;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_etx_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_ety_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_sumet_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_phi_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_etx_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_ety_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Staco_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_etx;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_ety;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Staco_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_etx;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_ety;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Staco_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_etx;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_ety;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_sumet;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_etx_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_ety_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_phi_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Staco_phi_ForwardReg;
   Float_t         MET_Simplified20_CellOut_etx;
   Float_t         MET_Simplified20_CellOut_ety;
   Float_t         MET_Simplified20_CellOut_sumet;
   Float_t         MET_Simplified20_CellOut_etx_CentralReg;
   Float_t         MET_Simplified20_CellOut_ety_CentralReg;
   Float_t         MET_Simplified20_CellOut_sumet_CentralReg;
   Float_t         MET_Simplified20_CellOut_phi_CentralReg;
   Float_t         MET_Simplified20_CellOut_etx_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_ety_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_sumet_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_phi_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_etx_ForwardReg;
   Float_t         MET_Simplified20_CellOut_ety_ForwardReg;
   Float_t         MET_Simplified20_CellOut_sumet_ForwardReg;
   Float_t         MET_Simplified20_CellOut_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_etx;
   Float_t         MET_Simplified20withTightPhotons_CellOut_ety;
   Float_t         MET_Simplified20withTightPhotons_CellOut_sumet;
   Float_t         MET_Simplified20withTightPhotons_CellOut_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_phi_ForwardReg;
   Float_t         MET_Egamma20_CellOut_etx;
   Float_t         MET_Egamma20_CellOut_ety;
   Float_t         MET_Egamma20_CellOut_sumet;
   Float_t         MET_Egamma20_CellOut_etx_CentralReg;
   Float_t         MET_Egamma20_CellOut_ety_CentralReg;
   Float_t         MET_Egamma20_CellOut_sumet_CentralReg;
   Float_t         MET_Egamma20_CellOut_phi_CentralReg;
   Float_t         MET_Egamma20_CellOut_etx_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_ety_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_sumet_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_phi_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_etx_ForwardReg;
   Float_t         MET_Egamma20_CellOut_ety_ForwardReg;
   Float_t         MET_Egamma20_CellOut_sumet_ForwardReg;
   Float_t         MET_Egamma20_CellOut_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_etx;
   Float_t         MET_Egamma20NoTau_CellOut_ety;
   Float_t         MET_Egamma20NoTau_CellOut_sumet;
   Float_t         MET_Egamma20NoTau_CellOut_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_etx;
   Float_t         MET_Egamma10NoTau_CellOut_ety;
   Float_t         MET_Egamma10NoTau_CellOut_sumet;
   Float_t         MET_Egamma10NoTau_CellOut_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_etx;
   Float_t         MET_LArFixSimp20_CellOut_ety;
   Float_t         MET_LArFixSimp20_CellOut_sumet;
   Float_t         MET_LArFixSimp20_CellOut_etx_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_ety_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_phi_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_phi_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_etx;
   Float_t         MET_Simplified20_CellOut_Eflow_ety;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet;
   Float_t         MET_Simplified20_CellOut_Eflow_etx_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_ety_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_phi_CentralReg;
   Float_t         MET_Simplified20_CellOut_Eflow_etx_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_ety_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_phi_EndcapRegion;
   Float_t         MET_Simplified20_CellOut_Eflow_etx_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_ety_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_sumet_ForwardReg;
   Float_t         MET_Simplified20_CellOut_Eflow_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_etx;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_ety;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_sumet;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_CellOut_Eflow_phi_ForwardReg;
   Float_t         MET_Egamma20_CellOut_Eflow_etx;
   Float_t         MET_Egamma20_CellOut_Eflow_ety;
   Float_t         MET_Egamma20_CellOut_Eflow_sumet;
   Float_t         MET_Egamma20_CellOut_Eflow_etx_CentralReg;
   Float_t         MET_Egamma20_CellOut_Eflow_ety_CentralReg;
   Float_t         MET_Egamma20_CellOut_Eflow_sumet_CentralReg;
   Float_t         MET_Egamma20_CellOut_Eflow_phi_CentralReg;
   Float_t         MET_Egamma20_CellOut_Eflow_etx_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_Eflow_ety_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_Eflow_sumet_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_Eflow_phi_EndcapRegion;
   Float_t         MET_Egamma20_CellOut_Eflow_etx_ForwardReg;
   Float_t         MET_Egamma20_CellOut_Eflow_ety_ForwardReg;
   Float_t         MET_Egamma20_CellOut_Eflow_sumet_ForwardReg;
   Float_t         MET_Egamma20_CellOut_Eflow_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_etx;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_ety;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_sumet;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_CellOut_Eflow_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_etx;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_ety;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_sumet;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_CellOut_Eflow_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_etx;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_ety;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_sumet;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_etx_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_ety_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_phi_CentralReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_CellOut_Eflow_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_etx;
   Float_t         MET_Simplified20_Muon_Isol_Muid_ety;
   Float_t         MET_Simplified20_Muon_Isol_Muid_sumet;
   Float_t         MET_Simplified20_Muon_Isol_Muid_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Muid_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Muid_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Muid_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Isol_Muid_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_Isol_Muid_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Isol_Muid_phi_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_etx;
   Float_t         MET_Egamma20_Muon_Isol_Muid_ety;
   Float_t         MET_Egamma20_Muon_Isol_Muid_sumet;
   Float_t         MET_Egamma20_Muon_Isol_Muid_etx_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_ety_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_sumet_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_phi_CentralReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Isol_Muid_etx_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_ety_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma20_Muon_Isol_Muid_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_etx;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_ety;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_sumet;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Isol_Muid_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_etx;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_ety;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_sumet;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Isol_Muid_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_etx;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_ety;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_sumet;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_etx_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_ety_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_phi_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Isol_Muid_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_etx;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_ety;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_sumet;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_NonIsol_Muid_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_phi_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_etx;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_ety;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_sumet;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_etx_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_ety_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_sumet_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_phi_CentralReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_etx_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_ety_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma20_Muon_NonIsol_Muid_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_etx;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_ety;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_NonIsol_Muid_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_etx;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_ety;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_NonIsol_Muid_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_etx;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_ety;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_sumet;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_etx_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_ety_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_phi_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_NonIsol_Muid_phi_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_etx;
   Float_t         MET_Simplified20_Muon_Total_Muid_ety;
   Float_t         MET_Simplified20_Muon_Total_Muid_sumet;
   Float_t         MET_Simplified20_Muon_Total_Muid_etx_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_ety_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_sumet_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_phi_CentralReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_etx_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Muid_ety_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Muid_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Muid_phi_EndcapRegion;
   Float_t         MET_Simplified20_Muon_Total_Muid_etx_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_ety_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_sumet_ForwardReg;
   Float_t         MET_Simplified20_Muon_Total_Muid_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_etx;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_ety;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Muon_Total_Muid_phi_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_etx;
   Float_t         MET_Egamma20_Muon_Total_Muid_ety;
   Float_t         MET_Egamma20_Muon_Total_Muid_sumet;
   Float_t         MET_Egamma20_Muon_Total_Muid_etx_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_ety_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_sumet_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_phi_CentralReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma20_Muon_Total_Muid_etx_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_ety_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma20_Muon_Total_Muid_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_etx;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_ety;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_sumet;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_Muon_Total_Muid_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_etx;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_ety;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_sumet;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_Muon_Total_Muid_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_etx;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_ety;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_sumet;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_etx_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_ety_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_phi_CentralReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_Muon_Total_Muid_phi_ForwardReg;
   Float_t         MET_Simplified20_Track_etx;
   Float_t         MET_Simplified20_Track_ety;
   Float_t         MET_Simplified20_Track_sumet;
   Float_t         MET_Simplified20_Track_etx_CentralReg;
   Float_t         MET_Simplified20_Track_ety_CentralReg;
   Float_t         MET_Simplified20_Track_sumet_CentralReg;
   Float_t         MET_Simplified20_Track_phi_CentralReg;
   Float_t         MET_Simplified20_Track_etx_EndcapRegion;
   Float_t         MET_Simplified20_Track_ety_EndcapRegion;
   Float_t         MET_Simplified20_Track_sumet_EndcapRegion;
   Float_t         MET_Simplified20_Track_phi_EndcapRegion;
   Float_t         MET_Simplified20_Track_etx_ForwardReg;
   Float_t         MET_Simplified20_Track_ety_ForwardReg;
   Float_t         MET_Simplified20_Track_sumet_ForwardReg;
   Float_t         MET_Simplified20_Track_phi_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Track_etx;
   Float_t         MET_Simplified20withTightPhotons_Track_ety;
   Float_t         MET_Simplified20withTightPhotons_Track_sumet;
   Float_t         MET_Simplified20withTightPhotons_Track_etx_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Track_ety_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Track_sumet_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Track_phi_CentralReg;
   Float_t         MET_Simplified20withTightPhotons_Track_etx_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Track_ety_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Track_sumet_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Track_phi_EndcapRegion;
   Float_t         MET_Simplified20withTightPhotons_Track_etx_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Track_ety_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Track_sumet_ForwardReg;
   Float_t         MET_Simplified20withTightPhotons_Track_phi_ForwardReg;
   Float_t         MET_Egamma20_Track_etx;
   Float_t         MET_Egamma20_Track_ety;
   Float_t         MET_Egamma20_Track_sumet;
   Float_t         MET_Egamma20_Track_etx_CentralReg;
   Float_t         MET_Egamma20_Track_ety_CentralReg;
   Float_t         MET_Egamma20_Track_sumet_CentralReg;
   Float_t         MET_Egamma20_Track_phi_CentralReg;
   Float_t         MET_Egamma20_Track_etx_EndcapRegion;
   Float_t         MET_Egamma20_Track_ety_EndcapRegion;
   Float_t         MET_Egamma20_Track_sumet_EndcapRegion;
   Float_t         MET_Egamma20_Track_phi_EndcapRegion;
   Float_t         MET_Egamma20_Track_etx_ForwardReg;
   Float_t         MET_Egamma20_Track_ety_ForwardReg;
   Float_t         MET_Egamma20_Track_sumet_ForwardReg;
   Float_t         MET_Egamma20_Track_phi_ForwardReg;
   Float_t         MET_Egamma20NoTau_Track_etx;
   Float_t         MET_Egamma20NoTau_Track_ety;
   Float_t         MET_Egamma20NoTau_Track_sumet;
   Float_t         MET_Egamma20NoTau_Track_etx_CentralReg;
   Float_t         MET_Egamma20NoTau_Track_ety_CentralReg;
   Float_t         MET_Egamma20NoTau_Track_sumet_CentralReg;
   Float_t         MET_Egamma20NoTau_Track_phi_CentralReg;
   Float_t         MET_Egamma20NoTau_Track_etx_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Track_ety_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Track_sumet_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Track_phi_EndcapRegion;
   Float_t         MET_Egamma20NoTau_Track_etx_ForwardReg;
   Float_t         MET_Egamma20NoTau_Track_ety_ForwardReg;
   Float_t         MET_Egamma20NoTau_Track_sumet_ForwardReg;
   Float_t         MET_Egamma20NoTau_Track_phi_ForwardReg;
   Float_t         MET_Egamma10NoTau_Track_etx;
   Float_t         MET_Egamma10NoTau_Track_ety;
   Float_t         MET_Egamma10NoTau_Track_sumet;
   Float_t         MET_Egamma10NoTau_Track_etx_CentralReg;
   Float_t         MET_Egamma10NoTau_Track_ety_CentralReg;
   Float_t         MET_Egamma10NoTau_Track_sumet_CentralReg;
   Float_t         MET_Egamma10NoTau_Track_phi_CentralReg;
   Float_t         MET_Egamma10NoTau_Track_etx_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Track_ety_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Track_sumet_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Track_phi_EndcapRegion;
   Float_t         MET_Egamma10NoTau_Track_etx_ForwardReg;
   Float_t         MET_Egamma10NoTau_Track_ety_ForwardReg;
   Float_t         MET_Egamma10NoTau_Track_sumet_ForwardReg;
   Float_t         MET_Egamma10NoTau_Track_phi_ForwardReg;
   Float_t         MET_LArFixSimp20_Track_etx;
   Float_t         MET_LArFixSimp20_Track_ety;
   Float_t         MET_LArFixSimp20_Track_sumet;
   Float_t         MET_LArFixSimp20_Track_etx_CentralReg;
   Float_t         MET_LArFixSimp20_Track_ety_CentralReg;
   Float_t         MET_LArFixSimp20_Track_sumet_CentralReg;
   Float_t         MET_LArFixSimp20_Track_phi_CentralReg;
   Float_t         MET_LArFixSimp20_Track_etx_EndcapRegion;
   Float_t         MET_LArFixSimp20_Track_ety_EndcapRegion;
   Float_t         MET_LArFixSimp20_Track_sumet_EndcapRegion;
   Float_t         MET_LArFixSimp20_Track_phi_EndcapRegion;
   Float_t         MET_LArFixSimp20_Track_etx_ForwardReg;
   Float_t         MET_LArFixSimp20_Track_ety_ForwardReg;
   Float_t         MET_LArFixSimp20_Track_sumet_ForwardReg;
   Float_t         MET_LArFixSimp20_Track_phi_ForwardReg;
   Int_t           el_MET_Simplified20_n;
   vector<vector<float> > *el_MET_Simplified20_wpx;
   vector<vector<float> > *el_MET_Simplified20_wpy;
   vector<vector<float> > *el_MET_Simplified20_wet;
   vector<vector<unsigned int> > *el_MET_Simplified20_statusWord;
   Int_t           mu_staco_MET_Simplified20_n;
   vector<vector<float> > *mu_staco_MET_Simplified20_wpx;
   vector<vector<float> > *mu_staco_MET_Simplified20_wpy;
   vector<vector<float> > *mu_staco_MET_Simplified20_wet;
   vector<vector<unsigned int> > *mu_staco_MET_Simplified20_statusWord;
   Int_t           mu_muid_MET_Simplified20_n;
   vector<vector<float> > *mu_muid_MET_Simplified20_wpx;
   vector<vector<float> > *mu_muid_MET_Simplified20_wpy;
   vector<vector<float> > *mu_muid_MET_Simplified20_wet;
   vector<vector<unsigned int> > *mu_muid_MET_Simplified20_statusWord;
   Int_t           jet_AntiKt4TopoNewEM_MET_Simplified20_n;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wpx;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wpy;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20_wet;
   vector<vector<unsigned int> > *jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord;
   Int_t           el_MET_Simplified20withTightPhotons_n;
   vector<vector<float> > *el_MET_Simplified20withTightPhotons_wpx;
   vector<vector<float> > *el_MET_Simplified20withTightPhotons_wpy;
   vector<vector<float> > *el_MET_Simplified20withTightPhotons_wet;
   vector<vector<unsigned int> > *el_MET_Simplified20withTightPhotons_statusWord;
   Int_t           ph_MET_Simplified20withTightPhotons_n;
   vector<vector<float> > *ph_MET_Simplified20withTightPhotons_wpx;
   vector<vector<float> > *ph_MET_Simplified20withTightPhotons_wpy;
   vector<vector<float> > *ph_MET_Simplified20withTightPhotons_wet;
   vector<vector<unsigned int> > *ph_MET_Simplified20withTightPhotons_statusWord;
   Int_t           mu_staco_MET_Simplified20withTightPhotons_n;
   vector<vector<float> > *mu_staco_MET_Simplified20withTightPhotons_wpx;
   vector<vector<float> > *mu_staco_MET_Simplified20withTightPhotons_wpy;
   vector<vector<float> > *mu_staco_MET_Simplified20withTightPhotons_wet;
   vector<vector<unsigned int> > *mu_staco_MET_Simplified20withTightPhotons_statusWord;
   Int_t           mu_muid_MET_Simplified20withTightPhotons_n;
   vector<vector<float> > *mu_muid_MET_Simplified20withTightPhotons_wpx;
   vector<vector<float> > *mu_muid_MET_Simplified20withTightPhotons_wpy;
   vector<vector<float> > *mu_muid_MET_Simplified20withTightPhotons_wet;
   vector<vector<unsigned int> > *mu_muid_MET_Simplified20withTightPhotons_statusWord;
   Int_t           jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_n;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_wpx;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_wpy;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_wet;
   vector<vector<unsigned int> > *jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_statusWord;
   Int_t           el_MET_Egamma20_n;
   vector<vector<float> > *el_MET_Egamma20_wpx;
   vector<vector<float> > *el_MET_Egamma20_wpy;
   vector<vector<float> > *el_MET_Egamma20_wet;
   vector<vector<unsigned int> > *el_MET_Egamma20_statusWord;
   Int_t           ph_MET_Egamma20_n;
   vector<vector<float> > *ph_MET_Egamma20_wpx;
   vector<vector<float> > *ph_MET_Egamma20_wpy;
   vector<vector<float> > *ph_MET_Egamma20_wet;
   vector<vector<unsigned int> > *ph_MET_Egamma20_statusWord;
   Int_t           mu_staco_MET_Egamma20_n;
   vector<vector<float> > *mu_staco_MET_Egamma20_wpx;
   vector<vector<float> > *mu_staco_MET_Egamma20_wpy;
   vector<vector<float> > *mu_staco_MET_Egamma20_wet;
   vector<vector<unsigned int> > *mu_staco_MET_Egamma20_statusWord;
   Int_t           mu_muid_MET_Egamma20_n;
   vector<vector<float> > *mu_muid_MET_Egamma20_wpx;
   vector<vector<float> > *mu_muid_MET_Egamma20_wpy;
   vector<vector<float> > *mu_muid_MET_Egamma20_wet;
   vector<vector<unsigned int> > *mu_muid_MET_Egamma20_statusWord;
   Int_t           tau_MET_Egamma20_n;
   vector<vector<float> > *tau_MET_Egamma20_wpx;
   vector<vector<float> > *tau_MET_Egamma20_wpy;
   vector<vector<float> > *tau_MET_Egamma20_wet;
   vector<vector<unsigned int> > *tau_MET_Egamma20_statusWord;
   Int_t           jet_AntiKt4LCTopo_MET_Egamma20_n;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma20_wpx;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma20_wpy;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma20_wet;
   vector<vector<unsigned int> > *jet_AntiKt4LCTopo_MET_Egamma20_statusWord;
   Int_t           el_MET_Egamma20NoTau_n;
   vector<vector<float> > *el_MET_Egamma20NoTau_wpx;
   vector<vector<float> > *el_MET_Egamma20NoTau_wpy;
   vector<vector<float> > *el_MET_Egamma20NoTau_wet;
   vector<vector<unsigned int> > *el_MET_Egamma20NoTau_statusWord;
   Int_t           ph_MET_Egamma20NoTau_n;
   vector<vector<float> > *ph_MET_Egamma20NoTau_wpx;
   vector<vector<float> > *ph_MET_Egamma20NoTau_wpy;
   vector<vector<float> > *ph_MET_Egamma20NoTau_wet;
   vector<vector<unsigned int> > *ph_MET_Egamma20NoTau_statusWord;
   Int_t           mu_staco_MET_Egamma20NoTau_n;
   vector<vector<float> > *mu_staco_MET_Egamma20NoTau_wpx;
   vector<vector<float> > *mu_staco_MET_Egamma20NoTau_wpy;
   vector<vector<float> > *mu_staco_MET_Egamma20NoTau_wet;
   vector<vector<unsigned int> > *mu_staco_MET_Egamma20NoTau_statusWord;
   Int_t           mu_muid_MET_Egamma20NoTau_n;
   vector<vector<float> > *mu_muid_MET_Egamma20NoTau_wpx;
   vector<vector<float> > *mu_muid_MET_Egamma20NoTau_wpy;
   vector<vector<float> > *mu_muid_MET_Egamma20NoTau_wet;
   vector<vector<unsigned int> > *mu_muid_MET_Egamma20NoTau_statusWord;
   Int_t           jet_AntiKt4LCTopo_MET_Egamma20NoTau_n;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma20NoTau_wpx;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma20NoTau_wpy;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma20NoTau_wet;
   vector<vector<unsigned int> > *jet_AntiKt4LCTopo_MET_Egamma20NoTau_statusWord;
   Int_t           el_MET_Egamma10NoTau_n;
   vector<vector<float> > *el_MET_Egamma10NoTau_wpx;
   vector<vector<float> > *el_MET_Egamma10NoTau_wpy;
   vector<vector<float> > *el_MET_Egamma10NoTau_wet;
   vector<vector<unsigned int> > *el_MET_Egamma10NoTau_statusWord;
   Int_t           ph_MET_Egamma10NoTau_n;
   vector<vector<float> > *ph_MET_Egamma10NoTau_wpx;
   vector<vector<float> > *ph_MET_Egamma10NoTau_wpy;
   vector<vector<float> > *ph_MET_Egamma10NoTau_wet;
   vector<vector<unsigned int> > *ph_MET_Egamma10NoTau_statusWord;
   Int_t           mu_staco_MET_Egamma10NoTau_n;
   vector<vector<float> > *mu_staco_MET_Egamma10NoTau_wpx;
   vector<vector<float> > *mu_staco_MET_Egamma10NoTau_wpy;
   vector<vector<float> > *mu_staco_MET_Egamma10NoTau_wet;
   vector<vector<unsigned int> > *mu_staco_MET_Egamma10NoTau_statusWord;
   Int_t           mu_muid_MET_Egamma10NoTau_n;
   vector<vector<float> > *mu_muid_MET_Egamma10NoTau_wpx;
   vector<vector<float> > *mu_muid_MET_Egamma10NoTau_wpy;
   vector<vector<float> > *mu_muid_MET_Egamma10NoTau_wet;
   vector<vector<unsigned int> > *mu_muid_MET_Egamma10NoTau_statusWord;
   Int_t           jet_AntiKt4LCTopo_MET_Egamma10NoTau_n;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpx;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpy;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_Egamma10NoTau_wet;
   vector<vector<unsigned int> > *jet_AntiKt4LCTopo_MET_Egamma10NoTau_statusWord;
   Int_t           el_MET_n;
   vector<vector<float> > *el_MET_wpx;
   vector<vector<float> > *el_MET_wpy;
   vector<vector<float> > *el_MET_wet;
   vector<vector<unsigned int> > *el_MET_statusWord;
   Int_t           ph_MET_n;
   vector<vector<float> > *ph_MET_wpx;
   vector<vector<float> > *ph_MET_wpy;
   vector<vector<float> > *ph_MET_wet;
   vector<vector<unsigned int> > *ph_MET_statusWord;
   Int_t           mu_staco_MET_n;
   vector<vector<float> > *mu_staco_MET_wpx;
   vector<vector<float> > *mu_staco_MET_wpy;
   vector<vector<float> > *mu_staco_MET_wet;
   vector<vector<unsigned int> > *mu_staco_MET_statusWord;
   Int_t           mu_muid_MET_n;
   vector<vector<float> > *mu_muid_MET_wpx;
   vector<vector<float> > *mu_muid_MET_wpy;
   vector<vector<float> > *mu_muid_MET_wet;
   vector<vector<unsigned int> > *mu_muid_MET_statusWord;
   Int_t           tau_MET_n;
   vector<vector<float> > *tau_MET_wpx;
   vector<vector<float> > *tau_MET_wpy;
   vector<vector<float> > *tau_MET_wet;
   vector<vector<unsigned int> > *tau_MET_statusWord;
   Int_t           jet_AntiKt4LCTopo_MET_n;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_wpx;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_wpy;
   vector<vector<float> > *jet_AntiKt4LCTopo_MET_wet;
   vector<vector<unsigned int> > *jet_AntiKt4LCTopo_MET_statusWord;
   Int_t           cl_MET_n;
   vector<vector<float> > *cl_MET_wpx;
   vector<vector<float> > *cl_MET_wpy;
   vector<vector<float> > *cl_MET_wet;
   vector<vector<unsigned int> > *cl_MET_statusWord;
   Int_t           trk_MET_n;
   vector<vector<float> > *trk_MET_wpx;
   vector<vector<float> > *trk_MET_wpy;
   vector<vector<float> > *trk_MET_wet;
   vector<vector<unsigned int> > *trk_MET_statusWord;
   Int_t           jet_AntiKt4TopoNewEM_n;
   vector<float>   *jet_AntiKt4TopoNewEM_E;
   vector<float>   *jet_AntiKt4TopoNewEM_pt;
   vector<float>   *jet_AntiKt4TopoNewEM_m;
   vector<float>   *jet_AntiKt4TopoNewEM_eta;
   vector<float>   *jet_AntiKt4TopoNewEM_phi;
   vector<float>   *jet_AntiKt4TopoNewEM_EtaOrigin;
   vector<float>   *jet_AntiKt4TopoNewEM_PhiOrigin;
   vector<float>   *jet_AntiKt4TopoNewEM_MOrigin;
   vector<float>   *jet_AntiKt4TopoNewEM_EtaOriginEM;
   vector<float>   *jet_AntiKt4TopoNewEM_PhiOriginEM;
   vector<float>   *jet_AntiKt4TopoNewEM_MOriginEM;
   vector<float>   *jet_AntiKt4TopoNewEM_WIDTH;
   vector<float>   *jet_AntiKt4TopoNewEM_n90;
   vector<float>   *jet_AntiKt4TopoNewEM_Timing;
   vector<float>   *jet_AntiKt4TopoNewEM_LArQuality;
   vector<float>   *jet_AntiKt4TopoNewEM_nTrk;
   vector<float>   *jet_AntiKt4TopoNewEM_sumPtTrk;
   vector<float>   *jet_AntiKt4TopoNewEM_OriginIndex;
   vector<float>   *jet_AntiKt4TopoNewEM_HECQuality;
   vector<float>   *jet_AntiKt4TopoNewEM_NegativeE;
   vector<float>   *jet_AntiKt4TopoNewEM_AverageLArQF;
   vector<float>   *jet_AntiKt4TopoNewEM_YFlip12;
   vector<float>   *jet_AntiKt4TopoNewEM_YFlip23;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_CELL;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_DOTX;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_JET;
   vector<float>   *jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL;
   vector<float>   *jet_AntiKt4TopoNewEM_ENG_BAD_CELLS;
   vector<float>   *jet_AntiKt4TopoNewEM_N_BAD_CELLS;
   vector<float>   *jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR;
   vector<float>   *jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E;
   vector<float>   *jet_AntiKt4TopoNewEM_NumTowers;
   vector<int>     *jet_AntiKt4TopoNewEM_SamplingMax;
   vector<float>   *jet_AntiKt4TopoNewEM_fracSamplingMax;
   vector<float>   *jet_AntiKt4TopoNewEM_hecf;
   vector<float>   *jet_AntiKt4TopoNewEM_tgap3f;
   vector<int>     *jet_AntiKt4TopoNewEM_isUgly;
   vector<int>     *jet_AntiKt4TopoNewEM_isBadLoose;
   vector<int>     *jet_AntiKt4TopoNewEM_isBadMedium;
   vector<int>     *jet_AntiKt4TopoNewEM_isBadTight;
   vector<float>   *jet_AntiKt4TopoNewEM_emfrac;
   vector<float>   *jet_AntiKt4TopoNewEM_Offset;
   vector<float>   *jet_AntiKt4TopoNewEM_EMJES;
   vector<float>   *jet_AntiKt4TopoNewEM_EMJES_EtaCorr;
   vector<float>   *jet_AntiKt4TopoNewEM_EMJESnooffset;
   vector<float>   *jet_AntiKt4TopoNewEM_GCWJES;
   vector<float>   *jet_AntiKt4TopoNewEM_GCWJES_EtaCorr;
   vector<float>   *jet_AntiKt4TopoNewEM_CB;
   vector<float>   *jet_AntiKt4TopoNewEM_LCJES;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_E;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_pt;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_m;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_eta;
   vector<float>   *jet_AntiKt4TopoNewEM_emscale_phi;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtx_x;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtx_y;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtx_z;
   vector<float>   *jet_AntiKt4TopoNewEM_jvtxf;
   vector<float>   *jet_AntiKt4TopoNewEM_GSCFactorF;
   vector<float>   *jet_AntiKt4TopoNewEM_WidthFraction;
   vector<float>   *jet_AntiKt4TopoNewEM_e_PreSamplerB;
   vector<float>   *jet_AntiKt4TopoNewEM_e_EMB1;
   vector<float>   *jet_AntiKt4TopoNewEM_e_EMB2;
   vector<float>   *jet_AntiKt4TopoNewEM_e_EMB3;
   vector<float>   *jet_AntiKt4TopoNewEM_e_PreSamplerE;
   vector<float>   *jet_AntiKt4TopoNewEM_e_EME1;
   vector<float>   *jet_AntiKt4TopoNewEM_e_EME2;
   vector<float>   *jet_AntiKt4TopoNewEM_e_EME3;
   vector<float>   *jet_AntiKt4TopoNewEM_e_HEC0;
   vector<float>   *jet_AntiKt4TopoNewEM_e_HEC1;
   vector<float>   *jet_AntiKt4TopoNewEM_e_HEC2;
   vector<float>   *jet_AntiKt4TopoNewEM_e_HEC3;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileBar0;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileBar1;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileBar2;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileGap1;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileGap2;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileGap3;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileExt0;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileExt1;
   vector<float>   *jet_AntiKt4TopoNewEM_e_TileExt2;
   vector<float>   *jet_AntiKt4TopoNewEM_e_FCAL0;
   vector<float>   *jet_AntiKt4TopoNewEM_e_FCAL1;
   vector<float>   *jet_AntiKt4TopoNewEM_e_FCAL2;
   vector<int>     *jet_AntiKt4TopoNewEM_Nconst;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_ptconst_default;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_econst_default;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_etaconst_default;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_phiconst_default;
   vector<vector<float> > *jet_AntiKt4TopoNewEM_weightconst_default;
   vector<float>   *jet_AntiKt4TopoNewEM_constscale_E;
   vector<float>   *jet_AntiKt4TopoNewEM_constscale_pt;
   vector<float>   *jet_AntiKt4TopoNewEM_constscale_m;
   vector<float>   *jet_AntiKt4TopoNewEM_constscale_eta;
   vector<float>   *jet_AntiKt4TopoNewEM_constscale_phi;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_Comb;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_IP2D;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_IP3D;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SV0;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SV1;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SV2;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_JetProb;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_weight_GbbNN;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_truth_label;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_dRminToB;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_dRminToC;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_dRminToT;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_vx_x;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_vx_y;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_truth_vx_z;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_ip2d_pu;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_ip2d_pb;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_ip2d_isValid;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_ip2d_ntrk;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_ip3d_pu;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_ip3d_pb;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_ip3d_isValid;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_ip3d_ntrk;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv1_pu;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv1_pb;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv1_isValid;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv2_pu;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv2_pb;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv2_isValid;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_pu;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_pb;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_pc;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfit_isValid;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_isValid;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfit_nvtx;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfit_nvtx1t;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_jfit_ntrkAtVx;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_efrc;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_mass;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_sig3d;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_deltaPhi;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_jfit_deltaEta;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_svp_isValid;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_svp_ntrkv;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_svp_ntrkj;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_svp_n2t;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_mass;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_efrc;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_x;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_y;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_z;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_err_x;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_err_y;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_err_z;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_cov_xy;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_cov_xz;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_cov_yz;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_svp_chi2;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_svp_ndof;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_svp_ntrk;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv0p_isValid;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv0p_ntrkv;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv0p_ntrkj;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv0p_n2t;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_mass;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_efrc;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_x;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_y;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_z;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_err_x;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_err_y;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_err_z;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_cov_xy;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_cov_xz;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_cov_yz;
   vector<float>   *jet_AntiKt4TopoNewEM_flavor_component_sv0p_chi2;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv0p_ndof;
   vector<int>     *jet_AntiKt4TopoNewEM_flavor_component_sv0p_ntrk;
   vector<float>   *jet_AntiKt4TopoNewEM_el_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_el_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_mu_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_mu_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_L1_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_L1_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_L2_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_L2_matched;
   vector<float>   *jet_AntiKt4TopoNewEM_EF_dr;
   vector<int>     *jet_AntiKt4TopoNewEM_EF_matched;
   vector<int>     *jet_AntiKt4TopoNewEM_bbDecision;
   vector<float>   *jet_AntiKt4TopoNewEM_SmearingFactor;
   Int_t           jet_AntiKt4TopoEMLArFix_n;
   vector<float>   *jet_AntiKt4TopoEMLArFix_E;
   vector<float>   *jet_AntiKt4TopoEMLArFix_pt;
   vector<float>   *jet_AntiKt4TopoEMLArFix_m;
   vector<float>   *jet_AntiKt4TopoEMLArFix_eta;
   vector<float>   *jet_AntiKt4TopoEMLArFix_phi;
   vector<float>   *jet_AntiKt4TopoEMLArFix_el_dr;
   vector<int>     *jet_AntiKt4TopoEMLArFix_el_matched;
   vector<float>   *jet_AntiKt4TopoEMLArFix_mu_dr;
   vector<int>     *jet_AntiKt4TopoEMLArFix_mu_matched;
   vector<float>   *jet_AntiKt4TopoEMLArFix_L1_dr;
   vector<int>     *jet_AntiKt4TopoEMLArFix_L1_matched;
   vector<float>   *jet_AntiKt4TopoEMLArFix_L2_dr;
   vector<int>     *jet_AntiKt4TopoEMLArFix_L2_matched;
   vector<float>   *jet_AntiKt4TopoEMLArFix_EF_dr;
   vector<int>     *jet_AntiKt4TopoEMLArFix_EF_matched;
   Int_t           jet_AntiKt4LCTopo_n;
   vector<float>   *jet_AntiKt4LCTopo_E;
   vector<float>   *jet_AntiKt4LCTopo_pt;
   vector<float>   *jet_AntiKt4LCTopo_m;
   vector<float>   *jet_AntiKt4LCTopo_eta;
   vector<float>   *jet_AntiKt4LCTopo_phi;
   vector<float>   *jet_AntiKt4LCTopo_el_dr;
   vector<int>     *jet_AntiKt4LCTopo_el_matched;
   vector<float>   *jet_AntiKt4LCTopo_mu_dr;
   vector<int>     *jet_AntiKt4LCTopo_mu_matched;
   vector<float>   *jet_AntiKt4LCTopo_L1_dr;
   vector<int>     *jet_AntiKt4LCTopo_L1_matched;
   vector<float>   *jet_AntiKt4LCTopo_L2_dr;
   vector<int>     *jet_AntiKt4LCTopo_L2_matched;
   vector<float>   *jet_AntiKt4LCTopo_EF_dr;
   vector<int>     *jet_AntiKt4LCTopo_EF_matched;
   Int_t           vx_n;
   vector<float>   *vx_x;
   vector<float>   *vx_y;
   vector<float>   *vx_z;
   vector<float>   *vx_px;
   vector<float>   *vx_py;
   vector<float>   *vx_pz;
   vector<float>   *vx_E;
   vector<float>   *vx_m;
   vector<int>     *vx_nTracks;
   vector<float>   *vx_sumPt;
   Int_t           trk_n;
   vector<float>   *trk_pt;
   vector<float>   *trk_eta;
   vector<float>   *trk_IPEstimate_d0_biased_wrtPV;
   vector<float>   *trk_IPEstimate_z0_biased_wrtPV;
   vector<float>   *trk_IPEstimate_err_d0_biased_wrtPV;
   vector<float>   *trk_IPEstimate_err_z0_biased_wrtPV;
   vector<float>   *trk_IPEstimate_errPV_d0_biased_wrtPV;
   vector<float>   *trk_IPEstimate_errPV_z0_biased_wrtPV;
   vector<float>   *trk_IPEstimate_d0_unbiased_wrtPV;
   vector<float>   *trk_IPEstimate_z0_unbiased_wrtPV;
   vector<float>   *trk_IPEstimate_err_d0_unbiased_wrtPV;
   vector<float>   *trk_IPEstimate_err_z0_unbiased_wrtPV;
   vector<float>   *trk_IPEstimate_errPV_d0_unbiased_wrtPV;
   vector<float>   *trk_IPEstimate_errPV_z0_unbiased_wrtPV;
   vector<float>   *trk_d0_wrtPV;
   vector<float>   *trk_z0_wrtPV;
   vector<float>   *trk_phi_wrtPV;
   vector<float>   *trk_theta_wrtPV;
   vector<float>   *trk_qoverp_wrtPV;
   vector<float>   *trk_cov_d0_wrtPV;
   vector<float>   *trk_cov_z0_wrtPV;
   vector<float>   *trk_cov_phi_wrtPV;
   vector<float>   *trk_cov_theta_wrtPV;
   vector<float>   *trk_cov_qoverp_wrtPV;
   vector<float>   *trk_chi2;
   vector<int>     *trk_ndof;
   vector<int>     *trk_nBLHits;
   vector<int>     *trk_nPixHits;
   vector<int>     *trk_nSCTHits;
   vector<int>     *trk_nTRTHits;
   vector<int>     *trk_nTRTHighTHits;
   vector<int>     *trk_nPixHoles;
   vector<int>     *trk_nSCTHoles;
   vector<int>     *trk_nTRTHoles;
   vector<int>     *trk_expectBLayerHit;
   vector<int>     *trk_nMDTHits;
   vector<int>     *trk_nCSCEtaHits;
   vector<int>     *trk_nCSCPhiHits;
   vector<int>     *trk_nRPCEtaHits;
   vector<int>     *trk_nRPCPhiHits;
   vector<int>     *trk_nTGCEtaHits;
   vector<int>     *trk_nTGCPhiHits;
   vector<int>     *trk_nHits;
   vector<int>     *trk_nHoles;
   vector<int>     *trk_hitPattern;
   vector<float>   *trk_TRTHighTHitsRatio;
   vector<float>   *trk_TRTHighTOutliersRatio;
   vector<float>   *trk_mc_probability;
   vector<int>     *trk_mc_barcode;
   vector<float>   *trk_cone20_trackIso;
   vector<float>   *trk_cone20_caloIso;
   vector<int>     *trk_cone20_nTrackIso;
   vector<float>   *trk_cone30_trackIso;
   vector<float>   *trk_cone30_caloIso;
   vector<int>     *trk_cone30_nTrackIso;
   vector<float>   *trk_cone40_trackIso;
   vector<float>   *trk_cone40_caloIso;
   vector<int>     *trk_cone40_nTrackIso;
   Int_t           top_hfor_type;
   Int_t           mcevt_n;
   vector<int>     *mcevt_signal_process_id;
   vector<int>     *mcevt_event_number;
   vector<double>  *mcevt_event_scale;
   vector<double>  *mcevt_alphaQCD;
   vector<double>  *mcevt_alphaQED;
   vector<int>     *mcevt_pdf_id1;
   vector<int>     *mcevt_pdf_id2;
   vector<double>  *mcevt_pdf_x1;
   vector<double>  *mcevt_pdf_x2;
   vector<double>  *mcevt_pdf_scale;
   vector<double>  *mcevt_pdf1;
   vector<double>  *mcevt_pdf2;
   vector<vector<double> > *mcevt_weight;
   vector<int>     *mcevt_nparticle;
   vector<short>   *mcevt_pileUpType;
   Int_t           mc_n;
   vector<float>   *mc_pt;
   vector<float>   *mc_m;
   vector<float>   *mc_eta;
   vector<float>   *mc_phi;
   vector<int>     *mc_status;
   vector<int>     *mc_barcode;
   vector<vector<int> > *mc_parents;
   vector<vector<int> > *mc_children;
   vector<int>     *mc_pdgId;
   vector<float>   *mc_charge;
   vector<float>   *mc_vx_x;
   vector<float>   *mc_vx_y;
   vector<float>   *mc_vx_z;
   vector<int>     *mc_vx_barcode;
   vector<vector<int> > *mc_child_index;
   vector<vector<int> > *mc_parent_index;
   Int_t           mcVx_n;
   vector<float>   *mcVx_x;
   vector<float>   *mcVx_y;
   vector<float>   *mcVx_z;
   Int_t           muonTruth_n;
   vector<float>   *muonTruth_pt;
   vector<float>   *muonTruth_m;
   vector<float>   *muonTruth_eta;
   vector<float>   *muonTruth_phi;
   vector<float>   *muonTruth_charge;
   vector<int>     *muonTruth_PDGID;
   vector<int>     *muonTruth_barcode;
   vector<int>     *muonTruth_type;
   vector<int>     *muonTruth_origin;
   Int_t           jet_AntiKt4TruthJets_n;
   vector<float>   *jet_AntiKt4TruthJets_E;
   vector<float>   *jet_AntiKt4TruthJets_pt;
   vector<float>   *jet_AntiKt4TruthJets_m;
   vector<float>   *jet_AntiKt4TruthJets_eta;
   vector<float>   *jet_AntiKt4TruthJets_phi;
   vector<float>   *jet_AntiKt4TruthJets_EtaOrigin;
   vector<float>   *jet_AntiKt4TruthJets_PhiOrigin;
   vector<float>   *jet_AntiKt4TruthJets_MOrigin;
   vector<float>   *jet_AntiKt4TruthJets_EtaOriginEM;
   vector<float>   *jet_AntiKt4TruthJets_PhiOriginEM;
   vector<float>   *jet_AntiKt4TruthJets_MOriginEM;
   vector<float>   *jet_AntiKt4TruthJets_WIDTH;
   vector<float>   *jet_AntiKt4TruthJets_n90;
   vector<float>   *jet_AntiKt4TruthJets_Timing;
   vector<float>   *jet_AntiKt4TruthJets_LArQuality;
   vector<float>   *jet_AntiKt4TruthJets_nTrk;
   vector<float>   *jet_AntiKt4TruthJets_sumPtTrk;
   vector<float>   *jet_AntiKt4TruthJets_OriginIndex;
   vector<float>   *jet_AntiKt4TruthJets_HECQuality;
   vector<float>   *jet_AntiKt4TruthJets_NegativeE;
   vector<float>   *jet_AntiKt4TruthJets_AverageLArQF;
   vector<float>   *jet_AntiKt4TruthJets_YFlip12;
   vector<float>   *jet_AntiKt4TruthJets_YFlip23;
   vector<float>   *jet_AntiKt4TruthJets_BCH_CORR_CELL;
   vector<float>   *jet_AntiKt4TruthJets_BCH_CORR_DOTX;
   vector<float>   *jet_AntiKt4TruthJets_BCH_CORR_JET;
   vector<float>   *jet_AntiKt4TruthJets_BCH_CORR_JET_FORCELL;
   vector<float>   *jet_AntiKt4TruthJets_ENG_BAD_CELLS;
   vector<float>   *jet_AntiKt4TruthJets_N_BAD_CELLS;
   vector<float>   *jet_AntiKt4TruthJets_N_BAD_CELLS_CORR;
   vector<float>   *jet_AntiKt4TruthJets_BAD_CELLS_CORR_E;
   vector<float>   *jet_AntiKt4TruthJets_NumTowers;
   vector<int>     *jet_AntiKt4TruthJets_SamplingMax;
   vector<float>   *jet_AntiKt4TruthJets_fracSamplingMax;
   vector<float>   *jet_AntiKt4TruthJets_hecf;
   vector<float>   *jet_AntiKt4TruthJets_tgap3f;
   vector<int>     *jet_AntiKt4TruthJets_isUgly;
   vector<int>     *jet_AntiKt4TruthJets_isBadLoose;
   vector<int>     *jet_AntiKt4TruthJets_isBadMedium;
   vector<int>     *jet_AntiKt4TruthJets_isBadTight;
   vector<float>   *jet_AntiKt4TruthJets_emfrac;
   vector<float>   *jet_AntiKt4TruthJets_Offset;
   vector<float>   *jet_AntiKt4TruthJets_EMJES;
   vector<float>   *jet_AntiKt4TruthJets_EMJES_EtaCorr;
   vector<float>   *jet_AntiKt4TruthJets_EMJESnooffset;
   vector<float>   *jet_AntiKt4TruthJets_GCWJES;
   vector<float>   *jet_AntiKt4TruthJets_GCWJES_EtaCorr;
   vector<float>   *jet_AntiKt4TruthJets_CB;
   vector<float>   *jet_AntiKt4TruthJets_LCJES;
   vector<float>   *jet_AntiKt4TruthJets_emscale_E;
   vector<float>   *jet_AntiKt4TruthJets_emscale_pt;
   vector<float>   *jet_AntiKt4TruthJets_emscale_m;
   vector<float>   *jet_AntiKt4TruthJets_emscale_eta;
   vector<float>   *jet_AntiKt4TruthJets_emscale_phi;
   vector<int>     *jet_AntiKt4TruthJets_flavor_truth_label;
   vector<float>   *jet_AntiKt4TruthJets_flavor_truth_dRminToB;
   vector<float>   *jet_AntiKt4TruthJets_flavor_truth_dRminToC;
   vector<float>   *jet_AntiKt4TruthJets_flavor_truth_dRminToT;
   vector<int>     *jet_AntiKt4TruthJets_flavor_truth_BHadronpdg;
   vector<float>   *jet_AntiKt4TruthJets_flavor_truth_vx_x;
   vector<float>   *jet_AntiKt4TruthJets_flavor_truth_vx_y;
   vector<float>   *jet_AntiKt4TruthJets_flavor_truth_vx_z;
   vector<float>   *jet_AntiKt4TruthJets_el_dr;
   vector<int>     *jet_AntiKt4TruthJets_el_matched;
   vector<float>   *jet_AntiKt4TruthJets_mu_dr;
   vector<int>     *jet_AntiKt4TruthJets_mu_matched;
   vector<float>   *jet_AntiKt4TruthJets_L1_dr;
   vector<int>     *jet_AntiKt4TruthJets_L1_matched;
   vector<float>   *jet_AntiKt4TruthJets_L2_dr;
   vector<int>     *jet_AntiKt4TruthJets_L2_matched;
   vector<float>   *jet_AntiKt4TruthJets_EF_dr;
   vector<int>     *jet_AntiKt4TruthJets_EF_matched;
   Int_t           trig_L1_emtau_n;
   vector<float>   *trig_L1_emtau_eta;
   vector<float>   *trig_L1_emtau_phi;
   vector<vector<string> > *trig_L1_emtau_thrNames;
   vector<vector<float> > *trig_L1_emtau_thrValues;
   vector<float>   *trig_L1_emtau_core;
   vector<float>   *trig_L1_emtau_EMClus;
   vector<float>   *trig_L1_emtau_tauClus;
   vector<float>   *trig_L1_emtau_EMIsol;
   vector<float>   *trig_L1_emtau_hadIsol;
   vector<float>   *trig_L1_emtau_hadCore;
   vector<unsigned int> *trig_L1_emtau_thrPattern;
   Int_t           trig_L2_emcl_n;
   vector<unsigned int> *trig_L2_emcl_quality;
   vector<float>   *trig_L2_emcl_E;
   vector<float>   *trig_L2_emcl_Et;
   vector<float>   *trig_L2_emcl_eta;
   vector<float>   *trig_L2_emcl_phi;
   Int_t           trig_L2_trk_idscan_eGamma_n;
   vector<int>     *trig_L2_trk_idscan_eGamma_algorithmId;
   vector<short>   *trig_L2_trk_idscan_eGamma_trackStatus;
   vector<float>   *trig_L2_trk_idscan_eGamma_chi2Ndof;
   vector<float>   *trig_L2_trk_idscan_eGamma_param_a0;
   vector<float>   *trig_L2_trk_idscan_eGamma_param_z0;
   vector<float>   *trig_L2_trk_idscan_eGamma_param_phi0;
   vector<float>   *trig_L2_trk_idscan_eGamma_param_eta;
   vector<float>   *trig_L2_trk_idscan_eGamma_param_pt;
   Int_t           trig_L2_trk_sitrack_eGamma_n;
   vector<int>     *trig_L2_trk_sitrack_eGamma_algorithmId;
   vector<short>   *trig_L2_trk_sitrack_eGamma_trackStatus;
   vector<float>   *trig_L2_trk_sitrack_eGamma_chi2Ndof;
   vector<float>   *trig_L2_trk_sitrack_eGamma_param_a0;
   vector<float>   *trig_L2_trk_sitrack_eGamma_param_z0;
   vector<float>   *trig_L2_trk_sitrack_eGamma_param_phi0;
   vector<float>   *trig_L2_trk_sitrack_eGamma_param_eta;
   vector<float>   *trig_L2_trk_sitrack_eGamma_param_pt;
   Int_t           trig_L2_el_n;
   vector<float>   *trig_L2_el_E;
   vector<float>   *trig_L2_el_Et;
   vector<float>   *trig_L2_el_pt;
   vector<float>   *trig_L2_el_eta;
   vector<float>   *trig_L2_el_phi;
   vector<int>     *trig_L2_el_RoIWord;
   vector<float>   *trig_L2_el_zvertex;
   vector<int>     *trig_L2_el_charge;
   Int_t           trig_L2_ph_n;
   vector<float>   *trig_L2_ph_E;
   vector<float>   *trig_L2_ph_Et;
   vector<float>   *trig_L2_ph_pt;
   vector<float>   *trig_L2_ph_eta;
   vector<float>   *trig_L2_ph_phi;
   vector<int>     *trig_L2_ph_RoIWord;
   Int_t           trig_EF_emcl_n;
   vector<float>   *trig_EF_emcl_pt;
   vector<float>   *trig_EF_emcl_eta;
   vector<float>   *trig_EF_emcl_phi;
   vector<float>   *trig_EF_emcl_E_em;
   vector<float>   *trig_EF_emcl_E_had;
   Int_t           trig_EF_emcl_slw_n;
   vector<float>   *trig_EF_emcl_slw_pt;
   vector<float>   *trig_EF_emcl_slw_eta;
   vector<float>   *trig_EF_emcl_slw_phi;
   vector<float>   *trig_EF_emcl_slw_E_em;
   vector<float>   *trig_EF_emcl_slw_E_had;
   Int_t           trig_EF_el_n;
   vector<float>   *trig_EF_el_E;
   vector<float>   *trig_EF_el_Et;
   vector<float>   *trig_EF_el_pt;
   vector<float>   *trig_EF_el_m;
   vector<float>   *trig_EF_el_eta;
   vector<float>   *trig_EF_el_phi;
   vector<float>   *trig_EF_el_px;
   vector<float>   *trig_EF_el_py;
   vector<float>   *trig_EF_el_pz;
   vector<float>   *trig_EF_el_charge;
   vector<int>     *trig_EF_el_author;
   vector<unsigned int> *trig_EF_el_isEM;
   vector<unsigned int> *trig_EF_el_isEMLoose;
   vector<unsigned int> *trig_EF_el_isEMMedium;
   vector<unsigned int> *trig_EF_el_isEMTight;
   vector<int>     *trig_EF_el_loose;
   vector<int>     *trig_EF_el_looseIso;
   vector<int>     *trig_EF_el_medium;
   vector<int>     *trig_EF_el_mediumIso;
   vector<int>     *trig_EF_el_mediumWithoutTrack;
   vector<int>     *trig_EF_el_mediumIsoWithoutTrack;
   vector<int>     *trig_EF_el_tight;
   vector<int>     *trig_EF_el_tightIso;
   vector<int>     *trig_EF_el_tightWithoutTrack;
   vector<int>     *trig_EF_el_tightIsoWithoutTrack;
   vector<int>     *trig_EF_el_loosePP;
   vector<int>     *trig_EF_el_loosePPIso;
   vector<int>     *trig_EF_el_mediumPP;
   vector<int>     *trig_EF_el_mediumPPIso;
   vector<int>     *trig_EF_el_tightPP;
   vector<int>     *trig_EF_el_tightPPIso;
   vector<float>   *trig_EF_el_vertweight;
   vector<int>     *trig_EF_el_hastrack;
   Int_t           trig_EF_ph_n;
   vector<float>   *trig_EF_ph_E;
   vector<float>   *trig_EF_ph_Et;
   vector<float>   *trig_EF_ph_pt;
   vector<float>   *trig_EF_ph_m;
   vector<float>   *trig_EF_ph_eta;
   vector<float>   *trig_EF_ph_phi;
   vector<float>   *trig_EF_ph_px;
   vector<float>   *trig_EF_ph_py;
   vector<float>   *trig_EF_ph_pz;
   vector<int>     *trig_EF_ph_author;
   vector<int>     *trig_EF_ph_isRecovered;
   vector<unsigned int> *trig_EF_ph_isEM;
   vector<unsigned int> *trig_EF_ph_isEMLoose;
   vector<unsigned int> *trig_EF_ph_isEMMedium;
   vector<unsigned int> *trig_EF_ph_isEMTight;
   vector<int>     *trig_EF_ph_convFlag;
   vector<int>     *trig_EF_ph_isConv;
   vector<int>     *trig_EF_ph_nConv;
   vector<int>     *trig_EF_ph_nSingleTrackConv;
   vector<int>     *trig_EF_ph_nDoubleTrackConv;
   vector<int>     *trig_EF_ph_loose;
   vector<int>     *trig_EF_ph_looseIso;
   vector<int>     *trig_EF_ph_tight;
   vector<int>     *trig_EF_ph_tightIso;
   vector<int>     *trig_EF_ph_looseAR;
   vector<int>     *trig_EF_ph_looseARIso;
   vector<int>     *trig_EF_ph_tightAR;
   vector<int>     *trig_EF_ph_tightARIso;
   Int_t           trig_Nav_n;
   vector<short>   *trig_Nav_chain_ChainId;
   vector<vector<int> > *trig_Nav_chain_RoIType;
   vector<vector<int> > *trig_Nav_chain_RoIIndex;
   Int_t           trig_RoI_L2_e_n;
   Int_t           trig_RoI_EF_e_n;
   UInt_t          trig_DB_SMK;
   UInt_t          trig_DB_L1PSK;
   UInt_t          trig_DB_HLTPSK;
   vector<unsigned int> *trig_L1_TAV;
   vector<short>   *trig_L2_passedPhysics;
   vector<short>   *trig_EF_passedPhysics;
   vector<unsigned int> *trig_L1_TBP;
   vector<unsigned int> *trig_L1_TAP;
   vector<short>   *trig_L2_passedRaw;
   vector<short>   *trig_EF_passedRaw;
   Bool_t          trig_L2_truncated;
   Bool_t          trig_EF_truncated;
   vector<short>   *trig_L2_resurrected;
   vector<short>   *trig_EF_resurrected;
   vector<short>   *trig_L2_passedThrough;
   vector<short>   *trig_EF_passedThrough;
   vector<short>   *trig_L2_wasPrescaled;
   vector<short>   *trig_L2_wasResurrected;
   vector<short>   *trig_EF_wasPrescaled;
   vector<short>   *trig_EF_wasResurrected;
   Int_t           trig_EF_trigmuonef_n;
   vector<int>     *trig_EF_trigmuonef_track_n;
   vector<vector<int> > *trig_EF_trigmuonef_track_MuonType;
   vector<vector<float> > *trig_EF_trigmuonef_track_MS_pt;
   vector<vector<float> > *trig_EF_trigmuonef_track_MS_eta;
   vector<vector<float> > *trig_EF_trigmuonef_track_MS_phi;
   vector<vector<int> > *trig_EF_trigmuonef_track_MS_hasMS;
   vector<vector<float> > *trig_EF_trigmuonef_track_SA_pt;
   vector<vector<float> > *trig_EF_trigmuonef_track_SA_eta;
   vector<vector<float> > *trig_EF_trigmuonef_track_SA_phi;
   vector<vector<int> > *trig_EF_trigmuonef_track_SA_hasSA;
   vector<vector<float> > *trig_EF_trigmuonef_track_CB_pt;
   vector<vector<float> > *trig_EF_trigmuonef_track_CB_eta;
   vector<vector<float> > *trig_EF_trigmuonef_track_CB_phi;
   vector<vector<int> > *trig_EF_trigmuonef_track_CB_hasCB;
   Int_t           trig_EF_trigmugirl_n;
   vector<int>     *trig_EF_trigmugirl_track_n;
   vector<vector<int> > *trig_EF_trigmugirl_track_MuonType;
   vector<vector<float> > *trig_EF_trigmugirl_track_MS_pt;
   vector<vector<float> > *trig_EF_trigmugirl_track_MS_eta;
   vector<vector<float> > *trig_EF_trigmugirl_track_MS_phi;
   vector<vector<int> > *trig_EF_trigmugirl_track_MS_hasMS;
   vector<vector<float> > *trig_EF_trigmugirl_track_SA_pt;
   vector<vector<float> > *trig_EF_trigmugirl_track_SA_eta;
   vector<vector<float> > *trig_EF_trigmugirl_track_SA_phi;
   vector<vector<int> > *trig_EF_trigmugirl_track_SA_hasSA;
   vector<vector<float> > *trig_EF_trigmugirl_track_CB_pt;
   vector<vector<float> > *trig_EF_trigmugirl_track_CB_eta;
   vector<vector<float> > *trig_EF_trigmugirl_track_CB_phi;
   vector<vector<int> > *trig_EF_trigmugirl_track_CB_hasCB;
   Int_t           trig_L1_mu_n;
   vector<float>   *trig_L1_mu_pt;
   vector<float>   *trig_L1_mu_eta;
   vector<float>   *trig_L1_mu_phi;
   vector<string>  *trig_L1_mu_thrName;
   Int_t           trig_L2_muonfeature_n;
   vector<float>   *trig_L2_muonfeature_pt;
   vector<float>   *trig_L2_muonfeature_eta;
   vector<float>   *trig_L2_muonfeature_phi;
   Int_t           trig_L2_muonfeaturedetails_n;
   Int_t           trig_L2_combmuonfeature_n;
   vector<float>   *trig_L2_combmuonfeature_pt;
   vector<float>   *trig_L2_combmuonfeature_eta;
   vector<float>   *trig_L2_combmuonfeature_phi;
   vector<int>     *trig_L2_combmuonfeature_idscantrk_index;
   vector<int>     *trig_L2_combmuonfeature_sitracktrk_index;
   Int_t           trig_L2_isomuonfeature_n;
   vector<float>   *trig_L2_isomuonfeature_pt;
   vector<float>   *trig_L2_isomuonfeature_eta;
   vector<float>   *trig_L2_isomuonfeature_phi;
   vector<float>   *trig_L2_isomuonfeature_EtInnerConeEC;
   vector<float>   *trig_L2_isomuonfeature_EtOuterConeEC;
   vector<float>   *trig_L2_isomuonfeature_EtInnerConeHC;
   vector<float>   *trig_L2_isomuonfeature_EtOuterConeHC;
   vector<int>     *trig_L2_isomuonfeature_NTracksCone;
   vector<float>   *trig_L2_isomuonfeature_SumPtTracksCone;
   vector<float>   *trig_L2_isomuonfeature_PtMuTracksCone;
   Int_t           trig_L2_tilemufeature_n;
   vector<float>   *trig_L2_tilemufeature_eta;
   vector<float>   *trig_L2_tilemufeature_phi;
   Int_t           trig_L2_tiletrackmufeature_n;
   vector<float>   *trig_L2_tiletrackmufeature_pt;
   vector<float>   *trig_L2_tiletrackmufeature_eta;
   vector<float>   *trig_L2_tiletrackmufeature_phi;
   vector<int>     *trig_L2_tiletrackmufeature_tilemu_index;
   vector<int>     *trig_L2_tiletrackmufeature_idtrk_algorithmId;
   vector<short>   *trig_L2_tiletrackmufeature_idtrk_trackStatus;
   vector<float>   *trig_L2_tiletrackmufeature_idtrk_chi2Ndof;
   vector<float>   *trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_a0;
   vector<float>   *trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_z0;
   vector<float>   *trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_phi0;
   vector<float>   *trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_eta;
   vector<float>   *trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_pt;
   vector<int>     *trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_hasidtrkfitpar;
   vector<int>     *trig_L2_tiletrackmufeature_idtrk_hasidtrk;
   Int_t           trig_L2_sitrack_muon_n;
   vector<int>     *trig_L2_sitrack_muon_algorithmId;
   vector<short>   *trig_L2_sitrack_muon_trackStatus;
   vector<float>   *trig_L2_sitrack_muon_chi2Ndof;
   vector<float>   *trig_L2_sitrack_muon_param_a0;
   vector<float>   *trig_L2_sitrack_muon_param_z0;
   vector<float>   *trig_L2_sitrack_muon_param_phi0;
   vector<float>   *trig_L2_sitrack_muon_param_eta;
   vector<float>   *trig_L2_sitrack_muon_param_pt;
   Int_t           trig_L2_idscan_muon_n;
   vector<int>     *trig_L2_idscan_muon_algorithmId;
   vector<short>   *trig_L2_idscan_muon_trackStatus;
   vector<float>   *trig_L2_idscan_muon_chi2Ndof;
   vector<float>   *trig_L2_idscan_muon_param_a0;
   vector<float>   *trig_L2_idscan_muon_param_z0;
   vector<float>   *trig_L2_idscan_muon_param_phi0;
   vector<float>   *trig_L2_idscan_muon_param_eta;
   vector<float>   *trig_L2_idscan_muon_param_pt;
   Int_t           trig_L2_sitrack_isomuon_n;
   vector<int>     *trig_L2_sitrack_isomuon_algorithmId;
   vector<short>   *trig_L2_sitrack_isomuon_trackStatus;
   vector<float>   *trig_L2_sitrack_isomuon_chi2Ndof;
   vector<float>   *trig_L2_sitrack_isomuon_param_a0;
   vector<float>   *trig_L2_sitrack_isomuon_param_z0;
   vector<float>   *trig_L2_sitrack_isomuon_param_phi0;
   vector<float>   *trig_L2_sitrack_isomuon_param_eta;
   vector<float>   *trig_L2_sitrack_isomuon_param_pt;
   Int_t           trig_L2_idscan_isomuon_n;
   vector<int>     *trig_L2_idscan_isomuon_algorithmId;
   vector<short>   *trig_L2_idscan_isomuon_trackStatus;
   vector<float>   *trig_L2_idscan_isomuon_chi2Ndof;
   vector<float>   *trig_L2_idscan_isomuon_param_a0;
   vector<float>   *trig_L2_idscan_isomuon_param_z0;
   vector<float>   *trig_L2_idscan_isomuon_param_phi0;
   vector<float>   *trig_L2_idscan_isomuon_param_eta;
   vector<float>   *trig_L2_idscan_isomuon_param_pt;
   Int_t           trig_roidescriptor_forID_n;
   vector<float>   *trig_roidescriptor_forID_phi;
   vector<float>   *trig_roidescriptor_forID_eta;
   vector<float>   *trig_roidescriptor_forID_zed0;
   Int_t           trig_L1_jet_n;
   vector<float>   *trig_L1_jet_eta;
   vector<float>   *trig_L1_jet_phi;
   vector<vector<string> > *trig_L1_jet_thrNames;
   vector<vector<float> > *trig_L1_jet_thrValues;
   vector<unsigned int> *trig_L1_jet_thrPattern;
   vector<float>   *trig_L1_jet_et4x4;
   vector<float>   *trig_L1_jet_et6x6;
   vector<float>   *trig_L1_jet_et8x8;
   vector<unsigned int> *trig_L1_jet_RoIWord;
   Int_t           trig_L2_jet_n;
   vector<float>   *trig_L2_jet_E;
   vector<float>   *trig_L2_jet_eta;
   vector<float>   *trig_L2_jet_phi;
   vector<unsigned int> *trig_L2_jet_RoIWord;
   vector<double>  *trig_L2_jet_ehad0;
   vector<double>  *trig_L2_jet_eem0;
   vector<int>     *trig_L2_jet_nLeadingCells;
   vector<float>   *trig_L2_jet_hecf;
   vector<float>   *trig_L2_jet_jetQuality;
   vector<float>   *trig_L2_jet_emf;
   vector<float>   *trig_L2_jet_jetTimeCells;
   Int_t           trig_EF_jet_n;
   vector<float>   *trig_EF_jet_emscale_E;
   vector<float>   *trig_EF_jet_emscale_pt;
   vector<float>   *trig_EF_jet_emscale_m;
   vector<float>   *trig_EF_jet_emscale_eta;
   vector<float>   *trig_EF_jet_emscale_phi;
   vector<int>     *trig_EF_jet_a4;
   vector<int>     *trig_EF_jet_a4tc;
   vector<int>     *trig_EF_jet_a10tc;
   vector<int>     *trig_EF_jet_a6;
   vector<int>     *trig_EF_jet_a6tc;
   vector<unsigned int> *trig_EF_jet_RoIword;
   Int_t           trig_RoI_L2_b_n;
   vector<short>   *trig_RoI_L2_b_type;
   vector<short>   *trig_RoI_L2_b_active;
   vector<short>   *trig_RoI_L2_b_lastStep;
   vector<short>   *trig_RoI_L2_b_TENumber;
   vector<short>   *trig_RoI_L2_b_roiNumber;
   vector<int>     *trig_RoI_L2_b_Jet_ROI;
   vector<int>     *trig_RoI_L2_b_Jet_ROIStatus;
   vector<int>     *trig_RoI_L2_b_Muon_ROI;
   vector<int>     *trig_RoI_L2_b_Muon_ROIStatus;
   vector<vector<int> > *trig_RoI_L2_b_TrigL2BjetContainer;
   vector<vector<int> > *trig_RoI_L2_b_TrigL2BjetContainerStatus;
   vector<vector<int> > *trig_RoI_L2_b_TrigInDetTrackCollection_TrigSiTrack_Jet;
   vector<vector<int> > *trig_RoI_L2_b_TrigInDetTrackCollection_TrigSiTrack_JetStatus;
   vector<vector<int> > *trig_RoI_L2_b_TrigInDetTrackCollection_TrigIDSCAN_Jet;
   vector<vector<int> > *trig_RoI_L2_b_TrigInDetTrackCollection_TrigIDSCAN_JetStatus;
   Int_t           trig_RoI_EF_b_n;
   vector<short>   *trig_RoI_EF_b_type;
   vector<short>   *trig_RoI_EF_b_active;
   vector<short>   *trig_RoI_EF_b_lastStep;
   vector<short>   *trig_RoI_EF_b_TENumber;
   vector<short>   *trig_RoI_EF_b_roiNumber;
   vector<int>     *trig_RoI_EF_b_Jet_ROI;
   vector<int>     *trig_RoI_EF_b_Jet_ROIStatus;
   vector<int>     *trig_RoI_EF_b_Muon_ROI;
   vector<int>     *trig_RoI_EF_b_Muon_ROIStatus;
   vector<vector<int> > *trig_RoI_EF_b_TrigEFBjetContainer;
   vector<vector<int> > *trig_RoI_EF_b_TrigEFBjetContainerStatus;
   vector<vector<int> > *trig_RoI_EF_b_Rec__TrackParticleContainer;
   vector<vector<int> > *trig_RoI_EF_b_Rec__TrackParticleContainerStatus;
   Int_t           trig_L2_bjet_n;
   vector<int>     *trig_L2_bjet_roiId;
   vector<int>     *trig_L2_bjet_valid;
   vector<float>   *trig_L2_bjet_prmVtx;
   vector<float>   *trig_L2_bjet_pt;
   vector<float>   *trig_L2_bjet_eta;
   vector<float>   *trig_L2_bjet_phi;
   vector<float>   *trig_L2_bjet_xComb;
   vector<float>   *trig_L2_bjet_xIP1D;
   vector<float>   *trig_L2_bjet_xIP2D;
   vector<float>   *trig_L2_bjet_xIP3D;
   vector<float>   *trig_L2_bjet_xCHI2;
   vector<float>   *trig_L2_bjet_xSV;
   vector<float>   *trig_L2_bjet_xMVtx;
   vector<float>   *trig_L2_bjet_xEVtx;
   vector<float>   *trig_L2_bjet_xNVtx;
   vector<float>   *trig_L2_bjet_BSx;
   vector<float>   *trig_L2_bjet_BSy;
   vector<float>   *trig_L2_bjet_BSz;
   vector<float>   *trig_L2_bjet_sBSx;
   vector<float>   *trig_L2_bjet_sBSy;
   vector<float>   *trig_L2_bjet_sBSz;
   vector<float>   *trig_L2_bjet_sBSxy;
   vector<float>   *trig_L2_bjet_BTiltXZ;
   vector<float>   *trig_L2_bjet_BTiltYZ;
   vector<int>     *trig_L2_bjet_BSstatus;
   Int_t           trig_EF_bjet_n;
   vector<int>     *trig_EF_bjet_roiId;
   vector<int>     *trig_EF_bjet_valid;
   vector<float>   *trig_EF_bjet_prmVtx;
   vector<float>   *trig_EF_bjet_pt;
   vector<float>   *trig_EF_bjet_eta;
   vector<float>   *trig_EF_bjet_phi;
   vector<float>   *trig_EF_bjet_xComb;
   vector<float>   *trig_EF_bjet_xIP1D;
   vector<float>   *trig_EF_bjet_xIP2D;
   vector<float>   *trig_EF_bjet_xIP3D;
   vector<float>   *trig_EF_bjet_xCHI2;
   vector<float>   *trig_EF_bjet_xSV;
   vector<float>   *trig_EF_bjet_xMVtx;
   vector<float>   *trig_EF_bjet_xEVtx;
   vector<float>   *trig_EF_bjet_xNVtx;
   Int_t           trig_EF_pv_n;
   vector<float>   *trig_EF_pv_x;
   vector<float>   *trig_EF_pv_y;
   vector<float>   *trig_EF_pv_z;
   vector<int>     *trig_EF_pv_type;
   vector<float>   *trig_EF_pv_err_x;
   vector<float>   *trig_EF_pv_err_y;
   vector<float>   *trig_EF_pv_err_z;
   Int_t           trig_EF_tau_n;
   vector<float>   *trig_EF_tau_Et;
   vector<float>   *trig_EF_tau_pt;
   vector<float>   *trig_EF_tau_m;
   vector<float>   *trig_EF_tau_eta;
   vector<float>   *trig_EF_tau_phi;
   vector<float>   *trig_EF_tau_px;
   vector<float>   *trig_EF_tau_py;
   vector<float>   *trig_EF_tau_pz;
   vector<int>     *trig_EF_tau_author;
   vector<int>     *trig_EF_tau_RoIWord;
   vector<int>     *trig_EF_tau_nProng;
   vector<int>     *trig_EF_tau_numTrack;
   vector<int>     *trig_EF_tau_seedCalo_numTrack;
   vector<int>     *trig_EF_tau_seedCalo_track_n;
   vector<int>     *trig_EF_tau_seedCalo_wideTrk_n;
   vector<int>     *trig_EF_tau_otherTrk_n;
   Int_t           trig_L2_trk_idscan_tau_n;
   vector<int>     *trig_L2_trk_idscan_tau_algorithmId;
   vector<short>   *trig_L2_trk_idscan_tau_trackStatus;
   vector<float>   *trig_L2_trk_idscan_tau_chi2Ndof;
   vector<float>   *trig_L2_trk_idscan_tau_param_a0;
   vector<float>   *trig_L2_trk_idscan_tau_param_z0;
   vector<float>   *trig_L2_trk_idscan_tau_param_phi0;
   vector<float>   *trig_L2_trk_idscan_tau_param_eta;
   vector<float>   *trig_L2_trk_idscan_tau_param_pt;
   Int_t           trig_L2_trk_sitrack_tau_n;
   vector<int>     *trig_L2_trk_sitrack_tau_algorithmId;
   vector<short>   *trig_L2_trk_sitrack_tau_trackStatus;
   vector<float>   *trig_L2_trk_sitrack_tau_chi2Ndof;
   vector<float>   *trig_L2_trk_sitrack_tau_param_a0;
   vector<float>   *trig_L2_trk_sitrack_tau_param_z0;
   vector<float>   *trig_L2_trk_sitrack_tau_param_phi0;
   vector<float>   *trig_L2_trk_sitrack_tau_param_eta;
   vector<float>   *trig_L2_trk_sitrack_tau_param_pt;
   Int_t           trig_L2_tau_n;
   vector<float>   *trig_L2_tau_pt;
   vector<float>   *trig_L2_tau_eta;
   vector<float>   *trig_L2_tau_phi;
   vector<float>   *trig_L2_tau_px;
   vector<float>   *trig_L2_tau_py;
   vector<float>   *trig_L2_tau_pz;
   vector<unsigned int> *trig_L2_tau_RoIWord;
   vector<int>     *trig_L2_tau_tracks_algorithmId;
   vector<int>     *trig_L2_tau_idscan_trk_n;
   vector<vector<int> > *trig_L2_tau_idscan_trk_index;
   vector<int>     *trig_L2_tau_sitrack_trk_n;
   vector<vector<int> > *trig_L2_tau_sitrack_trk_index;
   vector<string>  *trig_L1_esum_thrNames;
   Float_t         trig_L1_esum_ExMiss;
   Float_t         trig_L1_esum_EyMiss;
   Float_t         trig_L1_esum_energyT;
   Bool_t          trig_L1_esum_overflowX;
   Bool_t          trig_L1_esum_overflowY;
   Bool_t          trig_L1_esum_overflowT;
   Float_t         trig_L2_met_MEx;
   Float_t         trig_L2_met_MEy;
   Float_t         trig_L2_met_MEz;
   Float_t         trig_L2_met_sumEt;
   Float_t         trig_L2_met_sumE;
   Int_t           trig_L2_met_flag;
   vector<string>  *trig_L2_met_nameOfComponent;
   vector<float>   *trig_L2_met_MExComponent;
   vector<float>   *trig_L2_met_MEyComponent;
   vector<float>   *trig_L2_met_MEzComponent;
   vector<float>   *trig_L2_met_sumEtComponent;
   vector<float>   *trig_L2_met_sumEComponent;
   vector<float>   *trig_L2_met_componentCalib0;
   vector<float>   *trig_L2_met_componentCalib1;
   vector<short>   *trig_L2_met_sumOfSigns;
   vector<unsigned short> *trig_L2_met_usedChannels;
   vector<short>   *trig_L2_met_status;
   Float_t         trig_L2_feb_met_MEx;
   Float_t         trig_L2_feb_met_MEy;
   Float_t         trig_L2_feb_met_MEz;
   Float_t         trig_L2_feb_met_sumEt;
   Float_t         trig_L2_feb_met_sumE;
   Int_t           trig_L2_feb_met_flag;
   vector<string>  *trig_L2_feb_met_nameOfComponent;
   vector<float>   *trig_L2_feb_met_MExComponent;
   vector<float>   *trig_L2_feb_met_MEyComponent;
   vector<float>   *trig_L2_feb_met_MEzComponent;
   vector<float>   *trig_L2_feb_met_sumEtComponent;
   vector<float>   *trig_L2_feb_met_sumEComponent;
   vector<float>   *trig_L2_feb_met_componentCalib0;
   vector<float>   *trig_L2_feb_met_componentCalib1;
   vector<short>   *trig_L2_feb_met_sumOfSigns;
   vector<unsigned short> *trig_L2_feb_met_usedChannels;
   vector<short>   *trig_L2_feb_met_status;
   Float_t         trig_EF_met_MEx;
   Float_t         trig_EF_met_MEy;
   Float_t         trig_EF_met_MEz;
   Float_t         trig_EF_met_sumEt;
   Float_t         trig_EF_met_sumE;
   Int_t           trig_EF_met_flag;
   vector<string>  *trig_EF_met_nameOfComponent;
   vector<float>   *trig_EF_met_MExComponent;
   vector<float>   *trig_EF_met_MEyComponent;
   vector<float>   *trig_EF_met_MEzComponent;
   vector<float>   *trig_EF_met_sumEtComponent;
   vector<float>   *trig_EF_met_sumEComponent;
   vector<float>   *trig_EF_met_componentCalib0;
   vector<float>   *trig_EF_met_componentCalib1;
   vector<short>   *trig_EF_met_sumOfSigns;
   vector<unsigned short> *trig_EF_met_usedChannels;
   vector<short>   *trig_EF_met_status;
   Char_t          trig_bgCode;
   Int_t           jet_AntiKt4TrackZ_n;
   vector<float>   *jet_AntiKt4TrackZ_E;
   vector<float>   *jet_AntiKt4TrackZ_pt;
   vector<float>   *jet_AntiKt4TrackZ_m;
   vector<float>   *jet_AntiKt4TrackZ_eta;
   vector<float>   *jet_AntiKt4TrackZ_phi;
   vector<float>   *jet_AntiKt4TrackZ_el_dr;
   vector<int>     *jet_AntiKt4TrackZ_el_matched;
   vector<float>   *jet_AntiKt4TrackZ_mu_dr;
   vector<int>     *jet_AntiKt4TrackZ_mu_matched;
   vector<float>   *jet_AntiKt4TrackZ_L1_dr;
   vector<int>     *jet_AntiKt4TrackZ_L1_matched;
   vector<float>   *jet_AntiKt4TrackZ_L2_dr;
   vector<int>     *jet_AntiKt4TrackZ_L2_matched;
   vector<float>   *jet_AntiKt4TrackZ_EF_dr;
   vector<int>     *jet_AntiKt4TrackZ_EF_matched;
   UInt_t          bunch_configID;

   // List of branches
   TBranch        *b_EF_2MUL1_l2j30_HV;   //!
   TBranch        *b_EF_2MUL1_l2j30_HV_AllMS;   //!
   TBranch        *b_EF_2MUL1_l2j30_HV_cosmic;   //!
   TBranch        *b_EF_2MUL1_l2j30_HV_firstempty;   //!
   TBranch        *b_EF_2MUL1_l2j30_HV_unpaired_iso;   //!
   TBranch        *b_EF_2MUL1_l2j30_HV_unpaired_noniso;   //!
   TBranch        *b_EF_2b10_medium_3L1J10;   //!
   TBranch        *b_EF_2b10_medium_4L1J10;   //!
   TBranch        *b_EF_2b10_medium_4j30_a4tc_EFFS;   //!
   TBranch        *b_EF_2b10_medium_L1JE100;   //!
   TBranch        *b_EF_2b10_medium_L1JE140;   //!
   TBranch        *b_EF_2b10_medium_L1_2J10J50;   //!
   TBranch        *b_EF_2b10_medium_j100_j30_a4tc_EFFS;   //!
   TBranch        *b_EF_2b10_medium_j75_j30_a4tc_EFFS;   //!
   TBranch        *b_EF_2b10_tight_4j30_a4tc_EFFS;   //!
   TBranch        *b_EF_2b15_medium_3L1J15;   //!
   TBranch        *b_EF_2b15_medium_3j40_a4tc_EFFS;   //!
   TBranch        *b_EF_2b15_medium_j75_j40_a4tc_EFFS;   //!
   TBranch        *b_EF_2b20_medium_3L1J20;   //!
   TBranch        *b_EF_2b20_medium_3j45_a4tc_EFFS;   //!
   TBranch        *b_EF_2b20_medium_j75_j45_a4tc_EFFS;   //!
   TBranch        *b_EF_2e10_medium;   //!
   TBranch        *b_EF_2e10_medium_mu6;   //!
   TBranch        *b_EF_2e11T_medium;   //!
   TBranch        *b_EF_2e12T_medium;   //!
   TBranch        *b_EF_2e12_medium;   //!
   TBranch        *b_EF_2e15_loose;   //!
   TBranch        *b_EF_2e15_medium;   //!
   TBranch        *b_EF_2e5_medium;   //!
   TBranch        *b_EF_2e5_medium_SiTrk;   //!
   TBranch        *b_EF_2e5_medium_TRT;   //!
   TBranch        *b_EF_2e5_tight;   //!
   TBranch        *b_EF_2e5_tight_Jpsi;   //!
   TBranch        *b_EF_2fj100_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_EF_2fj30_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_EF_2fj30_a4tc_EFFS_deta50_FC;   //!
   TBranch        *b_EF_2fj55_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_EF_2fj55_a4tc_EFFS_deta50_FC;   //!
   TBranch        *b_EF_2fj75_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_EF_2fj75_a4tc_EFFS_deta50_FC;   //!
   TBranch        *b_EF_2g10_loose;   //!
   TBranch        *b_EF_2g15_loose;   //!
   TBranch        *b_EF_2g20_loose;   //!
   TBranch        *b_EF_2g50_etcut;   //!
   TBranch        *b_EF_2j100_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_EF_2j135_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_EF_2j180_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_EF_2j240_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_EF_2j45_a4tc_EFFS_leadingmct100_xe40_medium_noMu;   //!
   TBranch        *b_EF_2j55_a4tc_EFFS_leadingmct100_xe40_medium_noMu;   //!
   TBranch        *b_EF_2mu10;   //!
   TBranch        *b_EF_2mu10_NoAlg;   //!
   TBranch        *b_EF_2mu10_empty;   //!
   TBranch        *b_EF_2mu10_loose;   //!
   TBranch        *b_EF_2mu10_loose1;   //!
   TBranch        *b_EF_2mu10_loose_empty;   //!
   TBranch        *b_EF_2mu10_loose_noOvlpRm;   //!
   TBranch        *b_EF_2mu10_noOvlpRm;   //!
   TBranch        *b_EF_2mu13_Zmumu_IDTrkNoCut;   //!
   TBranch        *b_EF_2mu4;   //!
   TBranch        *b_EF_2mu4_Bmumu;   //!
   TBranch        *b_EF_2mu4_Bmumux;   //!
   TBranch        *b_EF_2mu4_DiMu;   //!
   TBranch        *b_EF_2mu4_DiMu_DY;   //!
   TBranch        *b_EF_2mu4_DiMu_DY20;   //!
   TBranch        *b_EF_2mu4_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_EF_2mu4_DiMu_SiTrk;   //!
   TBranch        *b_EF_2mu4_DiMu_noVtx;   //!
   TBranch        *b_EF_2mu4_DiMu_noVtx_noOS;   //!
   TBranch        *b_EF_2mu4_EFxe20_noMu;   //!
   TBranch        *b_EF_2mu4_Jpsimumu;   //!
   TBranch        *b_EF_2mu4_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_EF_2mu4_Upsimumu;   //!
   TBranch        *b_EF_2mu4_xe30_noMu;   //!
   TBranch        *b_EF_2mu4_xe40_noMu;   //!
   TBranch        *b_EF_2mu4i_DiMu_DY;   //!
   TBranch        *b_EF_2mu4i_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_EF_2mu6;   //!
   TBranch        *b_EF_2mu6_Bmumu;   //!
   TBranch        *b_EF_2mu6_Bmumux;   //!
   TBranch        *b_EF_2mu6_DiMu;   //!
   TBranch        *b_EF_2mu6_Jpsimumu;   //!
   TBranch        *b_EF_2mu6_MSonly_g10_loose;   //!
   TBranch        *b_EF_2mu6_MSonly_g10_loose_noOvlpRm;   //!
   TBranch        *b_EF_2mu6_MSonly_g10_loose_nonfilled;   //!
   TBranch        *b_EF_2mu6_NL;   //!
   TBranch        *b_EF_2mu6_Upsimumu;   //!
   TBranch        *b_EF_2mu6_e10_loose;   //!
   TBranch        *b_EF_2mu6_g10_loose;   //!
   TBranch        *b_EF_2tau16_loose;   //!
   TBranch        *b_EF_2tau29T_medium1;   //!
   TBranch        *b_EF_2tau29_loose;   //!
   TBranch        *b_EF_2tau29_loose1;   //!
   TBranch        *b_EF_2tau29_loose1_EFxe15_noMu;   //!
   TBranch        *b_EF_2tau29_loose1_EFxe40_noMu;   //!
   TBranch        *b_EF_2tau29_medium1;   //!
   TBranch        *b_EF_2tau29i_medium1;   //!
   TBranch        *b_EF_2tau38_loose;   //!
   TBranch        *b_EF_3b10_loose_4L1J10;   //!
   TBranch        *b_EF_3b10_medium_4j30_a4tc_EFFS;   //!
   TBranch        *b_EF_3b15_loose_4L1J15;   //!
   TBranch        *b_EF_3b15_medium_4j40_a4tc_EFFS;   //!
   TBranch        *b_EF_3e10_medium;   //!
   TBranch        *b_EF_3j100_a4tc_EFFS;   //!
   TBranch        *b_EF_3j100_a4tc_EFFS_L1J75;   //!
   TBranch        *b_EF_3j30_a4tc_EFFS;   //!
   TBranch        *b_EF_3j40_a4tc_EFFS;   //!
   TBranch        *b_EF_3j45_a4tc_EFFS;   //!
   TBranch        *b_EF_3j75_a4tc_EFFS;   //!
   TBranch        *b_EF_3mu4;   //!
   TBranch        *b_EF_3mu4_DiMu;   //!
   TBranch        *b_EF_3mu4_Jpsi;   //!
   TBranch        *b_EF_3mu4_Tau;   //!
   TBranch        *b_EF_3mu4_Upsi;   //!
   TBranch        *b_EF_3mu6;   //!
   TBranch        *b_EF_3mu6_MSonly;   //!
   TBranch        *b_EF_3mu6_MSonly_noOvlpRm;   //!
   TBranch        *b_EF_3mu6_MSonly_nonfilled;   //!
   TBranch        *b_EF_4j30_a4tc_EFFS;   //!
   TBranch        *b_EF_4j40_a4tc_EFFS;   //!
   TBranch        *b_EF_4j40_a4tc_EFFS_ht350;   //!
   TBranch        *b_EF_4j40_a4tc_EFFS_ht400;   //!
   TBranch        *b_EF_4j45_a4tc_EFFS;   //!
   TBranch        *b_EF_4j55_a4tc_EFFS;   //!
   TBranch        *b_EF_4j60_a4tc_EFFS;   //!
   TBranch        *b_EF_4mu4;   //!
   TBranch        *b_EF_5j30_a4tc_EFFS;   //!
   TBranch        *b_EF_5j40_a4tc_EFFS;   //!
   TBranch        *b_EF_5j45_a4tc_EFFS;   //!
   TBranch        *b_EF_6j30_a4tc_EFFS;   //!
   TBranch        *b_EF_6j30_a4tc_EFFS_L15J10;   //!
   TBranch        *b_EF_6j40_a4tc_EFFS;   //!
   TBranch        *b_EF_6j45_a4tc_EFFS;   //!
   TBranch        *b_EF_7j30_a4tc_EFFS_L15J10;   //!
   TBranch        *b_EF_7j30_a4tc_EFFS_L16J10;   //!
   TBranch        *b_EF_ALFA_Phys;   //!
   TBranch        *b_EF_Background;   //!
   TBranch        *b_EF_CosmicsAllTeEFID;   //!
   TBranch        *b_EF_CosmicsAllTeIDSCAN_AllPhysics_4Hits_TrkHypo;   //!
   TBranch        *b_EF_CosmicsAllTeIDSCAN_AllPhysics_TrkHypo;   //!
   TBranch        *b_EF_CosmicsAllTeSiTrack_AllPhysics_4Hits_TrkHypo;   //!
   TBranch        *b_EF_CosmicsAllTeSiTrack_AllPhysics_TrkHypo;   //!
   TBranch        *b_EF_CosmicsAllTeTRTxK_TRTTrkHypo_AllPhysics;   //!
   TBranch        *b_EF_CosmicsAllTeTRTxK_TRTTrkHypo_Pixel;   //!
   TBranch        *b_EF_Cosmics_MU11_TRTxK_TRTTrkHypo_AllID;   //!
   TBranch        *b_EF_Cosmics_MU11_TRTxK_TRTTrkHypo_Pixel;   //!
   TBranch        *b_EF_HLTTimeBurner;   //!
   TBranch        *b_EF_InDetMon_FS;   //!
   TBranch        *b_EF_InDetMon_L2Star_FS;   //!
   TBranch        *b_EF_L1Calo_NoAlg;   //!
   TBranch        *b_EF_L1FJ10_empty_NoAlg;   //!
   TBranch        *b_EF_L1FJ75_NoAlg;   //!
   TBranch        *b_EF_L1J10_empty_NoAlg;   //!
   TBranch        *b_EF_L1J10_firstempty_NoAlg;   //!
   TBranch        *b_EF_L1J10_unpaired_iso_NoAlg;   //!
   TBranch        *b_EF_L1J10_unpaired_noniso_NoAlg;   //!
   TBranch        *b_EF_L1J175_NoAlg;   //!
   TBranch        *b_EF_L1J250_NoAlg;   //!
   TBranch        *b_EF_L1J30_empty_NoAlg;   //!
   TBranch        *b_EF_L1J30_firstempty_NoAlg;   //!
   TBranch        *b_EF_L1J30_unpaired_iso_NoAlg;   //!
   TBranch        *b_EF_L1J30_unpaired_noniso_NoAlg;   //!
   TBranch        *b_EF_L1MU10_firstempty_NoAlg;   //!
   TBranch        *b_EF_L1MU11_NoAlg;   //!
   TBranch        *b_EF_L1MU15_NoAlg;   //!
   TBranch        *b_EF_L1MU20_NoAlg;   //!
   TBranch        *b_EF_L1MinBias_NoAlg;   //!
   TBranch        *b_EF_L1Muon_NoAlg;   //!
   TBranch        *b_EF_L1TAU30_NoAlg;   //!
   TBranch        *b_EF_L1TRT_NoAlg;   //!
   TBranch        *b_EF_MU0_j45_a4tc_EFFS_xe45_loose_noMu;   //!
   TBranch        *b_EF_PreS_L1EM3_empty;   //!
   TBranch        *b_EF_PreS_L1EM5_empty;   //!
   TBranch        *b_EF_PreS_L1J10_empty;   //!
   TBranch        *b_EF_PreS_L1J10_firstempty;   //!
   TBranch        *b_EF_PreS_L1J30_empty;   //!
   TBranch        *b_EF_PreS_L1J30_firstempty;   //!
   TBranch        *b_EF_PreS_L1TAU8_empty;   //!
   TBranch        *b_EF_ROBRequestMon;   //!
   TBranch        *b_EF_SeededStreamerL1CaloXS;   //!
   TBranch        *b_EF_Standby;   //!
   TBranch        *b_EF_TileCalib_laser;   //!
   TBranch        *b_EF_allL1_NoAlg;   //!
   TBranch        *b_EF_b10_EFj10_a4tc_EFFS_IDTrkNoCut;   //!
   TBranch        *b_EF_b10_IDTrkNoCut;   //!
   TBranch        *b_EF_b10_L2Star_IDTrkNoCut;   //!
   TBranch        *b_EF_b10_medium_4L1J10;   //!
   TBranch        *b_EF_b10_medium_4j30_a4tc_EFFS;   //!
   TBranch        *b_EF_b10_medium_EFxe25_noMu_L1JE100;   //!
   TBranch        *b_EF_b10_medium_EFxe25_noMu_L1JE140;   //!
   TBranch        *b_EF_b10_medium_EFxe25_noMu_L1_2J10J50;   //!
   TBranch        *b_EF_b10_medium_j75_j55_2j30_a4tc_EFFS;   //!
   TBranch        *b_EF_b10_tight_4L1J10;   //!
   TBranch        *b_EF_b10_tight_4j30_a4tc_EFFS;   //!
   TBranch        *b_EF_b10_tight_L1JE100;   //!
   TBranch        *b_EF_b10_tight_L1JE140;   //!
   TBranch        *b_EF_b10_tight_j75_j55_2j30_a4tc_EFFS;   //!
   TBranch        *b_EF_b15_IDTrkNoCut;   //!
   TBranch        *b_EF_b20_IDTrkNoCut;   //!
   TBranch        *b_EF_cost_monitor;   //!
   TBranch        *b_EF_e10_NoCut;   //!
   TBranch        *b_EF_e10_loose;   //!
   TBranch        *b_EF_e10_loose_mu10;   //!
   TBranch        *b_EF_e10_loose_mu6;   //!
   TBranch        *b_EF_e10_medium;   //!
   TBranch        *b_EF_e10_medium_2mu6;   //!
   TBranch        *b_EF_e10_medium_mu10;   //!
   TBranch        *b_EF_e10_medium_mu6;   //!
   TBranch        *b_EF_e10_medium_mu6_topo_medium;   //!
   TBranch        *b_EF_e11T_medium;   //!
   TBranch        *b_EF_e11T_medium_2e6T_medium;   //!
   TBranch        *b_EF_e11_etcut;   //!
   TBranch        *b_EF_e12T_medium_mu6_topo_medium;   //!
   TBranch        *b_EF_e12_medium;   //!
   TBranch        *b_EF_e13_etcutTRT_xs60_noMu;   //!
   TBranch        *b_EF_e13_etcut_xs60_noMu;   //!
   TBranch        *b_EF_e13_etcut_xs70_noMu;   //!
   TBranch        *b_EF_e15_HLTtighter;   //!
   TBranch        *b_EF_e15_loose;   //!
   TBranch        *b_EF_e15_medium;   //!
   TBranch        *b_EF_e15_medium_e12_medium;   //!
   TBranch        *b_EF_e15_medium_mu6;   //!
   TBranch        *b_EF_e15_medium_xe30_noMu;   //!
   TBranch        *b_EF_e15_medium_xe35_loose_noMu;   //!
   TBranch        *b_EF_e15_medium_xe35_noMu;   //!
   TBranch        *b_EF_e15_medium_xe40_loose_noMu;   //!
   TBranch        *b_EF_e15_medium_xe40_noMu;   //!
   TBranch        *b_EF_e15_medium_xe50_noMu;   //!
   TBranch        *b_EF_e15_tight;   //!
   TBranch        *b_EF_e15_tight1;   //!
   TBranch        *b_EF_e15_tight_xe30_noMu;   //!
   TBranch        *b_EF_e18_medium;   //!
   TBranch        *b_EF_e18_medium_xe30_noMu;   //!
   TBranch        *b_EF_e18_tight;   //!
   TBranch        *b_EF_e20_L2Star_IDTrkNoCut;   //!
   TBranch        *b_EF_e20_etcut_xs60_noMu;   //!
   TBranch        *b_EF_e20_loose;   //!
   TBranch        *b_EF_e20_loose1;   //!
   TBranch        *b_EF_e20_looseTrk;   //!
   TBranch        *b_EF_e20_loose_2j10_a4tc_EFFS;   //!
   TBranch        *b_EF_e20_loose_3j10_a4tc_EFFS;   //!
   TBranch        *b_EF_e20_loose_4j10_a4tc_EFFS;   //!
   TBranch        *b_EF_e20_loose_IDTrkNoCut;   //!
   TBranch        *b_EF_e20_loose_TRT;   //!
   TBranch        *b_EF_e20_loose_xe20;   //!
   TBranch        *b_EF_e20_loose_xe20_noMu;   //!
   TBranch        *b_EF_e20_loose_xe30;   //!
   TBranch        *b_EF_e20_loose_xe30_noMu;   //!
   TBranch        *b_EF_e20_medium;   //!
   TBranch        *b_EF_e20_medium1;   //!
   TBranch        *b_EF_e20_medium2;   //!
   TBranch        *b_EF_e20_medium_IDTrkNoCut;   //!
   TBranch        *b_EF_e20_medium_SiTrk;   //!
   TBranch        *b_EF_e20_medium_TRT;   //!
   TBranch        *b_EF_e20_tight;   //!
   TBranch        *b_EF_e20_tight_e15_NoCut_Zee;   //!
   TBranch        *b_EF_e22_etcut_xs60_noMu;   //!
   TBranch        *b_EF_e22_loose;   //!
   TBranch        *b_EF_e22_loose1;   //!
   TBranch        *b_EF_e22_looseTrk;   //!
   TBranch        *b_EF_e22_medium;   //!
   TBranch        *b_EF_e22_medium1;   //!
   TBranch        *b_EF_e22_medium2;   //!
   TBranch        *b_EF_e22_medium_IDTrkNoCut;   //!
   TBranch        *b_EF_e22_medium_SiTrk;   //!
   TBranch        *b_EF_e22_medium_TRT;   //!
   TBranch        *b_EF_e22_tight;   //!
   TBranch        *b_EF_e25_loose;   //!
   TBranch        *b_EF_e25_medium;   //!
   TBranch        *b_EF_e30_loose;   //!
   TBranch        *b_EF_e30_medium;   //!
   TBranch        *b_EF_e33_medium;   //!
   TBranch        *b_EF_e35_medium;   //!
   TBranch        *b_EF_e40_medium;   //!
   TBranch        *b_EF_e45_medium;   //!
   TBranch        *b_EF_e45_medium1;   //!
   TBranch        *b_EF_e5_NoCut_L2SW;   //!
   TBranch        *b_EF_e5_NoCut_Ringer;   //!
   TBranch        *b_EF_e5_NoCut_firstempty;   //!
   TBranch        *b_EF_e5_medium_mu4;   //!
   TBranch        *b_EF_e5_medium_mu6;   //!
   TBranch        *b_EF_e5_medium_mu6_topo_loose;   //!
   TBranch        *b_EF_e5_medium_mu6_topo_medium;   //!
   TBranch        *b_EF_e5_tight;   //!
   TBranch        *b_EF_e5_tight_TRT;   //!
   TBranch        *b_EF_e5_tight_e14_etcut_Jpsi;   //!
   TBranch        *b_EF_e5_tight_e4_etcut_Jpsi;   //!
   TBranch        *b_EF_e5_tight_e4_etcut_Jpsi_SiTrk;   //!
   TBranch        *b_EF_e5_tight_e4_etcut_Jpsi_TRT;   //!
   TBranch        *b_EF_e5_tight_e5_NoCut;   //!
   TBranch        *b_EF_e5_tight_e5_NoCut_Jpsi;   //!
   TBranch        *b_EF_e5_tight_e7_etcut_Jpsi;   //!
   TBranch        *b_EF_e5_tight_e9_etcut_Jpsi;   //!
   TBranch        *b_EF_e60_loose;   //!
   TBranch        *b_EF_e6T_medium;   //!
   TBranch        *b_EF_e7_tight_e14_etcut_Jpsi;   //!
   TBranch        *b_EF_e9_tight_e5_tight_Jpsi;   //!
   TBranch        *b_EF_eb_physics;   //!
   TBranch        *b_EF_eb_physics_empty;   //!
   TBranch        *b_EF_eb_physics_firstempty;   //!
   TBranch        *b_EF_eb_physics_noL1PS;   //!
   TBranch        *b_EF_eb_physics_unpaired_iso;   //!
   TBranch        *b_EF_eb_physics_unpaired_noniso;   //!
   TBranch        *b_EF_eb_random;   //!
   TBranch        *b_EF_eb_random_empty;   //!
   TBranch        *b_EF_eb_random_firstempty;   //!
   TBranch        *b_EF_eb_random_unpaired_iso;   //!
   TBranch        *b_EF_em105_passHLT;   //!
   TBranch        *b_EF_fj100_a4tc_EFFS;   //!
   TBranch        *b_EF_fj10_a4tc_EFFS;   //!
   TBranch        *b_EF_fj10_a4tc_EFFS_1vx;   //!
   TBranch        *b_EF_fj135_a4tc_EFFS;   //!
   TBranch        *b_EF_fj15_a4tc_EFFS;   //!
   TBranch        *b_EF_fj20_a4tc_EFFS;   //!
   TBranch        *b_EF_fj30_a4tc_EFFS;   //!
   TBranch        *b_EF_fj30_a4tc_EFFS_l2cleanph;   //!
   TBranch        *b_EF_fj55_a4tc_EFFS;   //!
   TBranch        *b_EF_fj75_a4tc_EFFS;   //!
   TBranch        *b_EF_g100_etcut_g50_etcut;   //!
   TBranch        *b_EF_g100_loose;   //!
   TBranch        *b_EF_g10_NoCut_cosmic;   //!
   TBranch        *b_EF_g11_etcut;   //!
   TBranch        *b_EF_g11_etcut_larcalib;   //!
   TBranch        *b_EF_g13_etcut_xs60_noMu;   //!
   TBranch        *b_EF_g150_etcut;   //!
   TBranch        *b_EF_g15_loose;   //!
   TBranch        *b_EF_g15_loose_larcalib;   //!
   TBranch        *b_EF_g17_etcut;   //!
   TBranch        *b_EF_g17_etcut_EFxe20_noMu;   //!
   TBranch        *b_EF_g17_etcut_EFxe30_noMu;   //!
   TBranch        *b_EF_g20_etcut;   //!
   TBranch        *b_EF_g20_etcut_xe30_noMu;   //!
   TBranch        *b_EF_g20_loose;   //!
   TBranch        *b_EF_g20_loose_cnv;   //!
   TBranch        *b_EF_g20_loose_larcalib;   //!
   TBranch        *b_EF_g20_loose_xe20_noMu;   //!
   TBranch        *b_EF_g20_loose_xe30_noMu;   //!
   TBranch        *b_EF_g20_tight;   //!
   TBranch        *b_EF_g22_hiptrt;   //!
   TBranch        *b_EF_g25_loose_xe30_noMu;   //!
   TBranch        *b_EF_g27_etcut;   //!
   TBranch        *b_EF_g27_etcut_EFxe30_noMu;   //!
   TBranch        *b_EF_g30_loose_g20_loose;   //!
   TBranch        *b_EF_g40_loose;   //!
   TBranch        *b_EF_g40_loose_EFxe35_noMu;   //!
   TBranch        *b_EF_g40_loose_EFxe40_noMu;   //!
   TBranch        *b_EF_g40_loose_b10_medium;   //!
   TBranch        *b_EF_g40_loose_larcalib;   //!
   TBranch        *b_EF_g40_loose_xe40_noMu;   //!
   TBranch        *b_EF_g40_loose_xe45_medium_noMu;   //!
   TBranch        *b_EF_g40_loose_xe55_medium_noMu;   //!
   TBranch        *b_EF_g40_loose_xs35_noMu;   //!
   TBranch        *b_EF_g40_tight;   //!
   TBranch        *b_EF_g40_tight_b10_medium;   //!
   TBranch        *b_EF_g40_tight_b15_medium;   //!
   TBranch        *b_EF_g50_loose;   //!
   TBranch        *b_EF_g5_NoCut_cosmic;   //!
   TBranch        *b_EF_g60_loose;   //!
   TBranch        *b_EF_g60_loose_larcalib;   //!
   TBranch        *b_EF_g80_loose;   //!
   TBranch        *b_EF_g80_loose_larcalib;   //!
   TBranch        *b_EF_hadCalib_trk9;   //!
   TBranch        *b_EF_hadCalib_trk9phi;   //!
   TBranch        *b_EF_high_eb_physics;   //!
   TBranch        *b_EF_ht350_a4tc_EFFS_L2je255;   //!
   TBranch        *b_EF_j100_a4tc_EFFS;   //!
   TBranch        *b_EF_j100_a4tc_EFFS_ht350;   //!
   TBranch        *b_EF_j100_a4tc_EFFS_ht400;   //!
   TBranch        *b_EF_j100_a4tc_EFFS_ht500;   //!
   TBranch        *b_EF_j100_j30_a4tc_EFFS_L2dphi04;   //!
   TBranch        *b_EF_j10_a4tc_EFFS;   //!
   TBranch        *b_EF_j10_a4tc_EFFS_1vx;   //!
   TBranch        *b_EF_j135_a4tc_EFFS;   //!
   TBranch        *b_EF_j135_a4tc_EFFS_ht500;   //!
   TBranch        *b_EF_j135_j30_a4tc_EFFS_L2dphi04;   //!
   TBranch        *b_EF_j135_j30_a4tc_EFFS_dphi04;   //!
   TBranch        *b_EF_j15_a4tc_EFFS;   //!
   TBranch        *b_EF_j180_a4tc_EFFS;   //!
   TBranch        *b_EF_j180_j30_a4tc_EFFS_dphi04;   //!
   TBranch        *b_EF_j20_a4tc_EFFS;   //!
   TBranch        *b_EF_j240_a10tc_EFFS;   //!
   TBranch        *b_EF_j240_a4tc_EFFS;   //!
   TBranch        *b_EF_j240_a4tc_EFFS_l2cleanph;   //!
   TBranch        *b_EF_j30_a4tc_EFFS;   //!
   TBranch        *b_EF_j30_a4tc_EFFS_l2cleanph;   //!
   TBranch        *b_EF_j30_cosmic;   //!
   TBranch        *b_EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_empty;   //!
   TBranch        *b_EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_firstempty;   //!
   TBranch        *b_EF_j30_firstempty;   //!
   TBranch        *b_EF_j30_fj30_a4tc_EFFS;   //!
   TBranch        *b_EF_j320_a10tc_EFFS;   //!
   TBranch        *b_EF_j320_a4tc_EFFS;   //!
   TBranch        *b_EF_j35_a4tc_EFFS;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HV;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HV_cosmic;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HV_firstempty;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_iso;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_noniso;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HVtrk;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HVtrk_LOF;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HVtrk_cosmic;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HVtrk_firstempty;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_iso;   //!
   TBranch        *b_EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_noniso;   //!
   TBranch        *b_EF_j40_a4tc_EFFS;   //!
   TBranch        *b_EF_j40_fj40_a4tc_EFFS;   //!
   TBranch        *b_EF_j425_a10tc_EFFS;   //!
   TBranch        *b_EF_j425_a4tc_EFFS;   //!
   TBranch        *b_EF_j45_a4tc_EFFS;   //!
   TBranch        *b_EF_j50_a4tc_EFFS;   //!
   TBranch        *b_EF_j50_cosmic;   //!
   TBranch        *b_EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_empty;   //!
   TBranch        *b_EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_firstempty;   //!
   TBranch        *b_EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_empty;   //!
   TBranch        *b_EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_firstempty;   //!
   TBranch        *b_EF_j50_firstempty;   //!
   TBranch        *b_EF_j55_a4tc_EFFS;   //!
   TBranch        *b_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10;   //!
   TBranch        *b_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10_l2cleancons;   //!
   TBranch        *b_EF_j55_fj55_a4tc_EFFS;   //!
   TBranch        *b_EF_j65_a4tc_EFFS_xe65_noMu_dphi2j30xe10;   //!
   TBranch        *b_EF_j70_j25_dphi03_NoEF;   //!
   TBranch        *b_EF_j75_2j30_a4tc_EFFS_ht350;   //!
   TBranch        *b_EF_j75_a4tc_EFFS;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe40_loose_noMu;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe45_loose_noMu;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe55_loose_noMu;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe55_noMu;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xe55_noMu_l2cleancons;   //!
   TBranch        *b_EF_j75_a4tc_EFFS_xs35_noMu;   //!
   TBranch        *b_EF_j75_fj75_a4tc_EFFS;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS_L2anymct100;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS_L2anymct150;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS_L2anymct175;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS_L2dphi04;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS_anymct150;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS_anymct175;   //!
   TBranch        *b_EF_j75_j30_a4tc_EFFS_leadingmct150;   //!
   TBranch        *b_EF_j80_a4tc_EFFS_xe60_noMu;   //!
   TBranch        *b_EF_je195_NoEF;   //!
   TBranch        *b_EF_je255_NoEF;   //!
   TBranch        *b_EF_je300_NoEF;   //!
   TBranch        *b_EF_je350_NoEF;   //!
   TBranch        *b_EF_je420_NoEF;   //!
   TBranch        *b_EF_je500_NoEF;   //!
   TBranch        *b_EF_l2j30_Trackless_HV;   //!
   TBranch        *b_EF_l2j30_Trackless_HV_L1MU10;   //!
   TBranch        *b_EF_l2j30_Trackless_HV_cosmic;   //!
   TBranch        *b_EF_l2j30_Trackless_HV_firstempty;   //!
   TBranch        *b_EF_l2j30_Trackless_HV_unpaired_iso;   //!
   TBranch        *b_EF_l2j30_Trackless_HV_unpaired_noniso;   //!
   TBranch        *b_EF_mbLucid_eff;   //!
   TBranch        *b_EF_mbMbts_2_eff_unpaired_iso;   //!
   TBranch        *b_EF_mbRd0_eff_empty;   //!
   TBranch        *b_EF_mbRd0_eff_unpaired_iso;   //!
   TBranch        *b_EF_mbRd1_eff;   //!
   TBranch        *b_EF_mbRmMbts_1_NoCut_Time;   //!
   TBranch        *b_EF_mbSpBg_unpaired_iso;   //!
   TBranch        *b_EF_mbSpTrk;   //!
   TBranch        *b_EF_mbSpTrkVtxMh;   //!
   TBranch        *b_EF_mbSpTrkVtxMh_eff;   //!
   TBranch        *b_EF_mbSpTrkVtxMh_medium;   //!
   TBranch        *b_EF_mbSpTrkVtxMh_medium_eff;   //!
   TBranch        *b_EF_mbSpTrkVtxMh_tight;   //!
   TBranch        *b_EF_mbSpTrkVtxMh_tight_eff;   //!
   TBranch        *b_EF_mbSpTrk_unpaired_iso;   //!
   TBranch        *b_EF_mbTE20_eff;   //!
   TBranch        *b_EF_mbZdc_eff;   //!
   TBranch        *b_EF_mb_BCM_Wide_eff;   //!
   TBranch        *b_EF_mu0_empty_NoAlg;   //!
   TBranch        *b_EF_mu0_firstempty_NoAlg;   //!
   TBranch        *b_EF_mu0_missingRoi;   //!
   TBranch        *b_EF_mu0_unpaired_iso_NoAlg;   //!
   TBranch        *b_EF_mu0_unpaired_noniso_NoAlg;   //!
   TBranch        *b_EF_mu10;   //!
   TBranch        *b_EF_mu100_MSonly;   //!
   TBranch        *b_EF_mu10_Jpsimumu;   //!
   TBranch        *b_EF_mu10_MG;   //!
   TBranch        *b_EF_mu10_NL;   //!
   TBranch        *b_EF_mu10_Upsimumu_FS;   //!
   TBranch        *b_EF_mu10_Upsimumu_tight_FS;   //!
   TBranch        *b_EF_mu10_loose;   //!
   TBranch        *b_EF_mu10_muCombTag_NoEF;   //!
   TBranch        *b_EF_mu11_empty_NoAlg;   //!
   TBranch        *b_EF_mu13;   //!
   TBranch        *b_EF_mu13_MG;   //!
   TBranch        *b_EF_mu13_muCombTag_NoEF;   //!
   TBranch        *b_EF_mu15;   //!
   TBranch        *b_EF_mu15_EFxe45;   //!
   TBranch        *b_EF_mu15_MG;   //!
   TBranch        *b_EF_mu15_j90_a4tc_EFFS;   //!
   TBranch        *b_EF_mu15_medium_EFxs65_noMu;   //!
   TBranch        *b_EF_mu15_medium_EFxs70_noMu;   //!
   TBranch        *b_EF_mu15_medium_EFxs75_noMu;   //!
   TBranch        *b_EF_mu15_mu10_EFFS;   //!
   TBranch        *b_EF_mu15_mu10_EFFS_medium;   //!
   TBranch        *b_EF_mu15_mu10_EFFS_tight;   //!
   TBranch        *b_EF_mu15_mu7_EFFS;   //!
   TBranch        *b_EF_mu15_xe30_noMu;   //!
   TBranch        *b_EF_mu15_xe35_noMu;   //!
   TBranch        *b_EF_mu15i;   //!
   TBranch        *b_EF_mu15i_medium;   //!
   TBranch        *b_EF_mu15ii_debug;   //!
   TBranch        *b_EF_mu18;   //!
   TBranch        *b_EF_mu18_L1J10;   //!
   TBranch        *b_EF_mu18_MG;   //!
   TBranch        *b_EF_mu18_MG_L1J10;   //!
   TBranch        *b_EF_mu18_MG_medium;   //!
   TBranch        *b_EF_mu18_medium;   //!
   TBranch        *b_EF_mu20;   //!
   TBranch        *b_EF_mu20_IDTrkNoCut;   //!
   TBranch        *b_EF_mu20_IDTrkNoCut_ManyVx;   //!
   TBranch        *b_EF_mu20_L2Star_IDTrkNoCut;   //!
   TBranch        *b_EF_mu20_MG;   //!
   TBranch        *b_EF_mu20_MG_medium;   //!
   TBranch        *b_EF_mu20_MSonly;   //!
   TBranch        *b_EF_mu20_empty;   //!
   TBranch        *b_EF_mu20_medium;   //!
   TBranch        *b_EF_mu20_mu10_EFFS_tight;   //!
   TBranch        *b_EF_mu20_mu7_EFFS;   //!
   TBranch        *b_EF_mu20_muCombTag_NoEF;   //!
   TBranch        *b_EF_mu20i;   //!
   TBranch        *b_EF_mu20i_medium;   //!
   TBranch        *b_EF_mu22;   //!
   TBranch        *b_EF_mu22_MG;   //!
   TBranch        *b_EF_mu22_MG_medium;   //!
   TBranch        *b_EF_mu22_medium;   //!
   TBranch        *b_EF_mu24_MG_medium;   //!
   TBranch        *b_EF_mu24_MG_tight;   //!
   TBranch        *b_EF_mu24_medium;   //!
   TBranch        *b_EF_mu24_tight;   //!
   TBranch        *b_EF_mu30_MG_medium;   //!
   TBranch        *b_EF_mu30_MG_tight;   //!
   TBranch        *b_EF_mu30_medium;   //!
   TBranch        *b_EF_mu30_tight;   //!
   TBranch        *b_EF_mu4;   //!
   TBranch        *b_EF_mu40_MSonly;   //!
   TBranch        *b_EF_mu40_MSonly_barrel;   //!
   TBranch        *b_EF_mu40_MSonly_barrel_medium;   //!
   TBranch        *b_EF_mu40_MSonly_empty;   //!
   TBranch        *b_EF_mu40_MSonly_tight;   //!
   TBranch        *b_EF_mu40_MSonly_tight_L1MU11;   //!
   TBranch        *b_EF_mu40_MSonly_tighter;   //!
   TBranch        *b_EF_mu40_slow;   //!
   TBranch        *b_EF_mu40_slow_empty;   //!
   TBranch        *b_EF_mu40_slow_medium;   //!
   TBranch        *b_EF_mu40_slow_outOfTime;   //!
   TBranch        *b_EF_mu40_slow_outOfTime_medium;   //!
   TBranch        *b_EF_mu4_Bmumu;   //!
   TBranch        *b_EF_mu4_BmumuX;   //!
   TBranch        *b_EF_mu4_DiMu;   //!
   TBranch        *b_EF_mu4_DiMu_FS;   //!
   TBranch        *b_EF_mu4_DiMu_FS_noOS;   //!
   TBranch        *b_EF_mu4_DiMu_MG;   //!
   TBranch        *b_EF_mu4_DiMu_MG_FS;   //!
   TBranch        *b_EF_mu4_DiMu_SiTrk;   //!
   TBranch        *b_EF_mu4_DiMu_SiTrk_FS;   //!
   TBranch        *b_EF_mu4_DiMu_noOS;   //!
   TBranch        *b_EF_mu4_Jpsimumu;   //!
   TBranch        *b_EF_mu4_Jpsimumu_FS;   //!
   TBranch        *b_EF_mu4_Jpsimumu_tight;   //!
   TBranch        *b_EF_mu4_L1J10_matched;   //!
   TBranch        *b_EF_mu4_L1J15_matched;   //!
   TBranch        *b_EF_mu4_L1J20_matched;   //!
   TBranch        *b_EF_mu4_L1J30_matched;   //!
   TBranch        *b_EF_mu4_L1J50_matched;   //!
   TBranch        *b_EF_mu4_L1J75_matched;   //!
   TBranch        *b_EF_mu4_L1MU11_MSonly_cosmic;   //!
   TBranch        *b_EF_mu4_L1MU11_cosmic;   //!
   TBranch        *b_EF_mu4_MSonly_cosmic;   //!
   TBranch        *b_EF_mu4_Trk_Jpsi;   //!
   TBranch        *b_EF_mu4_Trk_Jpsi_loose;   //!
   TBranch        *b_EF_mu4_Trk_Jpsi_looser;   //!
   TBranch        *b_EF_mu4_Trk_Upsi_FS;   //!
   TBranch        *b_EF_mu4_Upsimumu_FS;   //!
   TBranch        *b_EF_mu4_Upsimumu_SiTrk_FS;   //!
   TBranch        *b_EF_mu4_Upsimumu_tight_FS;   //!
   TBranch        *b_EF_mu4_cosmic;   //!
   TBranch        *b_EF_mu4_firstempty;   //!
   TBranch        *b_EF_mu4_j10_a4tc_EFFS;   //!
   TBranch        *b_EF_mu4_j10_a4tc_EFFS_matched;   //!
   TBranch        *b_EF_mu4_j135_a4tc_EFFS_L1matched;   //!
   TBranch        *b_EF_mu4_j180_a4tc_EFFS_L1matched;   //!
   TBranch        *b_EF_mu4_j20_a4tc_EFFS_matched;   //!
   TBranch        *b_EF_mu4_j30;   //!
   TBranch        *b_EF_mu4_j45_a4tc_EFFS_xe45_loose_noMu;   //!
   TBranch        *b_EF_mu4_mu6;   //!
   TBranch        *b_EF_mu4_unpaired_iso;   //!
   TBranch        *b_EF_mu4imu6i_DiMu_DY;   //!
   TBranch        *b_EF_mu4imu6i_DiMu_DY14_noVtx_noOS;   //!
   TBranch        *b_EF_mu4imu6i_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_EF_mu4mu6_Bmumu;   //!
   TBranch        *b_EF_mu4mu6_Bmumux;   //!
   TBranch        *b_EF_mu4mu6_DiMu;   //!
   TBranch        *b_EF_mu4mu6_DiMu_DY;   //!
   TBranch        *b_EF_mu4mu6_DiMu_DY20;   //!
   TBranch        *b_EF_mu4mu6_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_EF_mu4mu6_DiMu_noVtx_noOS;   //!
   TBranch        *b_EF_mu4mu6_Jpsimumu;   //!
   TBranch        *b_EF_mu4mu6_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_EF_mu4mu6_Upsimumu;   //!
   TBranch        *b_EF_mu6;   //!
   TBranch        *b_EF_mu60_MSonly;   //!
   TBranch        *b_EF_mu6_Bmumu;   //!
   TBranch        *b_EF_mu6_BmumuX;   //!
   TBranch        *b_EF_mu6_DiMu;   //!
   TBranch        *b_EF_mu6_DiMu_noOS;   //!
   TBranch        *b_EF_mu6_Jpsimumu;   //!
   TBranch        *b_EF_mu6_Jpsimumu_SiTrk;   //!
   TBranch        *b_EF_mu6_Jpsimumu_tight;   //!
   TBranch        *b_EF_mu6_Trk_Jpsi_loose;   //!
   TBranch        *b_EF_mu6_Upsimumu_FS;   //!
   TBranch        *b_EF_mu6_Upsimumu_tight_FS;   //!
   TBranch        *b_EF_mu80_MSonly;   //!
   TBranch        *b_EF_rd0_empty_NoAlg;   //!
   TBranch        *b_EF_rd0_filled_NoAlg;   //!
   TBranch        *b_EF_rd1_empty_NoAlg;   //!
   TBranch        *b_EF_tau100_loose;   //!
   TBranch        *b_EF_tau100_medium;   //!
   TBranch        *b_EF_tau125_loose;   //!
   TBranch        *b_EF_tau125_medium;   //!
   TBranch        *b_EF_tau125_medium1;   //!
   TBranch        *b_EF_tau16_IDTrkNoCut;   //!
   TBranch        *b_EF_tau16_L2Star_IDTrkNoCut;   //!
   TBranch        *b_EF_tau16_j75_a4tc_EFFS_xe40_loose_noMu;   //!
   TBranch        *b_EF_tau16_loose;   //!
   TBranch        *b_EF_tau16_loose_e10_loose;   //!
   TBranch        *b_EF_tau16_loose_e15_loose;   //!
   TBranch        *b_EF_tau16_loose_e15_medium;   //!
   TBranch        *b_EF_tau16_loose_e15_tight;   //!
   TBranch        *b_EF_tau16_loose_mu10;   //!
   TBranch        *b_EF_tau16_loose_mu15;   //!
   TBranch        *b_EF_tau16_loose_xe20_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe25_noMu;   //!
   TBranch        *b_EF_tau16_loose_xe30_noMu;   //!
   TBranch        *b_EF_tau16_medium;   //!
   TBranch        *b_EF_tau16_medium_e15_medium;   //!
   TBranch        *b_EF_tau16_medium_e15_tight;   //!
   TBranch        *b_EF_tau16_medium_mu10;   //!
   TBranch        *b_EF_tau16_medium_mu15;   //!
   TBranch        *b_EF_tau16_medium_xe22_noMu;   //!
   TBranch        *b_EF_tau16_medium_xe25_noMu;   //!
   TBranch        *b_EF_tau16_medium_xe25_tight_noMu;   //!
   TBranch        *b_EF_tau20T_medium;   //!
   TBranch        *b_EF_tau20T_medium_e15_medium;   //!
   TBranch        *b_EF_tau20T_medium_e15_tight;   //!
   TBranch        *b_EF_tau20T_medium_mu15;   //!
   TBranch        *b_EF_tau20_loose;   //!
   TBranch        *b_EF_tau20_loose_e15_medium;   //!
   TBranch        *b_EF_tau20_loose_mu15;   //!
   TBranch        *b_EF_tau20_loose_tau29_loose;   //!
   TBranch        *b_EF_tau20_medium;   //!
   TBranch        *b_EF_tau20_medium1;   //!
   TBranch        *b_EF_tau20_medium_e15_medium;   //!
   TBranch        *b_EF_tau20_medium_e15_tight;   //!
   TBranch        *b_EF_tau20_medium_mu15;   //!
   TBranch        *b_EF_tau29T_loose_xs75_noMu;   //!
   TBranch        *b_EF_tau29T_medium;   //!
   TBranch        *b_EF_tau29T_medium1_tau20T_medium1;   //!
   TBranch        *b_EF_tau29T_medium1_xs45_loose_noMu_3L1J10;   //!
   TBranch        *b_EF_tau29T_medium_xs70_noMu;   //!
   TBranch        *b_EF_tau29T_medium_xs75_loose_noMu;   //!
   TBranch        *b_EF_tau29T_medium_xs75_noMu;   //!
   TBranch        *b_EF_tau29_loose;   //!
   TBranch        *b_EF_tau29_loose1;   //!
   TBranch        *b_EF_tau29_loose1_xs45_loose_noMu_3L1J10;   //!
   TBranch        *b_EF_tau29_loose_e15_tight;   //!
   TBranch        *b_EF_tau29_loose_j45_a4tc_EFFS_xe40_noMu;   //!
   TBranch        *b_EF_tau29_loose_mu15;   //!
   TBranch        *b_EF_tau29_loose_tau38_loose;   //!
   TBranch        *b_EF_tau29_loose_tau50_loose;   //!
   TBranch        *b_EF_tau29_loose_xs45_loose_noMu_3L1J10;   //!
   TBranch        *b_EF_tau29_loose_xs60_loose_noMu_3L1J10;   //!
   TBranch        *b_EF_tau29_loose_xs65_loose_noMu;   //!
   TBranch        *b_EF_tau29_loose_xs70_loose_noMu;   //!
   TBranch        *b_EF_tau29_loose_xs75_loose_noMu;   //!
   TBranch        *b_EF_tau29_loose_xs75_noMu;   //!
   TBranch        *b_EF_tau29_loose_xs80_loose_noMu;   //!
   TBranch        *b_EF_tau29_medium;   //!
   TBranch        *b_EF_tau29_medium1;   //!
   TBranch        *b_EF_tau29_medium1_tau20_loose_1prong;   //!
   TBranch        *b_EF_tau29_medium1_tau20_medium1;   //!
   TBranch        *b_EF_tau29_medium1_xs45_loose_noMu_3L1J10;   //!
   TBranch        *b_EF_tau29_medium_tau20_loose_1prong;   //!
   TBranch        *b_EF_tau29_medium_tau38_loose;   //!
   TBranch        *b_EF_tau29_medium_tau38_medium;   //!
   TBranch        *b_EF_tau29_medium_xe35_noMu;   //!
   TBranch        *b_EF_tau29_medium_xe40_loose_noMu;   //!
   TBranch        *b_EF_tau29_medium_xs45_loose_noMu_3L1J10;   //!
   TBranch        *b_EF_tau29_medium_xs70_noMu;   //!
   TBranch        *b_EF_tau29_medium_xs75_noMu;   //!
   TBranch        *b_EF_tau29_medium_xs80_loose_noMu;   //!
   TBranch        *b_EF_tau38T_medium1_tau29T_medium1;   //!
   TBranch        *b_EF_tau38_loose;   //!
   TBranch        *b_EF_tau38_medium;   //!
   TBranch        *b_EF_tau38_medium1_tau29_medium1;   //!
   TBranch        *b_EF_tau50_IDTrkNoCut;   //!
   TBranch        *b_EF_tau50_loose;   //!
   TBranch        *b_EF_tau50_loose_IdScan;   //!
   TBranch        *b_EF_tau50_medium;   //!
   TBranch        *b_EF_tau84_loose;   //!
   TBranch        *b_EF_tau84_medium;   //!
   TBranch        *b_EF_tauNoCut;   //!
   TBranch        *b_EF_tauNoCut_L1TAU50;   //!
   TBranch        *b_EF_tauNoCut_cosmic;   //!
   TBranch        *b_EF_tauNoCut_firstempty;   //!
   TBranch        *b_EF_tauNoCut_unpaired_iso;   //!
   TBranch        *b_EF_tauNoCut_unpaired_noniso;   //!
   TBranch        *b_EF_te1000;   //!
   TBranch        *b_EF_te1100;   //!
   TBranch        *b_EF_te1200;   //!
   TBranch        *b_EF_te550;   //!
   TBranch        *b_EF_te700;   //!
   TBranch        *b_EF_te900;   //!
   TBranch        *b_EF_xe20_noMu;   //!
   TBranch        *b_EF_xe30_noMu;   //!
   TBranch        *b_EF_xe35_noMu;   //!
   TBranch        *b_EF_xe40_noMu;   //!
   TBranch        *b_EF_xe50_noMu;   //!
   TBranch        *b_EF_xe60_noMu;   //!
   TBranch        *b_EF_xe60_tight_noMu;   //!
   TBranch        *b_EF_xe60_verytight_noMu;   //!
   TBranch        *b_EF_xe70_noMu;   //!
   TBranch        *b_EF_xe70_tight_noMu;   //!
   TBranch        *b_EF_xe80_noMu;   //!
   TBranch        *b_EF_xe90_noMu;   //!
   TBranch        *b_EF_xe_NoCut_allL1;   //!
   TBranch        *b_EF_xs100_noMu;   //!
   TBranch        *b_EF_xs30_noMu;   //!
   TBranch        *b_EF_xs45_noMu;   //!
   TBranch        *b_EF_xs60_noMu;   //!
   TBranch        *b_EF_xs60_noMu_L1EM10XS45;   //!
   TBranch        *b_EF_xs70_noMu;   //!
   TBranch        *b_EF_xs70_tight_noMu;   //!
   TBranch        *b_EF_xs75_noMu;   //!
   TBranch        *b_EF_xs75_noMu_L1EM10XS50;   //!
   TBranch        *b_EF_xs90_noMu;   //!
   TBranch        *b_EF_zerobias_NoAlg;   //!
   TBranch        *b_L1_2EM10;   //!
   TBranch        *b_L1_2EM14;   //!
   TBranch        *b_L1_2EM3;   //!
   TBranch        *b_L1_2EM3_EM7;   //!
   TBranch        *b_L1_2EM5;   //!
   TBranch        *b_L1_2EM7;   //!
   TBranch        *b_L1_2J10_J30_JE140;   //!
   TBranch        *b_L1_2J10_J50;   //!
   TBranch        *b_L1_2J10_J75;   //!
   TBranch        *b_L1_2J20_XE20;   //!
   TBranch        *b_L1_2J30_XE20;   //!
   TBranch        *b_L1_2MU0;   //!
   TBranch        *b_L1_2MU0_XE20;   //!
   TBranch        *b_L1_2MU0_XE25;   //!
   TBranch        *b_L1_2MU10;   //!
   TBranch        *b_L1_2MU6;   //!
   TBranch        *b_L1_2TAU11;   //!
   TBranch        *b_L1_2TAU11I;   //!
   TBranch        *b_L1_2TAU11_EM10;   //!
   TBranch        *b_L1_2TAU11_TAU15;   //!
   TBranch        *b_L1_2TAU15;   //!
   TBranch        *b_L1_2TAU15_TAU20;   //!
   TBranch        *b_L1_2TAU6;   //!
   TBranch        *b_L1_2TAU6_EM10;   //!
   TBranch        *b_L1_2TAU6_EM5;   //!
   TBranch        *b_L1_2TAU8_EM10;   //!
   TBranch        *b_L1_2TAU8_TAU11;   //!
   TBranch        *b_L1_3EM5;   //!
   TBranch        *b_L1_3J10;   //!
   TBranch        *b_L1_3J10_J50;   //!
   TBranch        *b_L1_3J15;   //!
   TBranch        *b_L1_3J50;   //!
   TBranch        *b_L1_4J10;   //!
   TBranch        *b_L1_EM10;   //!
   TBranch        *b_L1_EM10_XE20;   //!
   TBranch        *b_L1_EM10_XE30;   //!
   TBranch        *b_L1_EM12;   //!
   TBranch        *b_L1_EM14;   //!
   TBranch        *b_L1_EM14_XE10;   //!
   TBranch        *b_L1_EM14_XE20;   //!
   TBranch        *b_L1_EM16;   //!
   TBranch        *b_L1_EM30;   //!
   TBranch        *b_L1_J10;   //!
   TBranch        *b_L1_J15;   //!
   TBranch        *b_L1_J175;   //!
   TBranch        *b_L1_J20;   //!
   TBranch        *b_L1_J250;   //!
   TBranch        *b_L1_J30;   //!
   TBranch        *b_L1_J30_XE35;   //!
   TBranch        *b_L1_J30_XE40;   //!
   TBranch        *b_L1_J50;   //!
   TBranch        *b_L1_J50_XE20;   //!
   TBranch        *b_L1_J50_XE30;   //!
   TBranch        *b_L1_J50_XE35;   //!
   TBranch        *b_L1_J50_XE40;   //!
   TBranch        *b_L1_J75;   //!
   TBranch        *b_L1_JE100;   //!
   TBranch        *b_L1_JE140;   //!
   TBranch        *b_L1_JE200;   //!
   TBranch        *b_L1_MU0;   //!
   TBranch        *b_L1_MU0_J10;   //!
   TBranch        *b_L1_MU0_J20_XE20;   //!
   TBranch        *b_L1_MU10;   //!
   TBranch        *b_L1_MU10_XE20;   //!
   TBranch        *b_L1_MU11;   //!
   TBranch        *b_L1_MU15;   //!
   TBranch        *b_L1_MU20;   //!
   TBranch        *b_L1_MU6;   //!
   TBranch        *b_L1_TAU11;   //!
   TBranch        *b_L1_TAU11_2J10_J20_XE20;   //!
   TBranch        *b_L1_TAU11_MU10;   //!
   TBranch        *b_L1_TAU11_XE10_3J10;   //!
   TBranch        *b_L1_TAU11_XE20;   //!
   TBranch        *b_L1_TAU11_XS30;   //!
   TBranch        *b_L1_TAU11_XS35;   //!
   TBranch        *b_L1_TAU15;   //!
   TBranch        *b_L1_TAU15_XE10_3J10;   //!
   TBranch        *b_L1_TAU15_XE20;   //!
   TBranch        *b_L1_TAU15_XS35;   //!
   TBranch        *b_L1_TAU20;   //!
   TBranch        *b_L1_TAU30;   //!
   TBranch        *b_L1_TAU50;   //!
   TBranch        *b_L1_TAU6;   //!
   TBranch        *b_L1_TAU6_J50_XE20;   //!
   TBranch        *b_L1_TAU6_MU10;   //!
   TBranch        *b_L1_TAU6_XE10;   //!
   TBranch        *b_L1_TAU8;   //!
   TBranch        *b_L1_TAU8_EMPTY;   //!
   TBranch        *b_L1_TAU8_FIRSTEMPTY;   //!
   TBranch        *b_L1_TAU8_MU10;   //!
   TBranch        *b_L1_TAU8_UNPAIRED_ISO;   //!
   TBranch        *b_L1_TAU8_UNPAIRED_NONISO;   //!
   TBranch        *b_L1_XE10;   //!
   TBranch        *b_L1_XE20;   //!
   TBranch        *b_L1_XE25;   //!
   TBranch        *b_L1_XE30;   //!
   TBranch        *b_L1_XE35;   //!
   TBranch        *b_L1_XE40;   //!
   TBranch        *b_L1_XE50;   //!
   TBranch        *b_L1_XE60;   //!
   TBranch        *b_L1_XS15;   //!
   TBranch        *b_L1_XS20;   //!
   TBranch        *b_L1_XS25;   //!
   TBranch        *b_L1_XS30;   //!
   TBranch        *b_L1_XS35;   //!
   TBranch        *b_L1_XS40;   //!
   TBranch        *b_L1_XS45;   //!
   TBranch        *b_L1_XS50;   //!
   TBranch        *b_L2_2b10_medium_3L1J10;   //!
   TBranch        *b_L2_2b10_medium_4L1J10;   //!
   TBranch        *b_L2_2b10_medium_4j25;   //!
   TBranch        *b_L2_2b10_medium_L1JE100;   //!
   TBranch        *b_L2_2b10_medium_L1JE140;   //!
   TBranch        *b_L2_2b10_medium_L1_2J10J50;   //!
   TBranch        *b_L2_2b10_medium_j70_j25;   //!
   TBranch        *b_L2_2b10_medium_j95_j25;   //!
   TBranch        *b_L2_2b10_tight_4j25;   //!
   TBranch        *b_L2_2b15_medium_3L1J15;   //!
   TBranch        *b_L2_2b15_medium_3j35;   //!
   TBranch        *b_L2_2b15_medium_j70_j35;   //!
   TBranch        *b_L2_2b20_medium_3L1J20;   //!
   TBranch        *b_L2_2b20_medium_3j40;   //!
   TBranch        *b_L2_2b20_medium_j70_j40;   //!
   TBranch        *b_L2_2e10_medium;   //!
   TBranch        *b_L2_2e12_medium;   //!
   TBranch        *b_L2_2g15_loose;   //!
   TBranch        *b_L2_2g20_loose;   //!
   TBranch        *b_L2_2j25_j70_dphi03;   //!
   TBranch        *b_L2_2j40_anymct100_xe20_medium_noMu;   //!
   TBranch        *b_L2_2j50_anymct100_xe20_medium_noMu;   //!
   TBranch        *b_L2_2mu10;   //!
   TBranch        *b_L2_2mu4;   //!
   TBranch        *b_L2_2mu4_xe20_noMu;   //!
   TBranch        *b_L2_2mu4_xe25_noMu;   //!
   TBranch        *b_L2_2tau16_loose;   //!
   TBranch        *b_L2_2tau29T_medium1;   //!
   TBranch        *b_L2_2tau29_loose;   //!
   TBranch        *b_L2_2tau29_loose1;   //!
   TBranch        *b_L2_2tau29_loose1_EFxe15_noMu;   //!
   TBranch        *b_L2_2tau29_loose1_EFxe20_noMu;   //!
   TBranch        *b_L2_2tau29_medium1;   //!
   TBranch        *b_L2_2tau29i_medium1;   //!
   TBranch        *b_L2_2tau38_loose;   //!
   TBranch        *b_L2_3b10_loose_4L1J10;   //!
   TBranch        *b_L2_3b10_medium_4j25;   //!
   TBranch        *b_L2_3b15_loose_4L1J15;   //!
   TBranch        *b_L2_3b15_medium_4j35;   //!
   TBranch        *b_L2_3e10_medium;   //!
   TBranch        *b_L2_3j25;   //!
   TBranch        *b_L2_3j70;   //!
   TBranch        *b_L2_4j25;   //!
   TBranch        *b_L2_5j25;   //!
   TBranch        *b_L2_6j25;   //!
   TBranch        *b_L2_MU0_j40_xe20_loose_noMu;   //!
   TBranch        *b_L2_b10_IDTrkNoCut;   //!
   TBranch        *b_L2_b10_L2Star_IDTrkNoCut;   //!
   TBranch        *b_L2_b10_medium_4L1J10;   //!
   TBranch        *b_L2_b10_medium_4j25;   //!
   TBranch        *b_L2_b10_medium_EFxe25_noMu_L1JE100;   //!
   TBranch        *b_L2_b10_medium_EFxe25_noMu_L1JE140;   //!
   TBranch        *b_L2_b10_medium_EFxe25_noMu_L1_2J10J50;   //!
   TBranch        *b_L2_b10_medium_j70_2j50_4j25;   //!
   TBranch        *b_L2_b10_tight_4L1J10;   //!
   TBranch        *b_L2_b10_tight_4j25;   //!
   TBranch        *b_L2_b10_tight_L1JE100;   //!
   TBranch        *b_L2_b10_tight_L1JE140;   //!
   TBranch        *b_L2_b10_tight_j70_2j50_4j25;   //!
   TBranch        *b_L2_b15_IDTrkNoCut;   //!
   TBranch        *b_L2_b20_IDTrkNoCut;   //!
   TBranch        *b_L2_e10_medium;   //!
   TBranch        *b_L2_e15_medium;   //!
   TBranch        *b_L2_e15_medium_xe20_noMu;   //!
   TBranch        *b_L2_e15_medium_xe25_noMu;   //!
   TBranch        *b_L2_e15_medium_xe30_noMu;   //!
   TBranch        *b_L2_e15_medium_xe35_noMu;   //!
   TBranch        *b_L2_e15_medium_xe40_loose_noMu;   //!
   TBranch        *b_L2_e15_tight;   //!
   TBranch        *b_L2_e15_tight_xe30_noMu;   //!
   TBranch        *b_L2_e18_medium_xe20_noMu;   //!
   TBranch        *b_L2_e20_loose;   //!
   TBranch        *b_L2_e20_loose_xe20;   //!
   TBranch        *b_L2_e20_loose_xe20_noMu;   //!
   TBranch        *b_L2_e20_loose_xe30;   //!
   TBranch        *b_L2_e20_loose_xe30_noMu;   //!
   TBranch        *b_L2_e20_medium;   //!
   TBranch        *b_L2_e20_medium1;   //!
   TBranch        *b_L2_e22_medium;   //!
   TBranch        *b_L2_e60_loose;   //!
   TBranch        *b_L2_g11_etcut;   //!
   TBranch        *b_L2_g150_etcut;   //!
   TBranch        *b_L2_g15_loose;   //!
   TBranch        *b_L2_g20_etcut;   //!
   TBranch        *b_L2_g20_etcut_xe30_noMu;   //!
   TBranch        *b_L2_g20_loose;   //!
   TBranch        *b_L2_g20_loose_xe20_noMu;   //!
   TBranch        *b_L2_g20_loose_xe30_noMu;   //!
   TBranch        *b_L2_g25_loose_xe30_noMu;   //!
   TBranch        *b_L2_g40_loose;   //!
   TBranch        *b_L2_g40_loose_xe30_medium_noMu;   //!
   TBranch        *b_L2_g40_loose_xe35_medium_noMu;   //!
   TBranch        *b_L2_g40_loose_xe40_noMu;   //!
   TBranch        *b_L2_g60_loose;   //!
   TBranch        *b_L2_g80_loose;   //!
   TBranch        *b_L2_j25;   //!
   TBranch        *b_L2_j35;   //!
   TBranch        *b_L2_j50;   //!
   TBranch        *b_L2_j50_xe35_medium_noMu;   //!
   TBranch        *b_L2_j50_xe35_medium_noMu_l2cleancons;   //!
   TBranch        *b_L2_j60_xe45_noMu;   //!
   TBranch        *b_L2_j70;   //!
   TBranch        *b_L2_j70_xe20_loose_noMu;   //!
   TBranch        *b_L2_j70_xe25_loose_noMu;   //!
   TBranch        *b_L2_j70_xe35_noMu;   //!
   TBranch        *b_L2_j70_xe35_noMu_l2cleancons;   //!
   TBranch        *b_L2_j75_xe40_noMu;   //!
   TBranch        *b_L2_j95;   //!
   TBranch        *b_L2_je255;   //!
   TBranch        *b_L2_mu10;   //!
   TBranch        *b_L2_mu13;   //!
   TBranch        *b_L2_mu13_MG;   //!
   TBranch        *b_L2_mu15;   //!
   TBranch        *b_L2_mu15_xe20_noMu;   //!
   TBranch        *b_L2_mu18_MG;   //!
   TBranch        *b_L2_mu20;   //!
   TBranch        *b_L2_mu20_MG;   //!
   TBranch        *b_L2_mu20_MSonly;   //!
   TBranch        *b_L2_mu22_MG;   //!
   TBranch        *b_L2_mu4;   //!
   TBranch        *b_L2_mu4_L1J10_matched;   //!
   TBranch        *b_L2_mu4_L1J20_matched;   //!
   TBranch        *b_L2_mu4_L1J30_matched;   //!
   TBranch        *b_L2_mu4_L1J50_matched;   //!
   TBranch        *b_L2_mu4_L1J75_matched;   //!
   TBranch        *b_L2_mu4_j10_a4tc_EFFS;   //!
   TBranch        *b_L2_mu4_j40_xe20_loose_noMu;   //!
   TBranch        *b_L2_mu6;   //!
   TBranch        *b_L2_tau100_loose;   //!
   TBranch        *b_L2_tau100_medium;   //!
   TBranch        *b_L2_tau125_loose;   //!
   TBranch        *b_L2_tau125_medium;   //!
   TBranch        *b_L2_tau125_medium1;   //!
   TBranch        *b_L2_tau16_IDTrkNoCut;   //!
   TBranch        *b_L2_tau16_L2Star_IDTrkNoCut;   //!
   TBranch        *b_L2_tau16_j70_xe20_loose_noMu;   //!
   TBranch        *b_L2_tau16_loose;   //!
   TBranch        *b_L2_tau16_loose_e10_loose;   //!
   TBranch        *b_L2_tau16_loose_e15_loose;   //!
   TBranch        *b_L2_tau16_loose_e15_medium;   //!
   TBranch        *b_L2_tau16_loose_e15_tight;   //!
   TBranch        *b_L2_tau16_loose_mu10;   //!
   TBranch        *b_L2_tau16_loose_mu15;   //!
   TBranch        *b_L2_tau16_loose_xe20_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe25_noMu;   //!
   TBranch        *b_L2_tau16_loose_xe30_noMu;   //!
   TBranch        *b_L2_tau16_medium;   //!
   TBranch        *b_L2_tau16_medium_e15_medium;   //!
   TBranch        *b_L2_tau16_medium_e15_tight;   //!
   TBranch        *b_L2_tau16_medium_mu10;   //!
   TBranch        *b_L2_tau16_medium_mu15;   //!
   TBranch        *b_L2_tau16_medium_xe22_noMu;   //!
   TBranch        *b_L2_tau16_medium_xe25_noMu;   //!
   TBranch        *b_L2_tau16_medium_xe25_tight_noMu;   //!
   TBranch        *b_L2_tau20T_medium;   //!
   TBranch        *b_L2_tau20T_medium_e15_medium;   //!
   TBranch        *b_L2_tau20T_medium_e15_tight;   //!
   TBranch        *b_L2_tau20T_medium_mu15;   //!
   TBranch        *b_L2_tau20_loose;   //!
   TBranch        *b_L2_tau20_loose_e15_medium;   //!
   TBranch        *b_L2_tau20_loose_mu15;   //!
   TBranch        *b_L2_tau20_loose_tau29_loose;   //!
   TBranch        *b_L2_tau20_medium;   //!
   TBranch        *b_L2_tau20_medium1;   //!
   TBranch        *b_L2_tau20_medium_e15_medium;   //!
   TBranch        *b_L2_tau20_medium_e15_tight;   //!
   TBranch        *b_L2_tau20_medium_mu15;   //!
   TBranch        *b_L2_tau29T_loose_xs50_noMu;   //!
   TBranch        *b_L2_tau29T_medium;   //!
   TBranch        *b_L2_tau29T_medium1_tau20T_medium1;   //!
   TBranch        *b_L2_tau29T_medium1_xs20_noMu_3L1J10;   //!
   TBranch        *b_L2_tau29T_medium_xs35_noMu;   //!
   TBranch        *b_L2_tau29T_medium_xs50_noMu;   //!
   TBranch        *b_L2_tau29T_medium_xs50_noMu_1;   //!
   TBranch        *b_L2_tau29_loose;   //!
   TBranch        *b_L2_tau29_loose1;   //!
   TBranch        *b_L2_tau29_loose1_xs20_noMu_3L1J10;   //!
   TBranch        *b_L2_tau29_loose_e15_tight;   //!
   TBranch        *b_L2_tau29_loose_j40_xe20_noMu;   //!
   TBranch        *b_L2_tau29_loose_mu15;   //!
   TBranch        *b_L2_tau29_loose_tau38_loose;   //!
   TBranch        *b_L2_tau29_loose_tau50_loose;   //!
   TBranch        *b_L2_tau29_loose_xs20_noMu_3L1J10;   //!
   TBranch        *b_L2_tau29_loose_xs35_noMu;   //!
   TBranch        *b_L2_tau29_loose_xs40_noMu;   //!
   TBranch        *b_L2_tau29_loose_xs50_noMu;   //!
   TBranch        *b_L2_tau29_medium;   //!
   TBranch        *b_L2_tau29_medium1;   //!
   TBranch        *b_L2_tau29_medium1_tau20_loose_1prong;   //!
   TBranch        *b_L2_tau29_medium1_tau20_medium1;   //!
   TBranch        *b_L2_tau29_medium1_xs20_noMu_3L1J10;   //!
   TBranch        *b_L2_tau29_medium_tau20_loose_1prong;   //!
   TBranch        *b_L2_tau29_medium_tau38_loose;   //!
   TBranch        *b_L2_tau29_medium_tau38_medium;   //!
   TBranch        *b_L2_tau29_medium_xe25_noMu;   //!
   TBranch        *b_L2_tau29_medium_xe30_loose_noMu;   //!
   TBranch        *b_L2_tau29_medium_xs20_noMu_3L1J10;   //!
   TBranch        *b_L2_tau29_medium_xs50_noMu;   //!
   TBranch        *b_L2_tau29_medium_xs50_noMu_1;   //!
   TBranch        *b_L2_tau38T_medium1_tau29T_medium1;   //!
   TBranch        *b_L2_tau38_loose;   //!
   TBranch        *b_L2_tau38_medium;   //!
   TBranch        *b_L2_tau38_medium1_tau29_medium1;   //!
   TBranch        *b_L2_tau50_IDTrkNoCut;   //!
   TBranch        *b_L2_tau50_loose;   //!
   TBranch        *b_L2_tau50_loose_IdScan;   //!
   TBranch        *b_L2_tau50_medium;   //!
   TBranch        *b_L2_tau84_loose;   //!
   TBranch        *b_L2_tau84_medium;   //!
   TBranch        *b_L2_tau8_empty_larcalib;   //!
   TBranch        *b_L2_tauNoCut;   //!
   TBranch        *b_L2_tauNoCut_L1TAU50;   //!
   TBranch        *b_L2_tauNoCut_cosmic;   //!
   TBranch        *b_L2_tauNoCut_firstempty;   //!
   TBranch        *b_L2_tauNoCut_unpaired_iso;   //!
   TBranch        *b_L2_tauNoCut_unpaired_noniso;   //!
   TBranch        *b_L2_xe10_noMu;   //!
   TBranch        *b_L2_xe20_noMu;   //!
   TBranch        *b_L2_xe25_noMu;   //!
   TBranch        *b_L2_xe30_noMu;   //!
   TBranch        *b_L2_xe35_noMu;   //!
   TBranch        *b_L2_xe40_noMu;   //!
   TBranch        *b_L2_xe45_noMu;   //!
   TBranch        *b_L2_xe50_noMu;   //!
   TBranch        *b_L2_xe55_noMu;   //!
   TBranch        *b_L2_xe60_noMu;   //!
   TBranch        *b_L2_xe70_noMu;   //!
   TBranch        *b_trig_EF_el_EF_2e10_medium;   //!
   TBranch        *b_trig_EF_el_EF_2e10_medium_mu6;   //!
   TBranch        *b_trig_EF_el_EF_2e11T_medium;   //!
   TBranch        *b_trig_EF_el_EF_2e12T_medium;   //!
   TBranch        *b_trig_EF_el_EF_2e12_medium;   //!
   TBranch        *b_trig_EF_el_EF_2e15_loose;   //!
   TBranch        *b_trig_EF_el_EF_2e15_medium;   //!
   TBranch        *b_trig_EF_el_EF_2e5_medium;   //!
   TBranch        *b_trig_EF_el_EF_2e5_medium_SiTrk;   //!
   TBranch        *b_trig_EF_el_EF_2e5_medium_TRT;   //!
   TBranch        *b_trig_EF_el_EF_2e5_tight;   //!
   TBranch        *b_trig_EF_el_EF_2e5_tight_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_e10_NoCut;   //!
   TBranch        *b_trig_EF_el_EF_e10_loose;   //!
   TBranch        *b_trig_EF_el_EF_e10_loose_mu10;   //!
   TBranch        *b_trig_EF_el_EF_e10_loose_mu6;   //!
   TBranch        *b_trig_EF_el_EF_e10_medium;   //!
   TBranch        *b_trig_EF_el_EF_e10_medium_2mu6;   //!
   TBranch        *b_trig_EF_el_EF_e10_medium_mu10;   //!
   TBranch        *b_trig_EF_el_EF_e10_medium_mu6;   //!
   TBranch        *b_trig_EF_el_EF_e10_medium_mu6_topo_medium;   //!
   TBranch        *b_trig_EF_el_EF_e11T_medium;   //!
   TBranch        *b_trig_EF_el_EF_e11T_medium_2e6T_medium;   //!
   TBranch        *b_trig_EF_el_EF_e11_etcut;   //!
   TBranch        *b_trig_EF_el_EF_e12T_medium_mu6_topo_medium;   //!
   TBranch        *b_trig_EF_el_EF_e12_medium;   //!
   TBranch        *b_trig_EF_el_EF_e13_etcutTRT_xs60_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e13_etcut_xs60_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e13_etcut_xs70_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e15_HLTtighter;   //!
   TBranch        *b_trig_EF_el_EF_e15_loose;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_e12_medium;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_mu6;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_xe30_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_xe35_loose_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_xe35_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_xe40_loose_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_xe40_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e15_medium_xe50_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e15_tight;   //!
   TBranch        *b_trig_EF_el_EF_e15_tight1;   //!
   TBranch        *b_trig_EF_el_EF_e15_tight_xe30_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e18_medium;   //!
   TBranch        *b_trig_EF_el_EF_e18_medium_xe30_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e18_tight;   //!
   TBranch        *b_trig_EF_el_EF_e20_L2Star_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_el_EF_e20_etcut_xs60_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose1;   //!
   TBranch        *b_trig_EF_el_EF_e20_looseTrk;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_2j10_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_3j10_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_4j10_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_TRT;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_xe20;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_xe20_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_xe30;   //!
   TBranch        *b_trig_EF_el_EF_e20_loose_xe30_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e20_medium;   //!
   TBranch        *b_trig_EF_el_EF_e20_medium1;   //!
   TBranch        *b_trig_EF_el_EF_e20_medium2;   //!
   TBranch        *b_trig_EF_el_EF_e20_medium_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_el_EF_e20_medium_SiTrk;   //!
   TBranch        *b_trig_EF_el_EF_e20_medium_TRT;   //!
   TBranch        *b_trig_EF_el_EF_e20_tight;   //!
   TBranch        *b_trig_EF_el_EF_e20_tight_e15_NoCut_Zee;   //!
   TBranch        *b_trig_EF_el_EF_e22_etcut_xs60_noMu;   //!
   TBranch        *b_trig_EF_el_EF_e22_loose;   //!
   TBranch        *b_trig_EF_el_EF_e22_loose1;   //!
   TBranch        *b_trig_EF_el_EF_e22_looseTrk;   //!
   TBranch        *b_trig_EF_el_EF_e22_medium;   //!
   TBranch        *b_trig_EF_el_EF_e22_medium1;   //!
   TBranch        *b_trig_EF_el_EF_e22_medium2;   //!
   TBranch        *b_trig_EF_el_EF_e22_medium_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_el_EF_e22_medium_SiTrk;   //!
   TBranch        *b_trig_EF_el_EF_e22_medium_TRT;   //!
   TBranch        *b_trig_EF_el_EF_e22_tight;   //!
   TBranch        *b_trig_EF_el_EF_e25_loose;   //!
   TBranch        *b_trig_EF_el_EF_e25_medium;   //!
   TBranch        *b_trig_EF_el_EF_e30_loose;   //!
   TBranch        *b_trig_EF_el_EF_e30_medium;   //!
   TBranch        *b_trig_EF_el_EF_e33_medium;   //!
   TBranch        *b_trig_EF_el_EF_e35_medium;   //!
   TBranch        *b_trig_EF_el_EF_e40_medium;   //!
   TBranch        *b_trig_EF_el_EF_e45_medium;   //!
   TBranch        *b_trig_EF_el_EF_e45_medium1;   //!
   TBranch        *b_trig_EF_el_EF_e5_NoCut_L2SW;   //!
   TBranch        *b_trig_EF_el_EF_e5_NoCut_Ringer;   //!
   TBranch        *b_trig_EF_el_EF_e5_NoCut_firstempty;   //!
   TBranch        *b_trig_EF_el_EF_e5_medium_mu4;   //!
   TBranch        *b_trig_EF_el_EF_e5_medium_mu6;   //!
   TBranch        *b_trig_EF_el_EF_e5_medium_mu6_topo_loose;   //!
   TBranch        *b_trig_EF_el_EF_e5_medium_mu6_topo_medium;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_TRT;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e14_etcut_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e4_etcut_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e4_etcut_Jpsi_SiTrk;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e4_etcut_Jpsi_TRT;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e5_NoCut;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e5_NoCut_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e7_etcut_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_e5_tight_e9_etcut_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_e60_loose;   //!
   TBranch        *b_trig_EF_el_EF_e6T_medium;   //!
   TBranch        *b_trig_EF_el_EF_e7_tight_e14_etcut_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_e9_tight_e5_tight_Jpsi;   //!
   TBranch        *b_trig_EF_el_EF_eb_physics;   //!
   TBranch        *b_trig_EF_el_EF_eb_physics_empty;   //!
   TBranch        *b_trig_EF_el_EF_eb_physics_firstempty;   //!
   TBranch        *b_trig_EF_el_EF_eb_physics_noL1PS;   //!
   TBranch        *b_trig_EF_el_EF_eb_physics_unpaired_iso;   //!
   TBranch        *b_trig_EF_el_EF_eb_physics_unpaired_noniso;   //!
   TBranch        *b_trig_EF_el_EF_eb_random;   //!
   TBranch        *b_trig_EF_el_EF_eb_random_empty;   //!
   TBranch        *b_trig_EF_el_EF_eb_random_firstempty;   //!
   TBranch        *b_trig_EF_el_EF_eb_random_unpaired_iso;   //!
   TBranch        *b_trig_EF_el_EF_em105_passHLT;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10_NoAlg;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10_empty;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10_loose;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10_loose1;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10_loose_empty;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10_loose_noOvlpRm;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu10_noOvlpRm;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu13_Zmumu_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_Bmumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_Bmumux;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_DiMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_DiMu_DY20;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_DiMu_SiTrk;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_DiMu_noVtx;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_DiMu_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_EFxe20_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_Upsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_xe30_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4_xe40_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4i_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu4i_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_Bmumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_Bmumux;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_DiMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_MSonly_g10_loose;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_MSonly_g10_loose_noOvlpRm;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_MSonly_g10_loose_nonfilled;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_NL;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_Upsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_e10_loose;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_2mu6_g10_loose;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu0_empty_NoAlg;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu0_firstempty_NoAlg;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu0_missingRoi;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu0_unpaired_iso_NoAlg;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu0_unpaired_noniso_NoAlg;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu100_MSonly;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10_MG;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10_NL;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10_Upsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10_Upsimumu_tight_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10_loose;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu10_muCombTag_NoEF;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu11_empty_NoAlg;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu13;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu13_MG;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu13_muCombTag_NoEF;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_EFxe45;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_MG;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_j90_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_medium_EFxs65_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_medium_EFxs70_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_medium_EFxs75_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_mu10_EFFS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_mu10_EFFS_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_mu10_EFFS_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_mu7_EFFS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_xe30_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15_xe35_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15i;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15i_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu15ii_debug;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu18;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu18_L1J10;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu18_MG;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu18_MG_L1J10;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu18_MG_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu18_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_IDTrkNoCut_ManyVx;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_L2Star_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_MG;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_MG_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_MSonly;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_empty;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_mu10_EFFS_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_mu7_EFFS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20_muCombTag_NoEF;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20i;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu20i_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu22;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu22_MG;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu22_MG_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu22_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu24_MG_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu24_MG_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu24_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu24_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu30_MG_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu30_MG_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu30_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu30_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_MSonly;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_MSonly_barrel;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_MSonly_barrel_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_MSonly_empty;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_MSonly_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_MSonly_tight_L1MU11;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_MSonly_tighter;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_slow;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_slow_empty;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_slow_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_slow_outOfTime;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu40_slow_outOfTime_medium;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Bmumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_BmumuX;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu_FS_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu_MG;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu_MG_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu_SiTrk;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu_SiTrk_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_DiMu_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Jpsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Jpsimumu_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1J10_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1J15_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1J20_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1J30_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1J50_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1J75_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1MU11_MSonly_cosmic;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_L1MU11_cosmic;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_MSonly_cosmic;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Trk_Jpsi;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Trk_Jpsi_loose;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Trk_Jpsi_looser;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Trk_Upsi_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Upsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Upsimumu_SiTrk_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_Upsimumu_tight_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_cosmic;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_firstempty;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_j10_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_j10_a4tc_EFFS_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_j135_a4tc_EFFS_L1matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_j180_a4tc_EFFS_L1matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_j20_a4tc_EFFS_matched;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_j30;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_j45_a4tc_EFFS_xe45_loose_noMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_mu6;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4_unpaired_iso;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4imu6i_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4imu6i_DiMu_DY14_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4imu6i_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_Bmumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_Bmumux;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_DiMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_DiMu_DY20;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_DiMu_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu4mu6_Upsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu60_MSonly;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_Bmumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_BmumuX;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_DiMu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_DiMu_noOS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_Jpsimumu_SiTrk;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_Jpsimumu_tight;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_Trk_Jpsi_loose;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_Upsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu6_Upsimumu_tight_FS;   //!
   TBranch        *b_trig_EF_trigmuonef_EF_mu80_MSonly;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10_NoAlg;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10_empty;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10_loose;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10_loose1;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10_loose_empty;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10_loose_noOvlpRm;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu10_noOvlpRm;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu13_Zmumu_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_Bmumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_Bmumux;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_DiMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_DiMu_DY20;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_DiMu_SiTrk;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_DiMu_noVtx;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_DiMu_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_EFxe20_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_Upsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_xe30_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4_xe40_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4i_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu4i_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_Bmumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_Bmumux;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_DiMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_MSonly_g10_loose;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_MSonly_g10_loose_noOvlpRm;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_MSonly_g10_loose_nonfilled;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_NL;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_Upsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_e10_loose;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_2mu6_g10_loose;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu0_empty_NoAlg;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu0_firstempty_NoAlg;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu0_missingRoi;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu0_unpaired_iso_NoAlg;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu0_unpaired_noniso_NoAlg;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu100_MSonly;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10_MG;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10_NL;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10_Upsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10_Upsimumu_tight_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10_loose;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu10_muCombTag_NoEF;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu11_empty_NoAlg;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu13;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu13_MG;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu13_muCombTag_NoEF;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_EFxe45;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_MG;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_j90_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_medium_EFxs65_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_medium_EFxs70_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_medium_EFxs75_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_mu10_EFFS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_mu10_EFFS_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_mu10_EFFS_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_mu7_EFFS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_xe30_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15_xe35_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15i;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15i_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu15ii_debug;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu18;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu18_L1J10;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu18_MG;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu18_MG_L1J10;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu18_MG_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu18_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_IDTrkNoCut_ManyVx;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_L2Star_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_MG;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_MG_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_MSonly;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_empty;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_mu10_EFFS_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_mu7_EFFS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20_muCombTag_NoEF;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20i;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu20i_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu22;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu22_MG;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu22_MG_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu22_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu24_MG_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu24_MG_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu24_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu24_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu30_MG_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu30_MG_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu30_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu30_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_MSonly;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_MSonly_barrel;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_MSonly_barrel_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_MSonly_empty;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_MSonly_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_MSonly_tight_L1MU11;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_MSonly_tighter;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_slow;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_slow_empty;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_slow_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_slow_outOfTime;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu40_slow_outOfTime_medium;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Bmumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_BmumuX;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu_FS_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu_MG;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu_MG_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu_SiTrk;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu_SiTrk_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_DiMu_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Jpsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Jpsimumu_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1J10_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1J15_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1J20_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1J30_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1J50_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1J75_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1MU11_MSonly_cosmic;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_L1MU11_cosmic;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_MSonly_cosmic;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Trk_Jpsi;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Trk_Jpsi_loose;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Trk_Jpsi_looser;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Trk_Upsi_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Upsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Upsimumu_SiTrk_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_Upsimumu_tight_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_cosmic;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_firstempty;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_j10_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_j10_a4tc_EFFS_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_j135_a4tc_EFFS_L1matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_j180_a4tc_EFFS_L1matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_j20_a4tc_EFFS_matched;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_j30;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_j45_a4tc_EFFS_xe45_loose_noMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_mu6;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4_unpaired_iso;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4imu6i_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4imu6i_DiMu_DY14_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4imu6i_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_Bmumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_Bmumux;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_DiMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_DiMu_DY;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_DiMu_DY20;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_DiMu_DY_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_DiMu_noVtx_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_Jpsimumu_IDTrkNoCut;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu4mu6_Upsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu60_MSonly;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_Bmumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_BmumuX;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_DiMu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_DiMu_noOS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_Jpsimumu;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_Jpsimumu_SiTrk;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_Jpsimumu_tight;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_Trk_Jpsi_loose;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_Upsimumu_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu6_Upsimumu_tight_FS;   //!
   TBranch        *b_trig_EF_trigmugirl_EF_mu80_MSonly;   //!
   TBranch        *b_trig_L2_jet_L2_2fj25;   //!
   TBranch        *b_trig_L2_jet_L2_2fj50;   //!
   TBranch        *b_trig_L2_jet_L2_2fj70;   //!
   TBranch        *b_trig_L2_jet_L2_2j25_j70_dphi03;   //!
   TBranch        *b_trig_L2_jet_L2_2j40_anymct100_xe20_medium_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_2j50_anymct100_xe20_medium_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_3j25;   //!
   TBranch        *b_trig_L2_jet_L2_3j35;   //!
   TBranch        *b_trig_L2_jet_L2_3j40;   //!
   TBranch        *b_trig_L2_jet_L2_3j70;   //!
   TBranch        *b_trig_L2_jet_L2_3j95;   //!
   TBranch        *b_trig_L2_jet_L2_4j25;   //!
   TBranch        *b_trig_L2_jet_L2_4j35;   //!
   TBranch        *b_trig_L2_jet_L2_4j40;   //!
   TBranch        *b_trig_L2_jet_L2_4j50;   //!
   TBranch        *b_trig_L2_jet_L2_5j25;   //!
   TBranch        *b_trig_L2_jet_L2_5j35;   //!
   TBranch        *b_trig_L2_jet_L2_5j40;   //!
   TBranch        *b_trig_L2_jet_L2_6j25;   //!
   TBranch        *b_trig_L2_jet_L2_fj10_empty_larcalib;   //!
   TBranch        *b_trig_L2_jet_L2_fj25;   //!
   TBranch        *b_trig_L2_jet_L2_fj25_l2cleanph;   //!
   TBranch        *b_trig_L2_jet_L2_fj25_larcalib;   //!
   TBranch        *b_trig_L2_jet_L2_fj50;   //!
   TBranch        *b_trig_L2_jet_L2_fj50_larcalib;   //!
   TBranch        *b_trig_L2_jet_L2_fj70;   //!
   TBranch        *b_trig_L2_jet_L2_fj95;   //!
   TBranch        *b_trig_L2_jet_L2_j10_empty_larcalib;   //!
   TBranch        *b_trig_L2_jet_L2_j25;   //!
   TBranch        *b_trig_L2_jet_L2_j25_cosmic;   //!
   TBranch        *b_trig_L2_jet_L2_j25_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j25_fj25;   //!
   TBranch        *b_trig_L2_jet_L2_j25_l2cleanph;   //!
   TBranch        *b_trig_L2_jet_L2_j25_larcalib;   //!
   TBranch        *b_trig_L2_jet_L2_j30;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HV;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HV_cosmic;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HV_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HV_unpaired_iso;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HV_unpaired_noniso;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HVtrk;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HVtrk_LOF;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HVtrk_cosmic;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HVtrk_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HVtrk_unpaired_iso;   //!
   TBranch        *b_trig_L2_jet_L2_j30_L1TAU_HVtrk_unpaired_noniso;   //!
   TBranch        *b_trig_L2_jet_L2_j30_Trackless_HV;   //!
   TBranch        *b_trig_L2_jet_L2_j30_Trackless_HV_L1MU10;   //!
   TBranch        *b_trig_L2_jet_L2_j30_Trackless_HV_cosmic;   //!
   TBranch        *b_trig_L2_jet_L2_j30_Trackless_HV_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j30_Trackless_HV_unpaired_iso;   //!
   TBranch        *b_trig_L2_jet_L2_j30_Trackless_HV_unpaired_noniso;   //!
   TBranch        *b_trig_L2_jet_L2_j30_eta13_a4tc_EFFS_EFxe30_noMu_empty;   //!
   TBranch        *b_trig_L2_jet_L2_j30_eta13_a4tc_EFFS_EFxe30_noMu_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j35;   //!
   TBranch        *b_trig_L2_jet_L2_j35_fj35;   //!
   TBranch        *b_trig_L2_jet_L2_j40;   //!
   TBranch        *b_trig_L2_jet_L2_j45;   //!
   TBranch        *b_trig_L2_jet_L2_j45_cosmic;   //!
   TBranch        *b_trig_L2_jet_L2_j45_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j50;   //!
   TBranch        *b_trig_L2_jet_L2_j50_eta13_a4tc_EFFS_EFxe50_noMu_empty;   //!
   TBranch        *b_trig_L2_jet_L2_j50_eta13_a4tc_EFFS_EFxe50_noMu_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j50_eta25_a4tc_EFFS_EFxe50_noMu_empty;   //!
   TBranch        *b_trig_L2_jet_L2_j50_eta25_a4tc_EFFS_EFxe50_noMu_firstempty;   //!
   TBranch        *b_trig_L2_jet_L2_j50_fj50;   //!
   TBranch        *b_trig_L2_jet_L2_j50_larcalib;   //!
   TBranch        *b_trig_L2_jet_L2_j50_xe35_medium_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_j50_xe35_medium_noMu_l2cleancons;   //!
   TBranch        *b_trig_L2_jet_L2_j60_xe45_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_j70;   //!
   TBranch        *b_trig_L2_jet_L2_j70_2j25;   //!
   TBranch        *b_trig_L2_jet_L2_j70_2j25_L2anymct100;   //!
   TBranch        *b_trig_L2_jet_L2_j70_2j25_anymct100;   //!
   TBranch        *b_trig_L2_jet_L2_j70_2j25_anymct150;   //!
   TBranch        *b_trig_L2_jet_L2_j70_2j25_anymct175;   //!
   TBranch        *b_trig_L2_jet_L2_j70_2j25_dphi04;   //!
   TBranch        *b_trig_L2_jet_L2_j70_3j25;   //!
   TBranch        *b_trig_L2_jet_L2_j70_fj70;   //!
   TBranch        *b_trig_L2_jet_L2_j70_xe20_loose_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_j70_xe25_loose_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_j70_xe35_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_j70_xe35_noMu_l2cleancons;   //!
   TBranch        *b_trig_L2_jet_L2_j70_xs25_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_j75_xe40_noMu;   //!
   TBranch        *b_trig_L2_jet_L2_j95;   //!
   TBranch        *b_trig_L2_jet_L2_j95_2j25_dphi04;   //!
   TBranch        *b_trig_L2_jet_L2_j95_l2cleanph;   //!
   TBranch        *b_trig_L2_jet_L2_j95_larcalib;   //!
   TBranch        *b_trig_L2_jet_L2_je195;   //!
   TBranch        *b_trig_L2_jet_L2_je255;   //!
   TBranch        *b_trig_L2_jet_L2_je300;   //!
   TBranch        *b_trig_L2_jet_L2_je350;   //!
   TBranch        *b_trig_L2_jet_L2_je420;   //!
   TBranch        *b_trig_L2_jet_L2_je500;   //!
   TBranch        *b_trig_EF_jet_EF_2fj100_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_trig_EF_jet_EF_2fj30_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_trig_EF_jet_EF_2fj30_a4tc_EFFS_deta50_FC;   //!
   TBranch        *b_trig_EF_jet_EF_2fj55_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_trig_EF_jet_EF_2fj55_a4tc_EFFS_deta50_FC;   //!
   TBranch        *b_trig_EF_jet_EF_2fj75_a4tc_EFFS_deta50_FB;   //!
   TBranch        *b_trig_EF_jet_EF_2fj75_a4tc_EFFS_deta50_FC;   //!
   TBranch        *b_trig_EF_jet_EF_2j100_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_trig_EF_jet_EF_2j135_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_trig_EF_jet_EF_2j180_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_trig_EF_jet_EF_2j240_a4tc_EFFS_deta35_FC;   //!
   TBranch        *b_trig_EF_jet_EF_2j45_a4tc_EFFS_leadingmct100_xe40_medium_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_2j55_a4tc_EFFS_leadingmct100_xe40_medium_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_3j100_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_3j100_a4tc_EFFS_L1J75;   //!
   TBranch        *b_trig_EF_jet_EF_3j30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_3j40_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_3j45_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_3j75_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_4j30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_4j40_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_4j40_a4tc_EFFS_ht350;   //!
   TBranch        *b_trig_EF_jet_EF_4j40_a4tc_EFFS_ht400;   //!
   TBranch        *b_trig_EF_jet_EF_4j45_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_4j55_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_4j60_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_5j30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_5j40_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_5j45_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_6j30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_6j30_a4tc_EFFS_L15J10;   //!
   TBranch        *b_trig_EF_jet_EF_6j40_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_6j45_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_7j30_a4tc_EFFS_L15J10;   //!
   TBranch        *b_trig_EF_jet_EF_7j30_a4tc_EFFS_L16J10;   //!
   TBranch        *b_trig_EF_jet_EF_fj100_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_fj10_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_fj10_a4tc_EFFS_1vx;   //!
   TBranch        *b_trig_EF_jet_EF_fj135_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_fj15_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_fj20_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_fj30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_fj30_a4tc_EFFS_l2cleanph;   //!
   TBranch        *b_trig_EF_jet_EF_fj55_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_fj75_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j100_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j100_a4tc_EFFS_ht350;   //!
   TBranch        *b_trig_EF_jet_EF_j100_a4tc_EFFS_ht400;   //!
   TBranch        *b_trig_EF_jet_EF_j100_a4tc_EFFS_ht500;   //!
   TBranch        *b_trig_EF_jet_EF_j100_j30_a4tc_EFFS_L2dphi04;   //!
   TBranch        *b_trig_EF_jet_EF_j10_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j10_a4tc_EFFS_1vx;   //!
   TBranch        *b_trig_EF_jet_EF_j135_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j135_a4tc_EFFS_ht500;   //!
   TBranch        *b_trig_EF_jet_EF_j135_j30_a4tc_EFFS_L2dphi04;   //!
   TBranch        *b_trig_EF_jet_EF_j135_j30_a4tc_EFFS_dphi04;   //!
   TBranch        *b_trig_EF_jet_EF_j15_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j180_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j180_j30_a4tc_EFFS_dphi04;   //!
   TBranch        *b_trig_EF_jet_EF_j20_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j240_a10tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j240_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j240_a4tc_EFFS_l2cleanph;   //!
   TBranch        *b_trig_EF_jet_EF_j30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j30_a4tc_EFFS_l2cleanph;   //!
   TBranch        *b_trig_EF_jet_EF_j30_cosmic;   //!
   TBranch        *b_trig_EF_jet_EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_empty;   //!
   TBranch        *b_trig_EF_jet_EF_j30_eta13_a4tc_EFFS_EFxe30_noMu_firstempty;   //!
   TBranch        *b_trig_EF_jet_EF_j30_firstempty;   //!
   TBranch        *b_trig_EF_jet_EF_j30_fj30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j320_a10tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j320_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_cosmic;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_firstempty;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_iso;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HV_unpaired_noniso;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_LOF;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_cosmic;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_firstempty;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_iso;   //!
   TBranch        *b_trig_EF_jet_EF_j35_a4tc_EFFS_L1TAU_HVtrk_unpaired_noniso;   //!
   TBranch        *b_trig_EF_jet_EF_j40_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j40_fj40_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j425_a10tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j425_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j45_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j50_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j50_cosmic;   //!
   TBranch        *b_trig_EF_jet_EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_empty;   //!
   TBranch        *b_trig_EF_jet_EF_j50_eta13_a4tc_EFFS_EFxe50_noMu_firstempty;   //!
   TBranch        *b_trig_EF_jet_EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_empty;   //!
   TBranch        *b_trig_EF_jet_EF_j50_eta25_a4tc_EFFS_EFxe50_noMu_firstempty;   //!
   TBranch        *b_trig_EF_jet_EF_j50_firstempty;   //!
   TBranch        *b_trig_EF_jet_EF_j55_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10;   //!
   TBranch        *b_trig_EF_jet_EF_j55_a4tc_EFFS_xe55_medium_noMu_dphi2j30xe10_l2cleancons;   //!
   TBranch        *b_trig_EF_jet_EF_j55_fj55_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j65_a4tc_EFFS_xe65_noMu_dphi2j30xe10;   //!
   TBranch        *b_trig_EF_jet_EF_j70_j25_dphi03_NoEF;   //!
   TBranch        *b_trig_EF_jet_EF_j75_2j30_a4tc_EFFS_ht350;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS_xe40_loose_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS_xe40_loose_noMu_dphijxe03;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS_xe45_loose_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS_xe55_loose_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS_xe55_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS_xe55_noMu_l2cleancons;   //!
   TBranch        *b_trig_EF_jet_EF_j75_a4tc_EFFS_xs35_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_j75_fj75_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2anymct100;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2anymct150;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2anymct175;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS_L2dphi04;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS_anymct150;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS_anymct175;   //!
   TBranch        *b_trig_EF_jet_EF_j75_j30_a4tc_EFFS_leadingmct150;   //!
   TBranch        *b_trig_EF_jet_EF_j80_a4tc_EFFS_xe60_noMu;   //!
   TBranch        *b_trig_EF_jet_EF_je195_NoEF;   //!
   TBranch        *b_trig_EF_jet_EF_je255_NoEF;   //!
   TBranch        *b_trig_EF_jet_EF_je300_NoEF;   //!
   TBranch        *b_trig_EF_jet_EF_je350_NoEF;   //!
   TBranch        *b_trig_EF_jet_EF_je420_NoEF;   //!
   TBranch        *b_trig_EF_jet_EF_je500_NoEF;   //!
   TBranch        *b_SkimDecision_NTPSUSY_name;   //!
   TBranch        *b_SkimDecision_NTPSUSY_accepted;   //!
   TBranch        *b_SkimDecision_DAODJETS_name;   //!
   TBranch        *b_SkimDecision_DAODJETS_accepted;   //!
   TBranch        *b_SkimDecision_DAODMUONS_name;   //!
   TBranch        *b_SkimDecision_DAODMUONS_accepted;   //!
   TBranch        *b_SkimDecision_DAODEGAMMA_name;   //!
   TBranch        *b_SkimDecision_DAODEGAMMA_accepted;   //!
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_timestamp_ns;   //!
   TBranch        *b_lbn;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_detmask0;   //!
   TBranch        *b_detmask1;   //!
   TBranch        *b_actualIntPerXing;   //!
   TBranch        *b_averageIntPerXing;   //!
   TBranch        *b_mc_channel_number;   //!
   TBranch        *b_mc_event_number;   //!
   TBranch        *b_mc_event_weight;   //!
   TBranch        *b_pixelFlags;   //!
   TBranch        *b_sctFlags;   //!
   TBranch        *b_trtFlags;   //!
   TBranch        *b_larFlags;   //!
   TBranch        *b_tileFlags;   //!
   TBranch        *b_muonFlags;   //!
   TBranch        *b_fwdFlags;   //!
   TBranch        *b_coreFlags;   //!
   TBranch        *b_pixelError;   //!
   TBranch        *b_sctError;   //!
   TBranch        *b_trtError;   //!
   TBranch        *b_larError;   //!
   TBranch        *b_tileError;   //!
   TBranch        *b_muonError;   //!
   TBranch        *b_fwdError;   //!
   TBranch        *b_coreError;   //!
   TBranch        *b_isSimulation;   //!
   TBranch        *b_isCalibration;   //!
   TBranch        *b_isTestBeam;   //!
   TBranch        *b_bb_decision;   //!
   TBranch        *b_bb_numSegment;   //!
   TBranch        *b_bb_numSegmentEarly;   //!
   TBranch        *b_bb_numSegmentAC;   //!
   TBranch        *b_bb_numMatched;   //!
   TBranch        *b_bb_numNoTimeLoose;   //!
   TBranch        *b_bb_numNoTimeMedium;   //!
   TBranch        *b_bb_numNoTimeTight;   //!
   TBranch        *b_bb_numOneSidedLoose;   //!
   TBranch        *b_bb_numOneSidedMedium;   //!
   TBranch        *b_bb_numOneSidedTight;   //!
   TBranch        *b_bb_numTwoSided;   //!
   TBranch        *b_bb_numClusterShape;   //!
   TBranch        *b_bb_cl_n;   //!
   TBranch        *b_bb_cl_bbDecision;   //!
   TBranch        *b_bb_cl_drdz;   //!
   TBranch        *b_bb_cl_pt;   //!
   TBranch        *b_bb_cl_eta;   //!
   TBranch        *b_bb_cl_phi;   //!
   TBranch        *b_bb_cl_E_em;   //!
   TBranch        *b_bb_cl_E_had;   //!
   TBranch        *b_bb_cl_firstEdens;   //!
   TBranch        *b_bb_cl_cellmaxfrac;   //!
   TBranch        *b_bb_cl_longitudinal;   //!
   TBranch        *b_bb_cl_secondlambda;   //!
   TBranch        *b_bb_cl_lateral;   //!
   TBranch        *b_bb_cl_secondR;   //!
   TBranch        *b_bb_cl_centerlambda;   //!
   TBranch        *b_bb_cl_centermag;   //!
   TBranch        *b_bb_cl_deltaTheta;   //!
   TBranch        *b_bb_cl_deltaPhi;   //!
   TBranch        *b_bb_cl_time;   //!
   TBranch        *b_bb_jet_n;   //!
   TBranch        *b_el_n;   //!
   TBranch        *b_el_E;   //!
   TBranch        *b_el_Et;   //!
   TBranch        *b_el_pt;   //!
   TBranch        *b_el_m;   //!
   TBranch        *b_el_eta;   //!
   TBranch        *b_el_phi;   //!
   TBranch        *b_el_px;   //!
   TBranch        *b_el_py;   //!
   TBranch        *b_el_pz;   //!
   TBranch        *b_el_charge;   //!
   TBranch        *b_el_author;   //!
   TBranch        *b_el_isEM;   //!
   TBranch        *b_el_isEMLoose;   //!
   TBranch        *b_el_isEMMedium;   //!
   TBranch        *b_el_isEMTight;   //!
   TBranch        *b_el_OQ;   //!
   TBranch        *b_el_convFlag;   //!
   TBranch        *b_el_isConv;   //!
   TBranch        *b_el_nConv;   //!
   TBranch        *b_el_nSingleTrackConv;   //!
   TBranch        *b_el_nDoubleTrackConv;   //!
   TBranch        *b_el_OQRecalc;   //!
   TBranch        *b_el_type;   //!
   TBranch        *b_el_origin;   //!
   TBranch        *b_el_typebkg;   //!
   TBranch        *b_el_originbkg;   //!
   TBranch        *b_el_truth_E;   //!
   TBranch        *b_el_truth_pt;   //!
   TBranch        *b_el_truth_eta;   //!
   TBranch        *b_el_truth_phi;   //!
   TBranch        *b_el_truth_type;   //!
   TBranch        *b_el_truth_status;   //!
   TBranch        *b_el_truth_barcode;   //!
   TBranch        *b_el_truth_mothertype;   //!
   TBranch        *b_el_truth_motherbarcode;   //!
   TBranch        *b_el_truth_hasHardBrem;   //!
   TBranch        *b_el_truth_index;   //!
   TBranch        *b_el_truth_matched;   //!
   TBranch        *b_el_mediumWithoutTrack;   //!
   TBranch        *b_el_mediumIsoWithoutTrack;   //!
   TBranch        *b_el_tightWithoutTrack;   //!
   TBranch        *b_el_tightIsoWithoutTrack;   //!
   TBranch        *b_el_loose;   //!
   TBranch        *b_el_looseIso;   //!
   TBranch        *b_el_medium;   //!
   TBranch        *b_el_mediumIso;   //!
   TBranch        *b_el_tight;   //!
   TBranch        *b_el_tightIso;   //!
   TBranch        *b_el_loosePP;   //!
   TBranch        *b_el_loosePPIso;   //!
   TBranch        *b_el_mediumPP;   //!
   TBranch        *b_el_mediumPPIso;   //!
   TBranch        *b_el_tightPP;   //!
   TBranch        *b_el_tightPPIso;   //!
   TBranch        *b_el_goodOQ;   //!
   TBranch        *b_el_Ethad;   //!
   TBranch        *b_el_Ethad1;   //!
   TBranch        *b_el_f1;   //!
   TBranch        *b_el_f1core;   //!
   TBranch        *b_el_Emins1;   //!
   TBranch        *b_el_fside;   //!
   TBranch        *b_el_Emax2;   //!
   TBranch        *b_el_ws3;   //!
   TBranch        *b_el_wstot;   //!
   TBranch        *b_el_emaxs1;   //!
   TBranch        *b_el_deltaEs;   //!
   TBranch        *b_el_E233;   //!
   TBranch        *b_el_E237;   //!
   TBranch        *b_el_E277;   //!
   TBranch        *b_el_weta2;   //!
   TBranch        *b_el_f3;   //!
   TBranch        *b_el_f3core;   //!
   TBranch        *b_el_rphiallcalo;   //!
   TBranch        *b_el_Etcone45;   //!
   TBranch        *b_el_Etcone15;   //!
   TBranch        *b_el_Etcone20;   //!
   TBranch        *b_el_Etcone25;   //!
   TBranch        *b_el_Etcone30;   //!
   TBranch        *b_el_Etcone35;   //!
   TBranch        *b_el_Etcone40;   //!
   TBranch        *b_el_ptcone20;   //!
   TBranch        *b_el_ptcone30;   //!
   TBranch        *b_el_ptcone40;   //!
   TBranch        *b_el_nucone20;   //!
   TBranch        *b_el_nucone30;   //!
   TBranch        *b_el_nucone40;   //!
   TBranch        *b_el_convanglematch;   //!
   TBranch        *b_el_convtrackmatch;   //!
   TBranch        *b_el_hasconv;   //!
   TBranch        *b_el_convvtxx;   //!
   TBranch        *b_el_convvtxy;   //!
   TBranch        *b_el_convvtxz;   //!
   TBranch        *b_el_Rconv;   //!
   TBranch        *b_el_zconv;   //!
   TBranch        *b_el_convvtxchi2;   //!
   TBranch        *b_el_pt1conv;   //!
   TBranch        *b_el_convtrk1nBLHits;   //!
   TBranch        *b_el_convtrk1nPixHits;   //!
   TBranch        *b_el_convtrk1nSCTHits;   //!
   TBranch        *b_el_convtrk1nTRTHits;   //!
   TBranch        *b_el_pt2conv;   //!
   TBranch        *b_el_convtrk2nBLHits;   //!
   TBranch        *b_el_convtrk2nPixHits;   //!
   TBranch        *b_el_convtrk2nSCTHits;   //!
   TBranch        *b_el_convtrk2nTRTHits;   //!
   TBranch        *b_el_ptconv;   //!
   TBranch        *b_el_pzconv;   //!
   TBranch        *b_el_pos7;   //!
   TBranch        *b_el_etacorrmag;   //!
   TBranch        *b_el_deltaeta1;   //!
   TBranch        *b_el_deltaeta2;   //!
   TBranch        *b_el_deltaphi2;   //!
   TBranch        *b_el_deltaphiRescaled;   //!
   TBranch        *b_el_deltaPhiRot;   //!
   TBranch        *b_el_expectHitInBLayer;   //!
   TBranch        *b_el_trackd0_physics;   //!
   TBranch        *b_el_etaSampling1;   //!
   TBranch        *b_el_reta;   //!
   TBranch        *b_el_rphi;   //!
   TBranch        *b_el_zvertex;   //!
   TBranch        *b_el_errz;   //!
   TBranch        *b_el_etap;   //!
   TBranch        *b_el_depth;   //!
   TBranch        *b_el_refittedTrack_n;   //!
   TBranch        *b_el_Es0;   //!
   TBranch        *b_el_etas0;   //!
   TBranch        *b_el_phis0;   //!
   TBranch        *b_el_Es1;   //!
   TBranch        *b_el_etas1;   //!
   TBranch        *b_el_phis1;   //!
   TBranch        *b_el_Es2;   //!
   TBranch        *b_el_etas2;   //!
   TBranch        *b_el_phis2;   //!
   TBranch        *b_el_Es3;   //!
   TBranch        *b_el_etas3;   //!
   TBranch        *b_el_phis3;   //!
   TBranch        *b_el_cl_E;   //!
   TBranch        *b_el_cl_pt;   //!
   TBranch        *b_el_cl_eta;   //!
   TBranch        *b_el_cl_phi;   //!
   TBranch        *b_el_firstEdens;   //!
   TBranch        *b_el_cellmaxfrac;   //!
   TBranch        *b_el_longitudinal;   //!
   TBranch        *b_el_secondlambda;   //!
   TBranch        *b_el_lateral;   //!
   TBranch        *b_el_secondR;   //!
   TBranch        *b_el_centerlambda;   //!
   TBranch        *b_el_cl_time;   //!
   TBranch        *b_el_trackd0;   //!
   TBranch        *b_el_trackz0;   //!
   TBranch        *b_el_trackphi;   //!
   TBranch        *b_el_tracktheta;   //!
   TBranch        *b_el_trackqoverp;   //!
   TBranch        *b_el_trackpt;   //!
   TBranch        *b_el_tracketa;   //!
   TBranch        *b_el_nBLHits;   //!
   TBranch        *b_el_nPixHits;   //!
   TBranch        *b_el_nSCTHits;   //!
   TBranch        *b_el_nTRTHits;   //!
   TBranch        *b_el_nTRTHighTHits;   //!
   TBranch        *b_el_nPixHoles;   //!
   TBranch        *b_el_nSCTHoles;   //!
   TBranch        *b_el_nTRTHoles;   //!
   TBranch        *b_el_nBLSharedHits;   //!
   TBranch        *b_el_nPixSharedHits;   //!
   TBranch        *b_el_nSCTSharedHits;   //!
   TBranch        *b_el_nBLayerOutliers;   //!
   TBranch        *b_el_nPixelOutliers;   //!
   TBranch        *b_el_nSCTOutliers;   //!
   TBranch        *b_el_nTRTOutliers;   //!
   TBranch        *b_el_nTRTHighTOutliers;   //!
   TBranch        *b_el_expectBLayerHit;   //!
   TBranch        *b_el_nSiHits;   //!
   TBranch        *b_el_TRTHighTHitsRatio;   //!
   TBranch        *b_el_TRTHighTOutliersRatio;   //!
   TBranch        *b_el_pixeldEdx;   //!
   TBranch        *b_el_nGoodHitsPixeldEdx;   //!
   TBranch        *b_el_massPixeldEdx;   //!
   TBranch        *b_el_likelihoodsPixeldEdx;   //!
   TBranch        *b_el_vertweight;   //!
   TBranch        *b_el_hastrack;   //!
   TBranch        *b_el_Etcone40_ED_corrected;   //!
   TBranch        *b_el_Etcone40_corrected;   //!
   TBranch        *b_el_EF_dr;   //!
   TBranch        *b_el_EF_index;   //!
   TBranch        *b_ph_n;   //!
   TBranch        *b_ph_E;   //!
   TBranch        *b_ph_Et;   //!
   TBranch        *b_ph_pt;   //!
   TBranch        *b_ph_m;   //!
   TBranch        *b_ph_eta;   //!
   TBranch        *b_ph_phi;   //!
   TBranch        *b_ph_px;   //!
   TBranch        *b_ph_py;   //!
   TBranch        *b_ph_pz;   //!
   TBranch        *b_ph_author;   //!
   TBranch        *b_ph_isRecovered;   //!
   TBranch        *b_ph_isEM;   //!
   TBranch        *b_ph_isEMLoose;   //!
   TBranch        *b_ph_isEMMedium;   //!
   TBranch        *b_ph_isEMTight;   //!
   TBranch        *b_ph_OQ;   //!
   TBranch        *b_ph_OQRecalc;   //!
   TBranch        *b_ph_convFlag;   //!
   TBranch        *b_ph_isConv;   //!
   TBranch        *b_ph_nConv;   //!
   TBranch        *b_ph_nSingleTrackConv;   //!
   TBranch        *b_ph_nDoubleTrackConv;   //!
   TBranch        *b_ph_type;   //!
   TBranch        *b_ph_origin;   //!
   TBranch        *b_ph_truth_deltaRRecPhoton;   //!
   TBranch        *b_ph_truth_E;   //!
   TBranch        *b_ph_truth_pt;   //!
   TBranch        *b_ph_truth_eta;   //!
   TBranch        *b_ph_truth_phi;   //!
   TBranch        *b_ph_truth_type;   //!
   TBranch        *b_ph_truth_status;   //!
   TBranch        *b_ph_truth_barcode;   //!
   TBranch        *b_ph_truth_mothertype;   //!
   TBranch        *b_ph_truth_motherbarcode;   //!
   TBranch        *b_ph_truth_index;   //!
   TBranch        *b_ph_truth_matched;   //!
   TBranch        *b_ph_loose;   //!
   TBranch        *b_ph_looseIso;   //!
   TBranch        *b_ph_tight;   //!
   TBranch        *b_ph_tightIso;   //!
   TBranch        *b_ph_looseAR;   //!
   TBranch        *b_ph_looseARIso;   //!
   TBranch        *b_ph_tightAR;   //!
   TBranch        *b_ph_tightARIso;   //!
   TBranch        *b_ph_goodOQ;   //!
   TBranch        *b_ph_Ethad;   //!
   TBranch        *b_ph_Ethad1;   //!
   TBranch        *b_ph_E033;   //!
   TBranch        *b_ph_f1;   //!
   TBranch        *b_ph_f1core;   //!
   TBranch        *b_ph_Emins1;   //!
   TBranch        *b_ph_fside;   //!
   TBranch        *b_ph_Emax2;   //!
   TBranch        *b_ph_ws3;   //!
   TBranch        *b_ph_wstot;   //!
   TBranch        *b_ph_E132;   //!
   TBranch        *b_ph_E1152;   //!
   TBranch        *b_ph_emaxs1;   //!
   TBranch        *b_ph_deltaEs;   //!
   TBranch        *b_ph_E233;   //!
   TBranch        *b_ph_E237;   //!
   TBranch        *b_ph_E277;   //!
   TBranch        *b_ph_weta2;   //!
   TBranch        *b_ph_f3;   //!
   TBranch        *b_ph_f3core;   //!
   TBranch        *b_ph_rphiallcalo;   //!
   TBranch        *b_ph_Etcone45;   //!
   TBranch        *b_ph_Etcone15;   //!
   TBranch        *b_ph_Etcone20;   //!
   TBranch        *b_ph_Etcone25;   //!
   TBranch        *b_ph_Etcone30;   //!
   TBranch        *b_ph_Etcone35;   //!
   TBranch        *b_ph_Etcone40;   //!
   TBranch        *b_ph_ptcone20;   //!
   TBranch        *b_ph_ptcone30;   //!
   TBranch        *b_ph_ptcone40;   //!
   TBranch        *b_ph_nucone20;   //!
   TBranch        *b_ph_nucone30;   //!
   TBranch        *b_ph_nucone40;   //!
   TBranch        *b_ph_convanglematch;   //!
   TBranch        *b_ph_convtrackmatch;   //!
   TBranch        *b_ph_hasconv;   //!
   TBranch        *b_ph_convvtxx;   //!
   TBranch        *b_ph_convvtxy;   //!
   TBranch        *b_ph_convvtxz;   //!
   TBranch        *b_ph_Rconv;   //!
   TBranch        *b_ph_zconv;   //!
   TBranch        *b_ph_convvtxchi2;   //!
   TBranch        *b_ph_pt1conv;   //!
   TBranch        *b_ph_convtrk1nBLHits;   //!
   TBranch        *b_ph_convtrk1nPixHits;   //!
   TBranch        *b_ph_convtrk1nSCTHits;   //!
   TBranch        *b_ph_convtrk1nTRTHits;   //!
   TBranch        *b_ph_pt2conv;   //!
   TBranch        *b_ph_convtrk2nBLHits;   //!
   TBranch        *b_ph_convtrk2nPixHits;   //!
   TBranch        *b_ph_convtrk2nSCTHits;   //!
   TBranch        *b_ph_convtrk2nTRTHits;   //!
   TBranch        *b_ph_ptconv;   //!
   TBranch        *b_ph_pzconv;   //!
   TBranch        *b_ph_reta;   //!
   TBranch        *b_ph_rphi;   //!
   TBranch        *b_ph_zvertex;   //!
   TBranch        *b_ph_errz;   //!
   TBranch        *b_ph_etap;   //!
   TBranch        *b_ph_depth;   //!
   TBranch        *b_ph_cl_E;   //!
   TBranch        *b_ph_cl_pt;   //!
   TBranch        *b_ph_cl_eta;   //!
   TBranch        *b_ph_cl_phi;   //!
   TBranch        *b_ph_Es0;   //!
   TBranch        *b_ph_etas0;   //!
   TBranch        *b_ph_phis0;   //!
   TBranch        *b_ph_Es1;   //!
   TBranch        *b_ph_etas1;   //!
   TBranch        *b_ph_phis1;   //!
   TBranch        *b_ph_Es2;   //!
   TBranch        *b_ph_etas2;   //!
   TBranch        *b_ph_phis2;   //!
   TBranch        *b_ph_Es3;   //!
   TBranch        *b_ph_etas3;   //!
   TBranch        *b_ph_phis3;   //!
   TBranch        *b_ph_cl_time;   //!
   TBranch        *b_ph_truth_isConv;   //!
   TBranch        *b_ph_truth_isBrem;   //!
   TBranch        *b_ph_truth_isFromHardProc;   //!
   TBranch        *b_ph_truth_isPhotonFromHardProc;   //!
   TBranch        *b_ph_truth_Rconv;   //!
   TBranch        *b_ph_truth_zconv;   //!
   TBranch        *b_ph_deltaEmax2;   //!
   TBranch        *b_ph_jet_dr;   //!
   TBranch        *b_ph_jet_matched;   //!
   TBranch        *b_ph_convIP;   //!
   TBranch        *b_ph_convIPRev;   //!
   TBranch        *b_ph_ptIsolationCone;   //!
   TBranch        *b_ph_ptIsolationConePhAngle;   //!
   TBranch        *b_ph_Etcone40_ED_corrected;   //!
   TBranch        *b_ph_Etcone40_corrected;   //!
   TBranch        *b_mu_staco_n;   //!
   TBranch        *b_mu_staco_E;   //!
   TBranch        *b_mu_staco_pt;   //!
   TBranch        *b_mu_staco_m;   //!
   TBranch        *b_mu_staco_eta;   //!
   TBranch        *b_mu_staco_phi;   //!
   TBranch        *b_mu_staco_px;   //!
   TBranch        *b_mu_staco_py;   //!
   TBranch        *b_mu_staco_pz;   //!
   TBranch        *b_mu_staco_charge;   //!
   TBranch        *b_mu_staco_allauthor;   //!
   TBranch        *b_mu_staco_author;   //!
   TBranch        *b_mu_staco_matchchi2;   //!
   TBranch        *b_mu_staco_matchndof;   //!
   TBranch        *b_mu_staco_etcone20;   //!
   TBranch        *b_mu_staco_etcone30;   //!
   TBranch        *b_mu_staco_etcone40;   //!
   TBranch        *b_mu_staco_nucone20;   //!
   TBranch        *b_mu_staco_nucone30;   //!
   TBranch        *b_mu_staco_nucone40;   //!
   TBranch        *b_mu_staco_ptcone20;   //!
   TBranch        *b_mu_staco_ptcone30;   //!
   TBranch        *b_mu_staco_ptcone40;   //!
   TBranch        *b_mu_staco_energyLossPar;   //!
   TBranch        *b_mu_staco_energyLossErr;   //!
   TBranch        *b_mu_staco_etCore;   //!
   TBranch        *b_mu_staco_energyLossType;   //!
   TBranch        *b_mu_staco_caloMuonIdTag;   //!
   TBranch        *b_mu_staco_caloLRLikelihood;   //!
   TBranch        *b_mu_staco_bestMatch;   //!
   TBranch        *b_mu_staco_isStandAloneMuon;   //!
   TBranch        *b_mu_staco_isCombinedMuon;   //!
   TBranch        *b_mu_staco_isLowPtReconstructedMuon;   //!
   TBranch        *b_mu_staco_isSegmentTaggedMuon;   //!
   TBranch        *b_mu_staco_isCaloMuonId;   //!
   TBranch        *b_mu_staco_alsoFoundByLowPt;   //!
   TBranch        *b_mu_staco_alsoFoundByCaloMuonId;   //!
   TBranch        *b_mu_staco_loose;   //!
   TBranch        *b_mu_staco_medium;   //!
   TBranch        *b_mu_staco_tight;   //!
   TBranch        *b_mu_staco_d0_exPV;   //!
   TBranch        *b_mu_staco_z0_exPV;   //!
   TBranch        *b_mu_staco_phi_exPV;   //!
   TBranch        *b_mu_staco_theta_exPV;   //!
   TBranch        *b_mu_staco_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cb_d0_exPV;   //!
   TBranch        *b_mu_staco_cb_z0_exPV;   //!
   TBranch        *b_mu_staco_cb_phi_exPV;   //!
   TBranch        *b_mu_staco_cb_theta_exPV;   //!
   TBranch        *b_mu_staco_cb_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_d0_exPV;   //!
   TBranch        *b_mu_staco_id_z0_exPV;   //!
   TBranch        *b_mu_staco_id_phi_exPV;   //!
   TBranch        *b_mu_staco_id_theta_exPV;   //!
   TBranch        *b_mu_staco_id_qoverp_exPV;   //!
   TBranch        *b_mu_staco_me_d0_exPV;   //!
   TBranch        *b_mu_staco_me_z0_exPV;   //!
   TBranch        *b_mu_staco_me_phi_exPV;   //!
   TBranch        *b_mu_staco_me_theta_exPV;   //!
   TBranch        *b_mu_staco_me_qoverp_exPV;   //!
   TBranch        *b_mu_staco_ie_d0_exPV;   //!
   TBranch        *b_mu_staco_ie_z0_exPV;   //!
   TBranch        *b_mu_staco_ie_phi_exPV;   //!
   TBranch        *b_mu_staco_ie_theta_exPV;   //!
   TBranch        *b_mu_staco_ie_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_exPV;   //!
   TBranch        *b_mu_staco_cov_phi_exPV;   //!
   TBranch        *b_mu_staco_cov_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_staco_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_staco_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_exPV;   //!
   TBranch        *b_mu_staco_id_cov_phi_exPV;   //!
   TBranch        *b_mu_staco_id_cov_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_staco_id_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_staco_id_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_staco_ms_d0;   //!
   TBranch        *b_mu_staco_ms_z0;   //!
   TBranch        *b_mu_staco_ms_phi;   //!
   TBranch        *b_mu_staco_ms_theta;   //!
   TBranch        *b_mu_staco_ms_qoverp;   //!
   TBranch        *b_mu_staco_id_d0;   //!
   TBranch        *b_mu_staco_id_z0;   //!
   TBranch        *b_mu_staco_id_phi;   //!
   TBranch        *b_mu_staco_id_theta;   //!
   TBranch        *b_mu_staco_id_qoverp;   //!
   TBranch        *b_mu_staco_me_d0;   //!
   TBranch        *b_mu_staco_me_z0;   //!
   TBranch        *b_mu_staco_me_phi;   //!
   TBranch        *b_mu_staco_me_theta;   //!
   TBranch        *b_mu_staco_me_qoverp;   //!
   TBranch        *b_mu_staco_ie_d0;   //!
   TBranch        *b_mu_staco_ie_z0;   //!
   TBranch        *b_mu_staco_ie_phi;   //!
   TBranch        *b_mu_staco_ie_theta;   //!
   TBranch        *b_mu_staco_ie_qoverp;   //!
   TBranch        *b_mu_staco_nOutliersOnTrack;   //!
   TBranch        *b_mu_staco_nBLHits;   //!
   TBranch        *b_mu_staco_nPixHits;   //!
   TBranch        *b_mu_staco_nSCTHits;   //!
   TBranch        *b_mu_staco_nTRTHits;   //!
   TBranch        *b_mu_staco_nTRTHighTHits;   //!
   TBranch        *b_mu_staco_nBLSharedHits;   //!
   TBranch        *b_mu_staco_nPixSharedHits;   //!
   TBranch        *b_mu_staco_nPixHoles;   //!
   TBranch        *b_mu_staco_nSCTSharedHits;   //!
   TBranch        *b_mu_staco_nSCTHoles;   //!
   TBranch        *b_mu_staco_nTRTOutliers;   //!
   TBranch        *b_mu_staco_nTRTHighTOutliers;   //!
   TBranch        *b_mu_staco_nGangedPixels;   //!
   TBranch        *b_mu_staco_nPixelDeadSensors;   //!
   TBranch        *b_mu_staco_nSCTDeadSensors;   //!
   TBranch        *b_mu_staco_nTRTDeadStraws;   //!
   TBranch        *b_mu_staco_expectBLayerHit;   //!
   TBranch        *b_mu_staco_nMDTHits;   //!
   TBranch        *b_mu_staco_nMDTHoles;   //!
   TBranch        *b_mu_staco_nCSCEtaHits;   //!
   TBranch        *b_mu_staco_nCSCEtaHoles;   //!
   TBranch        *b_mu_staco_nCSCPhiHits;   //!
   TBranch        *b_mu_staco_nCSCPhiHoles;   //!
   TBranch        *b_mu_staco_nRPCEtaHits;   //!
   TBranch        *b_mu_staco_nRPCEtaHoles;   //!
   TBranch        *b_mu_staco_nRPCPhiHits;   //!
   TBranch        *b_mu_staco_nRPCPhiHoles;   //!
   TBranch        *b_mu_staco_nTGCEtaHits;   //!
   TBranch        *b_mu_staco_nTGCEtaHoles;   //!
   TBranch        *b_mu_staco_nTGCPhiHits;   //!
   TBranch        *b_mu_staco_nTGCPhiHoles;   //!
   TBranch        *b_mu_staco_nMDTBIHits;   //!
   TBranch        *b_mu_staco_nMDTBMHits;   //!
   TBranch        *b_mu_staco_nMDTBOHits;   //!
   TBranch        *b_mu_staco_nMDTBEEHits;   //!
   TBranch        *b_mu_staco_nMDTBIS78Hits;   //!
   TBranch        *b_mu_staco_nMDTEIHits;   //!
   TBranch        *b_mu_staco_nMDTEMHits;   //!
   TBranch        *b_mu_staco_nMDTEOHits;   //!
   TBranch        *b_mu_staco_nMDTEEHits;   //!
   TBranch        *b_mu_staco_nRPCLayer1EtaHits;   //!
   TBranch        *b_mu_staco_nRPCLayer2EtaHits;   //!
   TBranch        *b_mu_staco_nRPCLayer3EtaHits;   //!
   TBranch        *b_mu_staco_nRPCLayer1PhiHits;   //!
   TBranch        *b_mu_staco_nRPCLayer2PhiHits;   //!
   TBranch        *b_mu_staco_nRPCLayer3PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer1EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer2EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer3EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer4EtaHits;   //!
   TBranch        *b_mu_staco_nTGCLayer1PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer2PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer3PhiHits;   //!
   TBranch        *b_mu_staco_nTGCLayer4PhiHits;   //!
   TBranch        *b_mu_staco_barrelSectors;   //!
   TBranch        *b_mu_staco_endcapSectors;   //!
   TBranch        *b_mu_staco_trackd0;   //!
   TBranch        *b_mu_staco_trackz0;   //!
   TBranch        *b_mu_staco_trackphi;   //!
   TBranch        *b_mu_staco_tracktheta;   //!
   TBranch        *b_mu_staco_trackqoverp;   //!
   TBranch        *b_mu_staco_trackcov_d0;   //!
   TBranch        *b_mu_staco_trackcov_z0;   //!
   TBranch        *b_mu_staco_trackcov_phi;   //!
   TBranch        *b_mu_staco_trackcov_theta;   //!
   TBranch        *b_mu_staco_trackcov_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_d0_z0;   //!
   TBranch        *b_mu_staco_trackcov_d0_phi;   //!
   TBranch        *b_mu_staco_trackcov_d0_theta;   //!
   TBranch        *b_mu_staco_trackcov_d0_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_z0_phi;   //!
   TBranch        *b_mu_staco_trackcov_z0_theta;   //!
   TBranch        *b_mu_staco_trackcov_z0_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_phi_theta;   //!
   TBranch        *b_mu_staco_trackcov_phi_qoverp;   //!
   TBranch        *b_mu_staco_trackcov_theta_qoverp;   //!
   TBranch        *b_mu_staco_hastrack;   //!
   TBranch        *b_mu_staco_truth_dr;   //!
   TBranch        *b_mu_staco_truth_E;   //!
   TBranch        *b_mu_staco_truth_pt;   //!
   TBranch        *b_mu_staco_truth_eta;   //!
   TBranch        *b_mu_staco_truth_phi;   //!
   TBranch        *b_mu_staco_truth_type;   //!
   TBranch        *b_mu_staco_truth_status;   //!
   TBranch        *b_mu_staco_truth_barcode;   //!
   TBranch        *b_mu_staco_truth_mothertype;   //!
   TBranch        *b_mu_staco_truth_motherbarcode;   //!
   TBranch        *b_mu_staco_truth_matched;   //!
   TBranch        *b_mu_staco_EFCB_dr;   //!
   TBranch        *b_mu_staco_EFCB_index;   //!
   TBranch        *b_mu_staco_EFMG_dr;   //!
   TBranch        *b_mu_staco_EFMG_index;   //!
   TBranch        *b_mu_staco_EFME_dr;   //!
   TBranch        *b_mu_staco_EFME_index;   //!
   TBranch        *b_mu_muid_n;   //!
   TBranch        *b_mu_muid_E;   //!
   TBranch        *b_mu_muid_pt;   //!
   TBranch        *b_mu_muid_m;   //!
   TBranch        *b_mu_muid_eta;   //!
   TBranch        *b_mu_muid_phi;   //!
   TBranch        *b_mu_muid_px;   //!
   TBranch        *b_mu_muid_py;   //!
   TBranch        *b_mu_muid_pz;   //!
   TBranch        *b_mu_muid_charge;   //!
   TBranch        *b_mu_muid_allauthor;   //!
   TBranch        *b_mu_muid_author;   //!
   TBranch        *b_mu_muid_matchchi2;   //!
   TBranch        *b_mu_muid_matchndof;   //!
   TBranch        *b_mu_muid_etcone20;   //!
   TBranch        *b_mu_muid_etcone30;   //!
   TBranch        *b_mu_muid_etcone40;   //!
   TBranch        *b_mu_muid_nucone20;   //!
   TBranch        *b_mu_muid_nucone30;   //!
   TBranch        *b_mu_muid_nucone40;   //!
   TBranch        *b_mu_muid_ptcone20;   //!
   TBranch        *b_mu_muid_ptcone30;   //!
   TBranch        *b_mu_muid_ptcone40;   //!
   TBranch        *b_mu_muid_energyLossPar;   //!
   TBranch        *b_mu_muid_energyLossErr;   //!
   TBranch        *b_mu_muid_etCore;   //!
   TBranch        *b_mu_muid_energyLossType;   //!
   TBranch        *b_mu_muid_caloMuonIdTag;   //!
   TBranch        *b_mu_muid_caloLRLikelihood;   //!
   TBranch        *b_mu_muid_bestMatch;   //!
   TBranch        *b_mu_muid_isStandAloneMuon;   //!
   TBranch        *b_mu_muid_isCombinedMuon;   //!
   TBranch        *b_mu_muid_isLowPtReconstructedMuon;   //!
   TBranch        *b_mu_muid_isSegmentTaggedMuon;   //!
   TBranch        *b_mu_muid_isCaloMuonId;   //!
   TBranch        *b_mu_muid_alsoFoundByLowPt;   //!
   TBranch        *b_mu_muid_alsoFoundByCaloMuonId;   //!
   TBranch        *b_mu_muid_loose;   //!
   TBranch        *b_mu_muid_medium;   //!
   TBranch        *b_mu_muid_tight;   //!
   TBranch        *b_mu_muid_d0_exPV;   //!
   TBranch        *b_mu_muid_z0_exPV;   //!
   TBranch        *b_mu_muid_phi_exPV;   //!
   TBranch        *b_mu_muid_theta_exPV;   //!
   TBranch        *b_mu_muid_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cb_d0_exPV;   //!
   TBranch        *b_mu_muid_cb_z0_exPV;   //!
   TBranch        *b_mu_muid_cb_phi_exPV;   //!
   TBranch        *b_mu_muid_cb_theta_exPV;   //!
   TBranch        *b_mu_muid_cb_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_d0_exPV;   //!
   TBranch        *b_mu_muid_id_z0_exPV;   //!
   TBranch        *b_mu_muid_id_phi_exPV;   //!
   TBranch        *b_mu_muid_id_theta_exPV;   //!
   TBranch        *b_mu_muid_id_qoverp_exPV;   //!
   TBranch        *b_mu_muid_me_d0_exPV;   //!
   TBranch        *b_mu_muid_me_z0_exPV;   //!
   TBranch        *b_mu_muid_me_phi_exPV;   //!
   TBranch        *b_mu_muid_me_theta_exPV;   //!
   TBranch        *b_mu_muid_me_qoverp_exPV;   //!
   TBranch        *b_mu_muid_ie_d0_exPV;   //!
   TBranch        *b_mu_muid_ie_z0_exPV;   //!
   TBranch        *b_mu_muid_ie_phi_exPV;   //!
   TBranch        *b_mu_muid_ie_theta_exPV;   //!
   TBranch        *b_mu_muid_ie_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_exPV;   //!
   TBranch        *b_mu_muid_cov_phi_exPV;   //!
   TBranch        *b_mu_muid_cov_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_muid_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_muid_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_exPV;   //!
   TBranch        *b_mu_muid_id_cov_phi_exPV;   //!
   TBranch        *b_mu_muid_id_cov_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_z0_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_phi_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_d0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_phi_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_z0_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_phi_theta_exPV;   //!
   TBranch        *b_mu_muid_id_cov_phi_qoverp_exPV;   //!
   TBranch        *b_mu_muid_id_cov_theta_qoverp_exPV;   //!
   TBranch        *b_mu_muid_ms_d0;   //!
   TBranch        *b_mu_muid_ms_z0;   //!
   TBranch        *b_mu_muid_ms_phi;   //!
   TBranch        *b_mu_muid_ms_theta;   //!
   TBranch        *b_mu_muid_ms_qoverp;   //!
   TBranch        *b_mu_muid_id_d0;   //!
   TBranch        *b_mu_muid_id_z0;   //!
   TBranch        *b_mu_muid_id_phi;   //!
   TBranch        *b_mu_muid_id_theta;   //!
   TBranch        *b_mu_muid_id_qoverp;   //!
   TBranch        *b_mu_muid_me_d0;   //!
   TBranch        *b_mu_muid_me_z0;   //!
   TBranch        *b_mu_muid_me_phi;   //!
   TBranch        *b_mu_muid_me_theta;   //!
   TBranch        *b_mu_muid_me_qoverp;   //!
   TBranch        *b_mu_muid_ie_d0;   //!
   TBranch        *b_mu_muid_ie_z0;   //!
   TBranch        *b_mu_muid_ie_phi;   //!
   TBranch        *b_mu_muid_ie_theta;   //!
   TBranch        *b_mu_muid_ie_qoverp;   //!
   TBranch        *b_mu_muid_nOutliersOnTrack;   //!
   TBranch        *b_mu_muid_nBLHits;   //!
   TBranch        *b_mu_muid_nPixHits;   //!
   TBranch        *b_mu_muid_nSCTHits;   //!
   TBranch        *b_mu_muid_nTRTHits;   //!
   TBranch        *b_mu_muid_nTRTHighTHits;   //!
   TBranch        *b_mu_muid_nBLSharedHits;   //!
   TBranch        *b_mu_muid_nPixSharedHits;   //!
   TBranch        *b_mu_muid_nPixHoles;   //!
   TBranch        *b_mu_muid_nSCTSharedHits;   //!
   TBranch        *b_mu_muid_nSCTHoles;   //!
   TBranch        *b_mu_muid_nTRTOutliers;   //!
   TBranch        *b_mu_muid_nTRTHighTOutliers;   //!
   TBranch        *b_mu_muid_nGangedPixels;   //!
   TBranch        *b_mu_muid_nPixelDeadSensors;   //!
   TBranch        *b_mu_muid_nSCTDeadSensors;   //!
   TBranch        *b_mu_muid_nTRTDeadStraws;   //!
   TBranch        *b_mu_muid_expectBLayerHit;   //!
   TBranch        *b_mu_muid_nMDTHits;   //!
   TBranch        *b_mu_muid_nMDTHoles;   //!
   TBranch        *b_mu_muid_nCSCEtaHits;   //!
   TBranch        *b_mu_muid_nCSCEtaHoles;   //!
   TBranch        *b_mu_muid_nCSCPhiHits;   //!
   TBranch        *b_mu_muid_nCSCPhiHoles;   //!
   TBranch        *b_mu_muid_nRPCEtaHits;   //!
   TBranch        *b_mu_muid_nRPCEtaHoles;   //!
   TBranch        *b_mu_muid_nRPCPhiHits;   //!
   TBranch        *b_mu_muid_nRPCPhiHoles;   //!
   TBranch        *b_mu_muid_nTGCEtaHits;   //!
   TBranch        *b_mu_muid_nTGCEtaHoles;   //!
   TBranch        *b_mu_muid_nTGCPhiHits;   //!
   TBranch        *b_mu_muid_nTGCPhiHoles;   //!
   TBranch        *b_mu_muid_nMDTBIHits;   //!
   TBranch        *b_mu_muid_nMDTBMHits;   //!
   TBranch        *b_mu_muid_nMDTBOHits;   //!
   TBranch        *b_mu_muid_nMDTBEEHits;   //!
   TBranch        *b_mu_muid_nMDTBIS78Hits;   //!
   TBranch        *b_mu_muid_nMDTEIHits;   //!
   TBranch        *b_mu_muid_nMDTEMHits;   //!
   TBranch        *b_mu_muid_nMDTEOHits;   //!
   TBranch        *b_mu_muid_nMDTEEHits;   //!
   TBranch        *b_mu_muid_nRPCLayer1EtaHits;   //!
   TBranch        *b_mu_muid_nRPCLayer2EtaHits;   //!
   TBranch        *b_mu_muid_nRPCLayer3EtaHits;   //!
   TBranch        *b_mu_muid_nRPCLayer1PhiHits;   //!
   TBranch        *b_mu_muid_nRPCLayer2PhiHits;   //!
   TBranch        *b_mu_muid_nRPCLayer3PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer1EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer2EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer3EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer4EtaHits;   //!
   TBranch        *b_mu_muid_nTGCLayer1PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer2PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer3PhiHits;   //!
   TBranch        *b_mu_muid_nTGCLayer4PhiHits;   //!
   TBranch        *b_mu_muid_barrelSectors;   //!
   TBranch        *b_mu_muid_endcapSectors;   //!
   TBranch        *b_mu_muid_trackd0;   //!
   TBranch        *b_mu_muid_trackz0;   //!
   TBranch        *b_mu_muid_trackphi;   //!
   TBranch        *b_mu_muid_tracktheta;   //!
   TBranch        *b_mu_muid_trackqoverp;   //!
   TBranch        *b_mu_muid_trackcov_d0;   //!
   TBranch        *b_mu_muid_trackcov_z0;   //!
   TBranch        *b_mu_muid_trackcov_phi;   //!
   TBranch        *b_mu_muid_trackcov_theta;   //!
   TBranch        *b_mu_muid_trackcov_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_d0_z0;   //!
   TBranch        *b_mu_muid_trackcov_d0_phi;   //!
   TBranch        *b_mu_muid_trackcov_d0_theta;   //!
   TBranch        *b_mu_muid_trackcov_d0_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_z0_phi;   //!
   TBranch        *b_mu_muid_trackcov_z0_theta;   //!
   TBranch        *b_mu_muid_trackcov_z0_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_phi_theta;   //!
   TBranch        *b_mu_muid_trackcov_phi_qoverp;   //!
   TBranch        *b_mu_muid_trackcov_theta_qoverp;   //!
   TBranch        *b_mu_muid_hastrack;   //!
   TBranch        *b_mu_muid_truth_dr;   //!
   TBranch        *b_mu_muid_truth_E;   //!
   TBranch        *b_mu_muid_truth_pt;   //!
   TBranch        *b_mu_muid_truth_eta;   //!
   TBranch        *b_mu_muid_truth_phi;   //!
   TBranch        *b_mu_muid_truth_type;   //!
   TBranch        *b_mu_muid_truth_status;   //!
   TBranch        *b_mu_muid_truth_barcode;   //!
   TBranch        *b_mu_muid_truth_mothertype;   //!
   TBranch        *b_mu_muid_truth_motherbarcode;   //!
   TBranch        *b_mu_muid_truth_matched;   //!
   TBranch        *b_mu_muid_EFCB_dr;   //!
   TBranch        *b_mu_muid_EFCB_index;   //!
   TBranch        *b_mu_muid_EFMG_dr;   //!
   TBranch        *b_mu_muid_EFMG_index;   //!
   TBranch        *b_mu_muid_EFME_dr;   //!
   TBranch        *b_mu_muid_EFME_index;   //!
   TBranch        *b_trueTau_n;   //!
   TBranch        *b_trueTau_pt;   //!
   TBranch        *b_trueTau_m;   //!
   TBranch        *b_trueTau_eta;   //!
   TBranch        *b_trueTau_phi;   //!
   TBranch        *b_trueTau_vis_m;   //!
   TBranch        *b_trueTau_vis_Et;   //!
   TBranch        *b_trueTau_vis_eta;   //!
   TBranch        *b_trueTau_vis_phi;   //!
   TBranch        *b_trueTau_nProng;   //!
   TBranch        *b_trueTau_nPi0;   //!
   TBranch        *b_trueTau_charge;   //!
   TBranch        *b_trueTau_tauAssocSmall_dr;   //!
   TBranch        *b_trueTau_tauAssocSmall_index;   //!
   TBranch        *b_trueTau_tauAssocSmall_matched;   //!
   TBranch        *b_tau_n;   //!
   TBranch        *b_tau_Et;   //!
   TBranch        *b_tau_pt;   //!
   TBranch        *b_tau_m;   //!
   TBranch        *b_tau_eta;   //!
   TBranch        *b_tau_phi;   //!
   TBranch        *b_tau_charge;   //!
   TBranch        *b_tau_BDTEleScore;   //!
   TBranch        *b_tau_BDTJetScore;   //!
   TBranch        *b_tau_likelihood;   //!
   TBranch        *b_tau_SafeLikelihood;   //!
   TBranch        *b_tau_electronVetoLoose;   //!
   TBranch        *b_tau_electronVetoMedium;   //!
   TBranch        *b_tau_electronVetoTight;   //!
   TBranch        *b_tau_muonVeto;   //!
   TBranch        *b_tau_tauCutLoose;   //!
   TBranch        *b_tau_tauCutMedium;   //!
   TBranch        *b_tau_tauCutTight;   //!
   TBranch        *b_tau_tauLlhLoose;   //!
   TBranch        *b_tau_tauLlhMedium;   //!
   TBranch        *b_tau_tauLlhTight;   //!
   TBranch        *b_tau_JetBDTSigLoose;   //!
   TBranch        *b_tau_JetBDTSigMedium;   //!
   TBranch        *b_tau_JetBDTSigTight;   //!
   TBranch        *b_tau_EleBDTLoose;   //!
   TBranch        *b_tau_EleBDTMedium;   //!
   TBranch        *b_tau_EleBDTTight;   //!
   TBranch        *b_tau_author;   //!
   TBranch        *b_tau_RoIWord;   //!
   TBranch        *b_tau_nProng;   //!
   TBranch        *b_tau_numTrack;   //!
   TBranch        *b_tau_seedCalo_numTrack;   //!
   TBranch        *b_tau_etOverPtLeadTrk;   //!
   TBranch        *b_tau_ipZ0SinThetaSigLeadTrk;   //!
   TBranch        *b_tau_leadTrkPt;   //!
   TBranch        *b_tau_nLooseTrk;   //!
   TBranch        *b_tau_nLooseConvTrk;   //!
   TBranch        *b_tau_nProngLoose;   //!
   TBranch        *b_tau_ipSigLeadTrk;   //!
   TBranch        *b_tau_ipSigLeadLooseTrk;   //!
   TBranch        *b_tau_etOverPtLeadLooseTrk;   //!
   TBranch        *b_tau_leadLooseTrkPt;   //!
   TBranch        *b_tau_chrgLooseTrk;   //!
   TBranch        *b_tau_massTrkSys;   //!
   TBranch        *b_tau_trkWidth2;   //!
   TBranch        *b_tau_trFlightPathSig;   //!
   TBranch        *b_tau_etEflow;   //!
   TBranch        *b_tau_mEflow;   //!
   TBranch        *b_tau_nPi0;   //!
   TBranch        *b_tau_ele_E237E277;   //!
   TBranch        *b_tau_ele_PresamplerFraction;   //!
   TBranch        *b_tau_ele_ECALFirstFraction;   //!
   TBranch        *b_tau_EM_TES_scale;   //!
   TBranch        *b_tau_LC_TES_precalib;   //!
   TBranch        *b_tau_seedCalo_EMRadius;   //!
   TBranch        *b_tau_seedCalo_hadRadius;   //!
   TBranch        *b_tau_seedCalo_etEMAtEMScale;   //!
   TBranch        *b_tau_seedCalo_etHadAtEMScale;   //!
   TBranch        *b_tau_seedCalo_isolFrac;   //!
   TBranch        *b_tau_seedCalo_centFrac;   //!
   TBranch        *b_tau_seedCalo_stripWidth2;   //!
   TBranch        *b_tau_seedCalo_nStrip;   //!
   TBranch        *b_tau_seedCalo_eta;   //!
   TBranch        *b_tau_seedCalo_phi;   //!
   TBranch        *b_tau_seedCalo_nIsolLooseTrk;   //!
   TBranch        *b_tau_seedCalo_trkAvgDist;   //!
   TBranch        *b_tau_seedCalo_trkRmsDist;   //!
   TBranch        *b_tau_seedCalo_caloIso;   //!
   TBranch        *b_tau_seedCalo_caloIsoCorrected;   //!
   TBranch        *b_tau_seedCalo_dRmax;   //!
   TBranch        *b_tau_seedCalo_lead2ClusterEOverAllClusterE;   //!
   TBranch        *b_tau_seedCalo_lead3ClusterEOverAllClusterE;   //!
   TBranch        *b_tau_numTopoClusters;   //!
   TBranch        *b_tau_numEffTopoClusters;   //!
   TBranch        *b_tau_topoInvMass;   //!
   TBranch        *b_tau_effTopoInvMass;   //!
   TBranch        *b_tau_topoMeanDeltaR;   //!
   TBranch        *b_tau_effTopoMeanDeltaR;   //!
   TBranch        *b_tau_numCells;   //!
   TBranch        *b_tau_seedTrk_EMRadius;   //!
   TBranch        *b_tau_seedTrk_isolFrac;   //!
   TBranch        *b_tau_seedTrk_etChrgHadOverSumTrkPt;   //!
   TBranch        *b_tau_seedTrk_isolFracWide;   //!
   TBranch        *b_tau_seedTrk_etHadAtEMScale;   //!
   TBranch        *b_tau_seedTrk_etEMAtEMScale;   //!
   TBranch        *b_tau_seedTrk_etEMCL;   //!
   TBranch        *b_tau_seedTrk_etChrgEM;   //!
   TBranch        *b_tau_seedTrk_etNeuEM;   //!
   TBranch        *b_tau_seedTrk_etResNeuEM;   //!
   TBranch        *b_tau_seedTrk_hadLeakEt;   //!
   TBranch        *b_tau_seedTrk_sumEMCellEtOverLeadTrkPt;   //!
   TBranch        *b_tau_seedTrk_secMaxStripEt;   //!
   TBranch        *b_tau_seedTrk_stripWidth2;   //!
   TBranch        *b_tau_seedTrk_nStrip;   //!
   TBranch        *b_tau_seedTrk_etChrgHad;   //!
   TBranch        *b_tau_seedTrk_nOtherCoreTrk;   //!
   TBranch        *b_tau_seedTrk_nIsolTrk;   //!
   TBranch        *b_tau_seedTrk_etIsolEM;   //!
   TBranch        *b_tau_seedTrk_etIsolHad;   //!
   TBranch        *b_tau_calcVars_etHad_EMScale_Pt3Trks;   //!
   TBranch        *b_tau_calcVars_etEM_EMScale_Pt3Trks;   //!
   TBranch        *b_tau_calcVars_ipSigLeadLooseTrk;   //!
   TBranch        *b_tau_calcVars_drMin;   //!
   TBranch        *b_tau_calcVars_TRTHTOverLT_LeadTrk;   //!
   TBranch        *b_tau_calcVars_calRadius;   //!
   TBranch        *b_tau_calcVars_EMFractionAtEMScale;   //!
   TBranch        *b_tau_calcVars_trackIso;   //!
   TBranch        *b_tau_calcVars_ChPiEMEOverCaloEME;   //!
   TBranch        *b_tau_calcVars_PSSFraction;   //!
   TBranch        *b_tau_calcVars_EMPOverTrkSysP;   //!
   TBranch        *b_tau_cluster_E;   //!
   TBranch        *b_tau_cluster_eta;   //!
   TBranch        *b_tau_cluster_phi;   //!
   TBranch        *b_tau_cluster_PreSamplerStripF;   //!
   TBranch        *b_tau_cluster_EMLayer2F;   //!
   TBranch        *b_tau_cluster_EMLayer3F;   //!
   TBranch        *b_tau_cluster_n;   //!
   TBranch        *b_tau_Pi0Cluster_pt;   //!
   TBranch        *b_tau_Pi0Cluster_eta;   //!
   TBranch        *b_tau_Pi0Cluster_phi;   //!
   TBranch        *b_tau_secvtx_x;   //!
   TBranch        *b_tau_secvtx_y;   //!
   TBranch        *b_tau_secvtx_z;   //!
   TBranch        *b_tau_secvtx_chiSquared;   //!
   TBranch        *b_tau_secvtx_numberDoF;   //!
   TBranch        *b_tau_jet_Et;   //!
   TBranch        *b_tau_jet_pt;   //!
   TBranch        *b_tau_jet_m;   //!
   TBranch        *b_tau_jet_eta;   //!
   TBranch        *b_tau_jet_phi;   //!
   TBranch        *b_tau_jet_WIDTH;   //!
   TBranch        *b_tau_jet_n90;   //!
   TBranch        *b_tau_jet_n90constituents;   //!
   TBranch        *b_tau_jet_BCH_CORR_CELL;   //!
   TBranch        *b_tau_jet_BCH_CORR_JET;   //!
   TBranch        *b_tau_jet_BCH_CORR_JET_FORCELL;   //!
   TBranch        *b_tau_jet_ENG_BAD_CELLS;   //!
   TBranch        *b_tau_jet_N_BAD_CELLS;   //!
   TBranch        *b_tau_jet_N_BAD_CELLS_CORR;   //!
   TBranch        *b_tau_jet_BAD_CELLS_CORR_E;   //!
   TBranch        *b_tau_jet_Timing;   //!
   TBranch        *b_tau_jet_LArQuality;   //!
   TBranch        *b_tau_jet_nTrk;   //!
   TBranch        *b_tau_jet_sumPtTrk;   //!
   TBranch        *b_tau_jet_OriginIndex;   //!
   TBranch        *b_tau_jet_HECQuality;   //!
   TBranch        *b_tau_jet_NegativeE;   //!
   TBranch        *b_tau_jet_YFlip12;   //!
   TBranch        *b_tau_jet_YFlip23;   //!
   TBranch        *b_tau_jet_BCH_CORR_DOTX;   //!
   TBranch        *b_tau_jet_SamplingMax;   //!
   TBranch        *b_tau_jet_fracSamplingMax;   //!
   TBranch        *b_tau_jet_hecf;   //!
   TBranch        *b_tau_jet_tgap3f;   //!
   TBranch        *b_tau_jet_isUgly;   //!
   TBranch        *b_tau_jet_isBadLoose;   //!
   TBranch        *b_tau_jet_isBadMedium;   //!
   TBranch        *b_tau_jet_isBadTight;   //!
   TBranch        *b_tau_jet_emfrac;   //!
   TBranch        *b_tau_jet_GCWJES;   //!
   TBranch        *b_tau_jet_EMJES;   //!
   TBranch        *b_tau_jet_emscale_E;   //!
   TBranch        *b_tau_jet_emscale_pt;   //!
   TBranch        *b_tau_jet_emscale_m;   //!
   TBranch        *b_tau_jet_emscale_eta;   //!
   TBranch        *b_tau_jet_emscale_phi;   //!
   TBranch        *b_tau_jet_jvtx_x;   //!
   TBranch        *b_tau_jet_jvtx_y;   //!
   TBranch        *b_tau_jet_jvtx_z;   //!
   TBranch        *b_tau_jet_jvtxf;   //!
   TBranch        *b_tau_jet_flavor_weight;   //!
   TBranch        *b_tau_jet_flavor_weight_TrackCounting2D;   //!
   TBranch        *b_tau_jet_flavor_weight_JetProb;   //!
   TBranch        *b_tau_jet_flavor_weight_IP1D;   //!
   TBranch        *b_tau_jet_flavor_weight_IP2D;   //!
   TBranch        *b_tau_jet_flavor_weight_IP3D;   //!
   TBranch        *b_tau_jet_flavor_weight_SV0;   //!
   TBranch        *b_tau_jet_flavor_weight_SV1;   //!
   TBranch        *b_tau_jet_flavor_weight_SV2;   //!
   TBranch        *b_tau_jet_flavor_weight_JetFitterTag;   //!
   TBranch        *b_tau_jet_flavor_weight_JetFitterCOMB;   //!
   TBranch        *b_tau_jet_flavor_weight_JetFitterTagNN;   //!
   TBranch        *b_tau_jet_flavor_weight_JetFitterCOMBNN;   //!
   TBranch        *b_tau_jet_flavor_weight_SoftMuonTag;   //!
   TBranch        *b_tau_jet_flavor_weight_SoftElectronTag;   //!
   TBranch        *b_tau_jet_flavor_weight_IP3DSV1;   //!
   TBranch        *b_tau_jet_e_PreSamplerB;   //!
   TBranch        *b_tau_jet_e_EMB1;   //!
   TBranch        *b_tau_jet_e_EMB2;   //!
   TBranch        *b_tau_jet_e_EMB3;   //!
   TBranch        *b_tau_jet_e_PreSamplerE;   //!
   TBranch        *b_tau_jet_e_EME1;   //!
   TBranch        *b_tau_jet_e_EME2;   //!
   TBranch        *b_tau_jet_e_EME3;   //!
   TBranch        *b_tau_jet_e_HEC0;   //!
   TBranch        *b_tau_jet_e_HEC1;   //!
   TBranch        *b_tau_jet_e_HEC2;   //!
   TBranch        *b_tau_jet_e_HEC3;   //!
   TBranch        *b_tau_jet_e_TileBar0;   //!
   TBranch        *b_tau_jet_e_TileBar1;   //!
   TBranch        *b_tau_jet_e_TileBar2;   //!
   TBranch        *b_tau_jet_e_TileGap1;   //!
   TBranch        *b_tau_jet_e_TileGap2;   //!
   TBranch        *b_tau_jet_e_TileGap3;   //!
   TBranch        *b_tau_jet_e_TileExt0;   //!
   TBranch        *b_tau_jet_e_TileExt1;   //!
   TBranch        *b_tau_jet_e_TileExt2;   //!
   TBranch        *b_tau_jet_e_FCAL0;   //!
   TBranch        *b_tau_jet_e_FCAL1;   //!
   TBranch        *b_tau_jet_e_FCAL2;   //!
   TBranch        *b_tau_jet_shapeBins;   //!
   TBranch        *b_tau_seedCalo_track_n;   //!
   TBranch        *b_tau_seedCalo_track_d0;   //!
   TBranch        *b_tau_seedCalo_track_z0;   //!
   TBranch        *b_tau_seedCalo_track_phi;   //!
   TBranch        *b_tau_seedCalo_track_theta;   //!
   TBranch        *b_tau_seedCalo_track_qoverp;   //!
   TBranch        *b_tau_seedCalo_track_pt;   //!
   TBranch        *b_tau_seedCalo_track_eta;   //!
   TBranch        *b_tau_seedCalo_track_atPV_d0;   //!
   TBranch        *b_tau_seedCalo_track_atPV_z0;   //!
   TBranch        *b_tau_seedCalo_track_atPV_phi;   //!
   TBranch        *b_tau_seedCalo_track_atPV_theta;   //!
   TBranch        *b_tau_seedCalo_track_atPV_qoverp;   //!
   TBranch        *b_tau_seedCalo_track_atPV_pt;   //!
   TBranch        *b_tau_seedCalo_track_atPV_eta;   //!
   TBranch        *b_tau_seedCalo_track_nBLHits;   //!
   TBranch        *b_tau_seedCalo_track_nPixHits;   //!
   TBranch        *b_tau_seedCalo_track_nSCTHits;   //!
   TBranch        *b_tau_seedCalo_track_nTRTHits;   //!
   TBranch        *b_tau_seedCalo_track_nTRTHighTHits;   //!
   TBranch        *b_tau_seedCalo_track_nPixHoles;   //!
   TBranch        *b_tau_seedCalo_track_nSCTHoles;   //!
   TBranch        *b_tau_seedCalo_track_nTRTHoles;   //!
   TBranch        *b_tau_seedCalo_track_nBLSharedHits;   //!
   TBranch        *b_tau_seedCalo_track_nPixSharedHits;   //!
   TBranch        *b_tau_seedCalo_track_nSCTSharedHits;   //!
   TBranch        *b_tau_seedCalo_track_nBLayerOutliers;   //!
   TBranch        *b_tau_seedCalo_track_nPixelOutliers;   //!
   TBranch        *b_tau_seedCalo_track_nSCTOutliers;   //!
   TBranch        *b_tau_seedCalo_track_nTRTOutliers;   //!
   TBranch        *b_tau_seedCalo_track_nTRTHighTOutliers;   //!
   TBranch        *b_tau_seedCalo_track_nContribPixelLayers;   //!
   TBranch        *b_tau_seedCalo_track_nGangedPixels;   //!
   TBranch        *b_tau_seedCalo_track_nGangedFlaggedFakes;   //!
   TBranch        *b_tau_seedCalo_track_nPixelDeadSensors;   //!
   TBranch        *b_tau_seedCalo_track_nPixelSpoiltHits;   //!
   TBranch        *b_tau_seedCalo_track_nSCTDoubleHoles;   //!
   TBranch        *b_tau_seedCalo_track_nSCTDeadSensors;   //!
   TBranch        *b_tau_seedCalo_track_nSCTSpoiltHits;   //!
   TBranch        *b_tau_seedCalo_track_expectBLayerHit;   //!
   TBranch        *b_tau_seedCalo_track_nHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_n;   //!
   TBranch        *b_tau_seedCalo_wideTrk_d0;   //!
   TBranch        *b_tau_seedCalo_wideTrk_z0;   //!
   TBranch        *b_tau_seedCalo_wideTrk_phi;   //!
   TBranch        *b_tau_seedCalo_wideTrk_theta;   //!
   TBranch        *b_tau_seedCalo_wideTrk_qoverp;   //!
   TBranch        *b_tau_seedCalo_wideTrk_pt;   //!
   TBranch        *b_tau_seedCalo_wideTrk_eta;   //!
   TBranch        *b_tau_seedCalo_wideTrk_atPV_d0;   //!
   TBranch        *b_tau_seedCalo_wideTrk_atPV_z0;   //!
   TBranch        *b_tau_seedCalo_wideTrk_atPV_phi;   //!
   TBranch        *b_tau_seedCalo_wideTrk_atPV_theta;   //!
   TBranch        *b_tau_seedCalo_wideTrk_atPV_qoverp;   //!
   TBranch        *b_tau_seedCalo_wideTrk_atPV_pt;   //!
   TBranch        *b_tau_seedCalo_wideTrk_atPV_eta;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nBLHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nPixHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nSCTHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nTRTHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nTRTHighTHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nPixHoles;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nSCTHoles;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nTRTHoles;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nBLSharedHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nPixSharedHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nSCTSharedHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nBLayerOutliers;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nPixelOutliers;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nSCTOutliers;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nTRTOutliers;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nTRTHighTOutliers;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nContribPixelLayers;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nGangedPixels;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nGangedFlaggedFakes;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nPixelDeadSensors;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nPixelSpoiltHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nSCTDoubleHoles;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nSCTDeadSensors;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nSCTSpoiltHits;   //!
   TBranch        *b_tau_seedCalo_wideTrk_expectBLayerHit;   //!
   TBranch        *b_tau_seedCalo_wideTrk_nHits;   //!
   TBranch        *b_tau_otherTrk_n;   //!
   TBranch        *b_tau_otherTrk_d0;   //!
   TBranch        *b_tau_otherTrk_z0;   //!
   TBranch        *b_tau_otherTrk_phi;   //!
   TBranch        *b_tau_otherTrk_theta;   //!
   TBranch        *b_tau_otherTrk_qoverp;   //!
   TBranch        *b_tau_otherTrk_pt;   //!
   TBranch        *b_tau_otherTrk_eta;   //!
   TBranch        *b_tau_otherTrk_atPV_d0;   //!
   TBranch        *b_tau_otherTrk_atPV_z0;   //!
   TBranch        *b_tau_otherTrk_atPV_phi;   //!
   TBranch        *b_tau_otherTrk_atPV_theta;   //!
   TBranch        *b_tau_otherTrk_atPV_qoverp;   //!
   TBranch        *b_tau_otherTrk_atPV_pt;   //!
   TBranch        *b_tau_otherTrk_atPV_eta;   //!
   TBranch        *b_tau_otherTrk_nBLHits;   //!
   TBranch        *b_tau_otherTrk_nPixHits;   //!
   TBranch        *b_tau_otherTrk_nSCTHits;   //!
   TBranch        *b_tau_otherTrk_nTRTHits;   //!
   TBranch        *b_tau_otherTrk_nTRTHighTHits;   //!
   TBranch        *b_tau_otherTrk_nPixHoles;   //!
   TBranch        *b_tau_otherTrk_nSCTHoles;   //!
   TBranch        *b_tau_otherTrk_nTRTHoles;   //!
   TBranch        *b_tau_otherTrk_nBLSharedHits;   //!
   TBranch        *b_tau_otherTrk_nPixSharedHits;   //!
   TBranch        *b_tau_otherTrk_nSCTSharedHits;   //!
   TBranch        *b_tau_otherTrk_nBLayerOutliers;   //!
   TBranch        *b_tau_otherTrk_nPixelOutliers;   //!
   TBranch        *b_tau_otherTrk_nSCTOutliers;   //!
   TBranch        *b_tau_otherTrk_nTRTOutliers;   //!
   TBranch        *b_tau_otherTrk_nTRTHighTOutliers;   //!
   TBranch        *b_tau_otherTrk_nContribPixelLayers;   //!
   TBranch        *b_tau_otherTrk_nGangedPixels;   //!
   TBranch        *b_tau_otherTrk_nGangedFlaggedFakes;   //!
   TBranch        *b_tau_otherTrk_nPixelDeadSensors;   //!
   TBranch        *b_tau_otherTrk_nPixelSpoiltHits;   //!
   TBranch        *b_tau_otherTrk_nSCTDoubleHoles;   //!
   TBranch        *b_tau_otherTrk_nSCTDeadSensors;   //!
   TBranch        *b_tau_otherTrk_nSCTSpoiltHits;   //!
   TBranch        *b_tau_otherTrk_expectBLayerHit;   //!
   TBranch        *b_tau_otherTrk_nHits;   //!
   TBranch        *b_tau_EF_dr;   //!
   TBranch        *b_tau_EF_E;   //!
   TBranch        *b_tau_EF_Et;   //!
   TBranch        *b_tau_EF_pt;   //!
   TBranch        *b_tau_EF_eta;   //!
   TBranch        *b_tau_EF_phi;   //!
   TBranch        *b_tau_EF_matched;   //!
   TBranch        *b_tau_L2_dr;   //!
   TBranch        *b_tau_L2_E;   //!
   TBranch        *b_tau_L2_Et;   //!
   TBranch        *b_tau_L2_pt;   //!
   TBranch        *b_tau_L2_eta;   //!
   TBranch        *b_tau_L2_phi;   //!
   TBranch        *b_tau_L2_matched;   //!
   TBranch        *b_tau_L1_dr;   //!
   TBranch        *b_tau_L1_Et;   //!
   TBranch        *b_tau_L1_pt;   //!
   TBranch        *b_tau_L1_eta;   //!
   TBranch        *b_tau_L1_phi;   //!
   TBranch        *b_tau_L1_matched;   //!
   TBranch        *b_tau_trueTauAssocSmall_dr;   //!
   TBranch        *b_tau_trueTauAssocSmall_index;   //!
   TBranch        *b_tau_trueTauAssocSmall_matched;   //!
   TBranch        *b_MET_RefFinal_etx;   //!
   TBranch        *b_MET_RefFinal_ety;   //!
   TBranch        *b_MET_RefFinal_phi;   //!
   TBranch        *b_MET_RefFinal_et;   //!
   TBranch        *b_MET_RefFinal_sumet;   //!
   TBranch        *b_MET_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_RefEle_etx;   //!
   TBranch        *b_MET_RefEle_ety;   //!
   TBranch        *b_MET_RefEle_sumet;   //!
   TBranch        *b_MET_RefJet_etx;   //!
   TBranch        *b_MET_RefJet_ety;   //!
   TBranch        *b_MET_RefJet_sumet;   //!
   TBranch        *b_MET_SoftJets_etx;   //!
   TBranch        *b_MET_SoftJets_ety;   //!
   TBranch        *b_MET_SoftJets_sumet;   //!
   TBranch        *b_MET_RefMuon_etx;   //!
   TBranch        *b_MET_RefMuon_ety;   //!
   TBranch        *b_MET_RefMuon_sumet;   //!
   TBranch        *b_MET_RefMuon_Staco_etx;   //!
   TBranch        *b_MET_RefMuon_Staco_ety;   //!
   TBranch        *b_MET_RefMuon_Staco_sumet;   //!
   TBranch        *b_MET_RefMuon_Muid_etx;   //!
   TBranch        *b_MET_RefMuon_Muid_ety;   //!
   TBranch        *b_MET_RefMuon_Muid_sumet;   //!
   TBranch        *b_MET_RefTau_etx;   //!
   TBranch        *b_MET_RefTau_ety;   //!
   TBranch        *b_MET_RefTau_sumet;   //!
   TBranch        *b_MET_CellOut_etx;   //!
   TBranch        *b_MET_CellOut_ety;   //!
   TBranch        *b_MET_CellOut_sumet;   //!
   TBranch        *b_MET_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_Track_etx;   //!
   TBranch        *b_MET_Track_ety;   //!
   TBranch        *b_MET_Track_sumet;   //!
   TBranch        *b_MET_RefFinal_em_etx;   //!
   TBranch        *b_MET_RefFinal_em_ety;   //!
   TBranch        *b_MET_RefFinal_em_sumet;   //!
   TBranch        *b_MET_RefEle_em_etx;   //!
   TBranch        *b_MET_RefEle_em_ety;   //!
   TBranch        *b_MET_RefEle_em_sumet;   //!
   TBranch        *b_MET_RefJet_em_etx;   //!
   TBranch        *b_MET_RefJet_em_ety;   //!
   TBranch        *b_MET_RefJet_em_sumet;   //!
   TBranch        *b_MET_SoftJets_em_etx;   //!
   TBranch        *b_MET_SoftJets_em_ety;   //!
   TBranch        *b_MET_SoftJets_em_sumet;   //!
   TBranch        *b_MET_RefMuon_em_etx;   //!
   TBranch        *b_MET_RefMuon_em_ety;   //!
   TBranch        *b_MET_RefMuon_em_sumet;   //!
   TBranch        *b_MET_RefGamma_em_etx;   //!
   TBranch        *b_MET_RefGamma_em_ety;   //!
   TBranch        *b_MET_RefGamma_em_sumet;   //!
   TBranch        *b_MET_RefTau_em_etx;   //!
   TBranch        *b_MET_RefTau_em_ety;   //!
   TBranch        *b_MET_RefTau_em_sumet;   //!
   TBranch        *b_MET_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Muon_Isol_Muid_etx;   //!
   TBranch        *b_MET_Muon_Isol_Muid_ety;   //!
   TBranch        *b_MET_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_Muon_NonIsol_Muid_etx;   //!
   TBranch        *b_MET_Muon_NonIsol_Muid_ety;   //!
   TBranch        *b_MET_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_Muon_Total_Muid_etx;   //!
   TBranch        *b_MET_Muon_Total_Muid_ety;   //!
   TBranch        *b_MET_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_MuonBoy_etx;   //!
   TBranch        *b_MET_MuonBoy_ety;   //!
   TBranch        *b_MET_MuonBoy_sumet;   //!
   TBranch        *b_MET_MuonBoy_etx_CentralReg;   //!
   TBranch        *b_MET_MuonBoy_ety_CentralReg;   //!
   TBranch        *b_MET_MuonBoy_sumet_CentralReg;   //!
   TBranch        *b_MET_MuonBoy_phi_CentralReg;   //!
   TBranch        *b_MET_MuonBoy_etx_EndcapRegion;   //!
   TBranch        *b_MET_MuonBoy_ety_EndcapRegion;   //!
   TBranch        *b_MET_MuonBoy_sumet_EndcapRegion;   //!
   TBranch        *b_MET_MuonBoy_phi_EndcapRegion;   //!
   TBranch        *b_MET_MuonBoy_etx_ForwardReg;   //!
   TBranch        *b_MET_MuonBoy_ety_ForwardReg;   //!
   TBranch        *b_MET_MuonBoy_sumet_ForwardReg;   //!
   TBranch        *b_MET_MuonBoy_phi_ForwardReg;   //!
   TBranch        *b_MET_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_etx;   //!
   TBranch        *b_MET_RefMuon_Track_ety;   //!
   TBranch        *b_MET_RefMuon_Track_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_etx;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_ety;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_RefMuon_Track_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_etx;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_ety;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_sumet;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_RefMuon_Track_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Final_etx;   //!
   TBranch        *b_MET_Final_ety;   //!
   TBranch        *b_MET_Final_sumet;   //!
   TBranch        *b_MET_TopoObj_etx;   //!
   TBranch        *b_MET_TopoObj_ety;   //!
   TBranch        *b_MET_TopoObj_sumet;   //!
   TBranch        *b_MET_LocHadTopo_etx;   //!
   TBranch        *b_MET_LocHadTopo_ety;   //!
   TBranch        *b_MET_LocHadTopo_sumet;   //!
   TBranch        *b_MET_LocHadTopo_etx_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_ety_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_sumet_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_phi_CentralReg;   //!
   TBranch        *b_MET_LocHadTopo_etx_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_ety_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_phi_EndcapRegion;   //!
   TBranch        *b_MET_LocHadTopo_etx_ForwardReg;   //!
   TBranch        *b_MET_LocHadTopo_ety_ForwardReg;   //!
   TBranch        *b_MET_LocHadTopo_sumet_ForwardReg;   //!
   TBranch        *b_MET_LocHadTopo_phi_ForwardReg;   //!
   TBranch        *b_MET_Truth_NonInt_etx;   //!
   TBranch        *b_MET_Truth_NonInt_ety;   //!
   TBranch        *b_MET_Truth_NonInt_sumet;   //!
   TBranch        *b_MET_Truth_Int_etx;   //!
   TBranch        *b_MET_Truth_Int_ety;   //!
   TBranch        *b_MET_Truth_IntCentral_etx;   //!
   TBranch        *b_MET_Truth_IntCentral_ety;   //!
   TBranch        *b_MET_Truth_IntFwd_etx;   //!
   TBranch        *b_MET_Truth_IntFwd_ety;   //!
   TBranch        *b_MET_Truth_IntOutCover_etx;   //!
   TBranch        *b_MET_Truth_IntOutCover_ety;   //!
   TBranch        *b_MET_Truth_IntMuons_etx;   //!
   TBranch        *b_MET_Truth_IntMuons_ety;   //!
   TBranch        *b_MET_Truth_Int_sumet;   //!
   TBranch        *b_MET_Truth_IntCentral_sumet;   //!
   TBranch        *b_MET_Truth_IntFwd_sumet;   //!
   TBranch        *b_MET_Truth_IntOutCover_sumet;   //!
   TBranch        *b_MET_Truth_IntMuons_sumet;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefGamma_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefGamma_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefGamma_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_etx;   //!
   TBranch        *b_MET_Egamma20_RefGamma_ety;   //!
   TBranch        *b_MET_Egamma20_RefGamma_sumet;   //!
   TBranch        *b_MET_Egamma20_RefGamma_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefGamma_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefGamma_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefGamma_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefGamma_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefGamma_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefGamma_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefGamma_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_etx;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_ety;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefGamma_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_etx;   //!
   TBranch        *b_MET_Egamma20_RefFinal_ety;   //!
   TBranch        *b_MET_Egamma20_RefFinal_sumet;   //!
   TBranch        *b_MET_Egamma20_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_etx;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_ety;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefFinal_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefJet_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefJet_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefJet_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_etx;   //!
   TBranch        *b_MET_Egamma20_RefJet_ety;   //!
   TBranch        *b_MET_Egamma20_RefJet_sumet;   //!
   TBranch        *b_MET_Egamma20_RefJet_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefJet_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefJet_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefJet_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefJet_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefJet_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefJet_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefJet_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_etx;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_ety;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefJet_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_RefEle_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_RefEle_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_RefEle_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_etx;   //!
   TBranch        *b_MET_Egamma20_RefEle_ety;   //!
   TBranch        *b_MET_Egamma20_RefEle_sumet;   //!
   TBranch        *b_MET_Egamma20_RefEle_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefEle_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefEle_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefEle_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_RefEle_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_RefEle_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_RefEle_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_RefEle_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_etx;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_ety;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_RefEle_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_etx;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_ety;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_etx;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_ety;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_etx;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_ety;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Staco_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_etx;   //!
   TBranch        *b_MET_Egamma20_CellOut_ety;   //!
   TBranch        *b_MET_Egamma20_CellOut_sumet;   //!
   TBranch        *b_MET_Egamma20_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_etx;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_ety;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_CellOut_Eflow_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_CellOut_Eflow_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_CellOut_Eflow_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_CellOut_Eflow_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_CellOut_Eflow_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_etx;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_ety;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_CellOut_Eflow_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Isol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Isol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_etx;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_ety;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Isol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Isol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Isol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_etx;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_ety;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Isol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_NonIsol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_NonIsol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_etx;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_ety;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_NonIsol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_NonIsol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_NonIsol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_etx;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_ety;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_NonIsol_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_etx;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_ety;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Muon_Total_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Muon_Total_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_etx;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_ety;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Muon_Total_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Muon_Total_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Muon_Total_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_etx;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_ety;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Muon_Total_Muid_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Track_etx;   //!
   TBranch        *b_MET_Simplified20_Track_ety;   //!
   TBranch        *b_MET_Simplified20_Track_sumet;   //!
   TBranch        *b_MET_Simplified20_Track_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Track_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Track_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Track_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20_Track_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Track_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Track_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Track_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20_Track_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Track_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Track_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20_Track_phi_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_etx;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_ety;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_sumet;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_etx_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_ety_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_sumet_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_phi_CentralReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_etx_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_ety_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_phi_EndcapRegion;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_etx_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_ety_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_sumet_ForwardReg;   //!
   TBranch        *b_MET_Simplified20withTightPhotons_Track_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Track_etx;   //!
   TBranch        *b_MET_Egamma20_Track_ety;   //!
   TBranch        *b_MET_Egamma20_Track_sumet;   //!
   TBranch        *b_MET_Egamma20_Track_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Track_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Track_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Track_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20_Track_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Track_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Track_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Track_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20_Track_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Track_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Track_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20_Track_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_etx;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_ety;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_sumet;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma20NoTau_Track_phi_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_etx;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_ety;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_sumet;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_etx_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_ety_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_sumet_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_phi_CentralReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_etx_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_ety_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_sumet_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_phi_EndcapRegion;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_etx_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_ety_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_sumet_ForwardReg;   //!
   TBranch        *b_MET_Egamma10NoTau_Track_phi_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_etx;   //!
   TBranch        *b_MET_LArFixSimp20_Track_ety;   //!
   TBranch        *b_MET_LArFixSimp20_Track_sumet;   //!
   TBranch        *b_MET_LArFixSimp20_Track_etx_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_ety_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_sumet_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_phi_CentralReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_etx_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Track_ety_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Track_sumet_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Track_phi_EndcapRegion;   //!
   TBranch        *b_MET_LArFixSimp20_Track_etx_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_ety_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_sumet_ForwardReg;   //!
   TBranch        *b_MET_LArFixSimp20_Track_phi_ForwardReg;   //!
   TBranch        *b_el_MET_Simplified20_n;   //!
   TBranch        *b_el_MET_Simplified20_wpx;   //!
   TBranch        *b_el_MET_Simplified20_wpy;   //!
   TBranch        *b_el_MET_Simplified20_wet;   //!
   TBranch        *b_el_MET_Simplified20_statusWord;   //!
   TBranch        *b_mu_staco_MET_Simplified20_n;   //!
   TBranch        *b_mu_staco_MET_Simplified20_wpx;   //!
   TBranch        *b_mu_staco_MET_Simplified20_wpy;   //!
   TBranch        *b_mu_staco_MET_Simplified20_wet;   //!
   TBranch        *b_mu_staco_MET_Simplified20_statusWord;   //!
   TBranch        *b_mu_muid_MET_Simplified20_n;   //!
   TBranch        *b_mu_muid_MET_Simplified20_wpx;   //!
   TBranch        *b_mu_muid_MET_Simplified20_wpy;   //!
   TBranch        *b_mu_muid_MET_Simplified20_wet;   //!
   TBranch        *b_mu_muid_MET_Simplified20_statusWord;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_n;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_wpx;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_wpy;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_wet;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20_statusWord;   //!
   TBranch        *b_el_MET_Simplified20withTightPhotons_n;   //!
   TBranch        *b_el_MET_Simplified20withTightPhotons_wpx;   //!
   TBranch        *b_el_MET_Simplified20withTightPhotons_wpy;   //!
   TBranch        *b_el_MET_Simplified20withTightPhotons_wet;   //!
   TBranch        *b_el_MET_Simplified20withTightPhotons_statusWord;   //!
   TBranch        *b_ph_MET_Simplified20withTightPhotons_n;   //!
   TBranch        *b_ph_MET_Simplified20withTightPhotons_wpx;   //!
   TBranch        *b_ph_MET_Simplified20withTightPhotons_wpy;   //!
   TBranch        *b_ph_MET_Simplified20withTightPhotons_wet;   //!
   TBranch        *b_ph_MET_Simplified20withTightPhotons_statusWord;   //!
   TBranch        *b_mu_staco_MET_Simplified20withTightPhotons_n;   //!
   TBranch        *b_mu_staco_MET_Simplified20withTightPhotons_wpx;   //!
   TBranch        *b_mu_staco_MET_Simplified20withTightPhotons_wpy;   //!
   TBranch        *b_mu_staco_MET_Simplified20withTightPhotons_wet;   //!
   TBranch        *b_mu_staco_MET_Simplified20withTightPhotons_statusWord;   //!
   TBranch        *b_mu_muid_MET_Simplified20withTightPhotons_n;   //!
   TBranch        *b_mu_muid_MET_Simplified20withTightPhotons_wpx;   //!
   TBranch        *b_mu_muid_MET_Simplified20withTightPhotons_wpy;   //!
   TBranch        *b_mu_muid_MET_Simplified20withTightPhotons_wet;   //!
   TBranch        *b_mu_muid_MET_Simplified20withTightPhotons_statusWord;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_n;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_wpx;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_wpy;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_wet;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MET_Simplified20withTightPhotons_statusWord;   //!
   TBranch        *b_el_MET_Egamma20_n;   //!
   TBranch        *b_el_MET_Egamma20_wpx;   //!
   TBranch        *b_el_MET_Egamma20_wpy;   //!
   TBranch        *b_el_MET_Egamma20_wet;   //!
   TBranch        *b_el_MET_Egamma20_statusWord;   //!
   TBranch        *b_ph_MET_Egamma20_n;   //!
   TBranch        *b_ph_MET_Egamma20_wpx;   //!
   TBranch        *b_ph_MET_Egamma20_wpy;   //!
   TBranch        *b_ph_MET_Egamma20_wet;   //!
   TBranch        *b_ph_MET_Egamma20_statusWord;   //!
   TBranch        *b_mu_staco_MET_Egamma20_n;   //!
   TBranch        *b_mu_staco_MET_Egamma20_wpx;   //!
   TBranch        *b_mu_staco_MET_Egamma20_wpy;   //!
   TBranch        *b_mu_staco_MET_Egamma20_wet;   //!
   TBranch        *b_mu_staco_MET_Egamma20_statusWord;   //!
   TBranch        *b_mu_muid_MET_Egamma20_n;   //!
   TBranch        *b_mu_muid_MET_Egamma20_wpx;   //!
   TBranch        *b_mu_muid_MET_Egamma20_wpy;   //!
   TBranch        *b_mu_muid_MET_Egamma20_wet;   //!
   TBranch        *b_mu_muid_MET_Egamma20_statusWord;   //!
   TBranch        *b_tau_MET_Egamma20_n;   //!
   TBranch        *b_tau_MET_Egamma20_wpx;   //!
   TBranch        *b_tau_MET_Egamma20_wpy;   //!
   TBranch        *b_tau_MET_Egamma20_wet;   //!
   TBranch        *b_tau_MET_Egamma20_statusWord;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20_n;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20_wpx;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20_wpy;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20_wet;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20_statusWord;   //!
   TBranch        *b_el_MET_Egamma20NoTau_n;   //!
   TBranch        *b_el_MET_Egamma20NoTau_wpx;   //!
   TBranch        *b_el_MET_Egamma20NoTau_wpy;   //!
   TBranch        *b_el_MET_Egamma20NoTau_wet;   //!
   TBranch        *b_el_MET_Egamma20NoTau_statusWord;   //!
   TBranch        *b_ph_MET_Egamma20NoTau_n;   //!
   TBranch        *b_ph_MET_Egamma20NoTau_wpx;   //!
   TBranch        *b_ph_MET_Egamma20NoTau_wpy;   //!
   TBranch        *b_ph_MET_Egamma20NoTau_wet;   //!
   TBranch        *b_ph_MET_Egamma20NoTau_statusWord;   //!
   TBranch        *b_mu_staco_MET_Egamma20NoTau_n;   //!
   TBranch        *b_mu_staco_MET_Egamma20NoTau_wpx;   //!
   TBranch        *b_mu_staco_MET_Egamma20NoTau_wpy;   //!
   TBranch        *b_mu_staco_MET_Egamma20NoTau_wet;   //!
   TBranch        *b_mu_staco_MET_Egamma20NoTau_statusWord;   //!
   TBranch        *b_mu_muid_MET_Egamma20NoTau_n;   //!
   TBranch        *b_mu_muid_MET_Egamma20NoTau_wpx;   //!
   TBranch        *b_mu_muid_MET_Egamma20NoTau_wpy;   //!
   TBranch        *b_mu_muid_MET_Egamma20NoTau_wet;   //!
   TBranch        *b_mu_muid_MET_Egamma20NoTau_statusWord;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20NoTau_n;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20NoTau_wpx;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20NoTau_wpy;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20NoTau_wet;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma20NoTau_statusWord;   //!
   TBranch        *b_el_MET_Egamma10NoTau_n;   //!
   TBranch        *b_el_MET_Egamma10NoTau_wpx;   //!
   TBranch        *b_el_MET_Egamma10NoTau_wpy;   //!
   TBranch        *b_el_MET_Egamma10NoTau_wet;   //!
   TBranch        *b_el_MET_Egamma10NoTau_statusWord;   //!
   TBranch        *b_ph_MET_Egamma10NoTau_n;   //!
   TBranch        *b_ph_MET_Egamma10NoTau_wpx;   //!
   TBranch        *b_ph_MET_Egamma10NoTau_wpy;   //!
   TBranch        *b_ph_MET_Egamma10NoTau_wet;   //!
   TBranch        *b_ph_MET_Egamma10NoTau_statusWord;   //!
   TBranch        *b_mu_staco_MET_Egamma10NoTau_n;   //!
   TBranch        *b_mu_staco_MET_Egamma10NoTau_wpx;   //!
   TBranch        *b_mu_staco_MET_Egamma10NoTau_wpy;   //!
   TBranch        *b_mu_staco_MET_Egamma10NoTau_wet;   //!
   TBranch        *b_mu_staco_MET_Egamma10NoTau_statusWord;   //!
   TBranch        *b_mu_muid_MET_Egamma10NoTau_n;   //!
   TBranch        *b_mu_muid_MET_Egamma10NoTau_wpx;   //!
   TBranch        *b_mu_muid_MET_Egamma10NoTau_wpy;   //!
   TBranch        *b_mu_muid_MET_Egamma10NoTau_wet;   //!
   TBranch        *b_mu_muid_MET_Egamma10NoTau_statusWord;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma10NoTau_n;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpx;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma10NoTau_wpy;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma10NoTau_wet;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_Egamma10NoTau_statusWord;   //!
   TBranch        *b_el_MET_n;   //!
   TBranch        *b_el_MET_wpx;   //!
   TBranch        *b_el_MET_wpy;   //!
   TBranch        *b_el_MET_wet;   //!
   TBranch        *b_el_MET_statusWord;   //!
   TBranch        *b_ph_MET_n;   //!
   TBranch        *b_ph_MET_wpx;   //!
   TBranch        *b_ph_MET_wpy;   //!
   TBranch        *b_ph_MET_wet;   //!
   TBranch        *b_ph_MET_statusWord;   //!
   TBranch        *b_mu_staco_MET_n;   //!
   TBranch        *b_mu_staco_MET_wpx;   //!
   TBranch        *b_mu_staco_MET_wpy;   //!
   TBranch        *b_mu_staco_MET_wet;   //!
   TBranch        *b_mu_staco_MET_statusWord;   //!
   TBranch        *b_mu_muid_MET_n;   //!
   TBranch        *b_mu_muid_MET_wpx;   //!
   TBranch        *b_mu_muid_MET_wpy;   //!
   TBranch        *b_mu_muid_MET_wet;   //!
   TBranch        *b_mu_muid_MET_statusWord;   //!
   TBranch        *b_tau_MET_n;   //!
   TBranch        *b_tau_MET_wpx;   //!
   TBranch        *b_tau_MET_wpy;   //!
   TBranch        *b_tau_MET_wet;   //!
   TBranch        *b_tau_MET_statusWord;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_n;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_wpx;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_wpy;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_wet;   //!
   TBranch        *b_jet_AntiKt4LCTopo_MET_statusWord;   //!
   TBranch        *b_cl_MET_n;   //!
   TBranch        *b_cl_MET_wpx;   //!
   TBranch        *b_cl_MET_wpy;   //!
   TBranch        *b_cl_MET_wet;   //!
   TBranch        *b_cl_MET_statusWord;   //!
   TBranch        *b_trk_MET_n;   //!
   TBranch        *b_trk_MET_wpx;   //!
   TBranch        *b_trk_MET_wpy;   //!
   TBranch        *b_trk_MET_wet;   //!
   TBranch        *b_trk_MET_statusWord;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_n;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_E;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_pt;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_m;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_eta;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_phi;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EtaOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_PhiOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MOrigin;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EtaOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_PhiOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_MOriginEM;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_WIDTH;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_n90;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_Timing;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_LArQuality;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_nTrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_sumPtTrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_OriginIndex;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_HECQuality;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_NegativeE;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_AverageLArQF;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_YFlip12;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_YFlip23;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_CELL;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_DOTX;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_JET;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BCH_CORR_JET_FORCELL;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_ENG_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_N_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_N_BAD_CELLS_CORR;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_BAD_CELLS_CORR_E;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_NumTowers;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_SamplingMax;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_fracSamplingMax;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_hecf;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_tgap3f;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isUgly;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isBadLoose;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isBadMedium;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_isBadTight;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emfrac;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_Offset;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EMJES;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EMJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EMJESnooffset;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_GCWJES;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_GCWJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_CB;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_LCJES;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_E;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_pt;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_m;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_eta;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_emscale_phi;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtx_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtx_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtx_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_jvtxf;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_GSCFactorF;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_WidthFraction;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_PreSamplerB;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_EMB1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_EMB2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_EMB3;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_PreSamplerE;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_EME1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_EME2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_EME3;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_HEC0;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_HEC1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_HEC2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_HEC3;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileBar0;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileBar1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileBar2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileGap1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileGap2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileGap3;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileExt0;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileExt1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_TileExt2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_FCAL0;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_FCAL1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_e_FCAL2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_Nconst;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_ptconst_default;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_econst_default;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_etaconst_default;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_phiconst_default;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_weightconst_default;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_constscale_E;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_constscale_pt;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_constscale_m;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_constscale_eta;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_constscale_phi;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_Comb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_IP2D;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_IP3D;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SV0;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SV1;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SV2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_JetProb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_SoftMuonTag;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_JetFitterTagNN;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_JetFitterCOMBNN;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_weight_GbbNN;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_label;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToB;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToC;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_dRminToT;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_BHadronpdg;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_vx_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_vx_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_truth_vx_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip2d_pu;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip2d_pb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip2d_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip2d_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip3d_pu;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip3d_pb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip3d_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_ip3d_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv1_pu;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv1_pb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv1_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv2_pu;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv2_pb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv2_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_pu;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_pb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_pc;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pu;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pb;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_pc;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfitcomb_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_nvtx;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_nvtx1t;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_ntrkAtVx;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_efrc;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_mass;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_sig3d;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_deltaPhi;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_jfit_deltaEta;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_ntrkv;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_ntrkj;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_n2t;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_mass;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_efrc;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_err_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_err_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_err_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_cov_xy;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_cov_xz;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_cov_yz;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_chi2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_ndof;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_svp_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_isValid;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_ntrkv;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_ntrkj;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_n2t;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_mass;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_efrc;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_err_x;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_err_y;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_err_z;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_cov_xy;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_cov_xz;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_cov_yz;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_chi2;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_ndof;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_flavor_component_sv0p_ntrk;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_el_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_el_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_mu_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_mu_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L1_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L1_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L2_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_L2_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EF_dr;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_EF_matched;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_bbDecision;   //!
   TBranch        *b_jet_AntiKt4TopoNewEM_SmearingFactor;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_n;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_E;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_pt;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_m;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_eta;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_phi;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_el_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_el_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_mu_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_mu_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_L1_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_L1_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_L2_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_L2_matched;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_EF_dr;   //!
   TBranch        *b_jet_AntiKt4TopoEMLArFix_EF_matched;   //!
   TBranch        *b_jet_AntiKt4LCTopo_n;   //!
   TBranch        *b_jet_AntiKt4LCTopo_E;   //!
   TBranch        *b_jet_AntiKt4LCTopo_pt;   //!
   TBranch        *b_jet_AntiKt4LCTopo_m;   //!
   TBranch        *b_jet_AntiKt4LCTopo_eta;   //!
   TBranch        *b_jet_AntiKt4LCTopo_phi;   //!
   TBranch        *b_jet_AntiKt4LCTopo_el_dr;   //!
   TBranch        *b_jet_AntiKt4LCTopo_el_matched;   //!
   TBranch        *b_jet_AntiKt4LCTopo_mu_dr;   //!
   TBranch        *b_jet_AntiKt4LCTopo_mu_matched;   //!
   TBranch        *b_jet_AntiKt4LCTopo_L1_dr;   //!
   TBranch        *b_jet_AntiKt4LCTopo_L1_matched;   //!
   TBranch        *b_jet_AntiKt4LCTopo_L2_dr;   //!
   TBranch        *b_jet_AntiKt4LCTopo_L2_matched;   //!
   TBranch        *b_jet_AntiKt4LCTopo_EF_dr;   //!
   TBranch        *b_jet_AntiKt4LCTopo_EF_matched;   //!
   TBranch        *b_vx_n;   //!
   TBranch        *b_vx_x;   //!
   TBranch        *b_vx_y;   //!
   TBranch        *b_vx_z;   //!
   TBranch        *b_vx_px;   //!
   TBranch        *b_vx_py;   //!
   TBranch        *b_vx_pz;   //!
   TBranch        *b_vx_E;   //!
   TBranch        *b_vx_m;   //!
   TBranch        *b_vx_nTracks;   //!
   TBranch        *b_vx_sumPt;   //!
   TBranch        *b_trk_n;   //!
   TBranch        *b_trk_pt;   //!
   TBranch        *b_trk_eta;   //!
   TBranch        *b_trk_IPEstimate_d0_biased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_z0_biased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_err_d0_biased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_err_z0_biased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_errPV_d0_biased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_errPV_z0_biased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_d0_unbiased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_z0_unbiased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_err_d0_unbiased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_err_z0_unbiased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_errPV_d0_unbiased_wrtPV;   //!
   TBranch        *b_trk_IPEstimate_errPV_z0_unbiased_wrtPV;   //!
   TBranch        *b_trk_d0_wrtPV;   //!
   TBranch        *b_trk_z0_wrtPV;   //!
   TBranch        *b_trk_phi_wrtPV;   //!
   TBranch        *b_trk_theta_wrtPV;   //!
   TBranch        *b_trk_qoverp_wrtPV;   //!
   TBranch        *b_trk_cov_d0_wrtPV;   //!
   TBranch        *b_trk_cov_z0_wrtPV;   //!
   TBranch        *b_trk_cov_phi_wrtPV;   //!
   TBranch        *b_trk_cov_theta_wrtPV;   //!
   TBranch        *b_trk_cov_qoverp_wrtPV;   //!
   TBranch        *b_trk_chi2;   //!
   TBranch        *b_trk_ndof;   //!
   TBranch        *b_trk_nBLHits;   //!
   TBranch        *b_trk_nPixHits;   //!
   TBranch        *b_trk_nSCTHits;   //!
   TBranch        *b_trk_nTRTHits;   //!
   TBranch        *b_trk_nTRTHighTHits;   //!
   TBranch        *b_trk_nPixHoles;   //!
   TBranch        *b_trk_nSCTHoles;   //!
   TBranch        *b_trk_nTRTHoles;   //!
   TBranch        *b_trk_expectBLayerHit;   //!
   TBranch        *b_trk_nMDTHits;   //!
   TBranch        *b_trk_nCSCEtaHits;   //!
   TBranch        *b_trk_nCSCPhiHits;   //!
   TBranch        *b_trk_nRPCEtaHits;   //!
   TBranch        *b_trk_nRPCPhiHits;   //!
   TBranch        *b_trk_nTGCEtaHits;   //!
   TBranch        *b_trk_nTGCPhiHits;   //!
   TBranch        *b_trk_nHits;   //!
   TBranch        *b_trk_nHoles;   //!
   TBranch        *b_trk_hitPattern;   //!
   TBranch        *b_trk_TRTHighTHitsRatio;   //!
   TBranch        *b_trk_TRTHighTOutliersRatio;   //!
   TBranch        *b_trk_mc_probability;   //!
   TBranch        *b_trk_mc_barcode;   //!
   TBranch        *b_trk_cone20_trackIso;   //!
   TBranch        *b_trk_cone20_caloIso;   //!
   TBranch        *b_trk_cone20_nTrackIso;   //!
   TBranch        *b_trk_cone30_trackIso;   //!
   TBranch        *b_trk_cone30_caloIso;   //!
   TBranch        *b_trk_cone30_nTrackIso;   //!
   TBranch        *b_trk_cone40_trackIso;   //!
   TBranch        *b_trk_cone40_caloIso;   //!
   TBranch        *b_trk_cone40_nTrackIso;   //!
   TBranch        *b_top_hfor_type;   //!
   TBranch        *b_mcevt_n;   //!
   TBranch        *b_mcevt_signal_process_id;   //!
   TBranch        *b_mcevt_event_number;   //!
   TBranch        *b_mcevt_event_scale;   //!
   TBranch        *b_mcevt_alphaQCD;   //!
   TBranch        *b_mcevt_alphaQED;   //!
   TBranch        *b_mcevt_pdf_id1;   //!
   TBranch        *b_mcevt_pdf_id2;   //!
   TBranch        *b_mcevt_pdf_x1;   //!
   TBranch        *b_mcevt_pdf_x2;   //!
   TBranch        *b_mcevt_pdf_scale;   //!
   TBranch        *b_mcevt_pdf1;   //!
   TBranch        *b_mcevt_pdf2;   //!
   TBranch        *b_mcevt_weight;   //!
   TBranch        *b_mcevt_nparticle;   //!
   TBranch        *b_mcevt_pileUpType;   //!
   TBranch        *b_mc_n;   //!
   TBranch        *b_mc_pt;   //!
   TBranch        *b_mc_m;   //!
   TBranch        *b_mc_eta;   //!
   TBranch        *b_mc_phi;   //!
   TBranch        *b_mc_status;   //!
   TBranch        *b_mc_barcode;   //!
   TBranch        *b_mc_parents;   //!
   TBranch        *b_mc_children;   //!
   TBranch        *b_mc_pdgId;   //!
   TBranch        *b_mc_charge;   //!
   TBranch        *b_mc_vx_x;   //!
   TBranch        *b_mc_vx_y;   //!
   TBranch        *b_mc_vx_z;   //!
   TBranch        *b_mc_vx_barcode;   //!
   TBranch        *b_mc_child_index;   //!
   TBranch        *b_mc_parent_index;   //!
   TBranch        *b_mcVx_n;   //!
   TBranch        *b_mcVx_x;   //!
   TBranch        *b_mcVx_y;   //!
   TBranch        *b_mcVx_z;   //!
   TBranch        *b_muonTruth_n;   //!
   TBranch        *b_muonTruth_pt;   //!
   TBranch        *b_muonTruth_m;   //!
   TBranch        *b_muonTruth_eta;   //!
   TBranch        *b_muonTruth_phi;   //!
   TBranch        *b_muonTruth_charge;   //!
   TBranch        *b_muonTruth_PDGID;   //!
   TBranch        *b_muonTruth_barcode;   //!
   TBranch        *b_muonTruth_type;   //!
   TBranch        *b_muonTruth_origin;   //!
   TBranch        *b_jet_AntiKt4TruthJets_n;   //!
   TBranch        *b_jet_AntiKt4TruthJets_E;   //!
   TBranch        *b_jet_AntiKt4TruthJets_pt;   //!
   TBranch        *b_jet_AntiKt4TruthJets_m;   //!
   TBranch        *b_jet_AntiKt4TruthJets_eta;   //!
   TBranch        *b_jet_AntiKt4TruthJets_phi;   //!
   TBranch        *b_jet_AntiKt4TruthJets_EtaOrigin;   //!
   TBranch        *b_jet_AntiKt4TruthJets_PhiOrigin;   //!
   TBranch        *b_jet_AntiKt4TruthJets_MOrigin;   //!
   TBranch        *b_jet_AntiKt4TruthJets_EtaOriginEM;   //!
   TBranch        *b_jet_AntiKt4TruthJets_PhiOriginEM;   //!
   TBranch        *b_jet_AntiKt4TruthJets_MOriginEM;   //!
   TBranch        *b_jet_AntiKt4TruthJets_WIDTH;   //!
   TBranch        *b_jet_AntiKt4TruthJets_n90;   //!
   TBranch        *b_jet_AntiKt4TruthJets_Timing;   //!
   TBranch        *b_jet_AntiKt4TruthJets_LArQuality;   //!
   TBranch        *b_jet_AntiKt4TruthJets_nTrk;   //!
   TBranch        *b_jet_AntiKt4TruthJets_sumPtTrk;   //!
   TBranch        *b_jet_AntiKt4TruthJets_OriginIndex;   //!
   TBranch        *b_jet_AntiKt4TruthJets_HECQuality;   //!
   TBranch        *b_jet_AntiKt4TruthJets_NegativeE;   //!
   TBranch        *b_jet_AntiKt4TruthJets_AverageLArQF;   //!
   TBranch        *b_jet_AntiKt4TruthJets_YFlip12;   //!
   TBranch        *b_jet_AntiKt4TruthJets_YFlip23;   //!
   TBranch        *b_jet_AntiKt4TruthJets_BCH_CORR_CELL;   //!
   TBranch        *b_jet_AntiKt4TruthJets_BCH_CORR_DOTX;   //!
   TBranch        *b_jet_AntiKt4TruthJets_BCH_CORR_JET;   //!
   TBranch        *b_jet_AntiKt4TruthJets_BCH_CORR_JET_FORCELL;   //!
   TBranch        *b_jet_AntiKt4TruthJets_ENG_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TruthJets_N_BAD_CELLS;   //!
   TBranch        *b_jet_AntiKt4TruthJets_N_BAD_CELLS_CORR;   //!
   TBranch        *b_jet_AntiKt4TruthJets_BAD_CELLS_CORR_E;   //!
   TBranch        *b_jet_AntiKt4TruthJets_NumTowers;   //!
   TBranch        *b_jet_AntiKt4TruthJets_SamplingMax;   //!
   TBranch        *b_jet_AntiKt4TruthJets_fracSamplingMax;   //!
   TBranch        *b_jet_AntiKt4TruthJets_hecf;   //!
   TBranch        *b_jet_AntiKt4TruthJets_tgap3f;   //!
   TBranch        *b_jet_AntiKt4TruthJets_isUgly;   //!
   TBranch        *b_jet_AntiKt4TruthJets_isBadLoose;   //!
   TBranch        *b_jet_AntiKt4TruthJets_isBadMedium;   //!
   TBranch        *b_jet_AntiKt4TruthJets_isBadTight;   //!
   TBranch        *b_jet_AntiKt4TruthJets_emfrac;   //!
   TBranch        *b_jet_AntiKt4TruthJets_Offset;   //!
   TBranch        *b_jet_AntiKt4TruthJets_EMJES;   //!
   TBranch        *b_jet_AntiKt4TruthJets_EMJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TruthJets_EMJESnooffset;   //!
   TBranch        *b_jet_AntiKt4TruthJets_GCWJES;   //!
   TBranch        *b_jet_AntiKt4TruthJets_GCWJES_EtaCorr;   //!
   TBranch        *b_jet_AntiKt4TruthJets_CB;   //!
   TBranch        *b_jet_AntiKt4TruthJets_LCJES;   //!
   TBranch        *b_jet_AntiKt4TruthJets_emscale_E;   //!
   TBranch        *b_jet_AntiKt4TruthJets_emscale_pt;   //!
   TBranch        *b_jet_AntiKt4TruthJets_emscale_m;   //!
   TBranch        *b_jet_AntiKt4TruthJets_emscale_eta;   //!
   TBranch        *b_jet_AntiKt4TruthJets_emscale_phi;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_label;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_dRminToB;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_dRminToC;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_dRminToT;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_BHadronpdg;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_vx_x;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_vx_y;   //!
   TBranch        *b_jet_AntiKt4TruthJets_flavor_truth_vx_z;   //!
   TBranch        *b_jet_AntiKt4TruthJets_el_dr;   //!
   TBranch        *b_jet_AntiKt4TruthJets_el_matched;   //!
   TBranch        *b_jet_AntiKt4TruthJets_mu_dr;   //!
   TBranch        *b_jet_AntiKt4TruthJets_mu_matched;   //!
   TBranch        *b_jet_AntiKt4TruthJets_L1_dr;   //!
   TBranch        *b_jet_AntiKt4TruthJets_L1_matched;   //!
   TBranch        *b_jet_AntiKt4TruthJets_L2_dr;   //!
   TBranch        *b_jet_AntiKt4TruthJets_L2_matched;   //!
   TBranch        *b_jet_AntiKt4TruthJets_EF_dr;   //!
   TBranch        *b_jet_AntiKt4TruthJets_EF_matched;   //!
   TBranch        *b_trig_L1_emtau_n;   //!
   TBranch        *b_trig_L1_emtau_eta;   //!
   TBranch        *b_trig_L1_emtau_phi;   //!
   TBranch        *b_trig_L1_emtau_thrNames;   //!
   TBranch        *b_trig_L1_emtau_thrValues;   //!
   TBranch        *b_trig_L1_emtau_core;   //!
   TBranch        *b_trig_L1_emtau_EMClus;   //!
   TBranch        *b_trig_L1_emtau_tauClus;   //!
   TBranch        *b_trig_L1_emtau_EMIsol;   //!
   TBranch        *b_trig_L1_emtau_hadIsol;   //!
   TBranch        *b_trig_L1_emtau_hadCore;   //!
   TBranch        *b_trig_L1_emtau_thrPattern;   //!
   TBranch        *b_trig_L2_emcl_n;   //!
   TBranch        *b_trig_L2_emcl_quality;   //!
   TBranch        *b_trig_L2_emcl_E;   //!
   TBranch        *b_trig_L2_emcl_Et;   //!
   TBranch        *b_trig_L2_emcl_eta;   //!
   TBranch        *b_trig_L2_emcl_phi;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_n;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_algorithmId;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_trackStatus;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_chi2Ndof;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_param_a0;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_param_z0;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_param_phi0;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_param_eta;   //!
   TBranch        *b_trig_L2_trk_idscan_eGamma_param_pt;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_n;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_algorithmId;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_trackStatus;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_chi2Ndof;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_param_a0;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_param_z0;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_param_phi0;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_param_eta;   //!
   TBranch        *b_trig_L2_trk_sitrack_eGamma_param_pt;   //!
   TBranch        *b_trig_L2_el_n;   //!
   TBranch        *b_trig_L2_el_E;   //!
   TBranch        *b_trig_L2_el_Et;   //!
   TBranch        *b_trig_L2_el_pt;   //!
   TBranch        *b_trig_L2_el_eta;   //!
   TBranch        *b_trig_L2_el_phi;   //!
   TBranch        *b_trig_L2_el_RoIWord;   //!
   TBranch        *b_trig_L2_el_zvertex;   //!
   TBranch        *b_trig_L2_el_charge;   //!
   TBranch        *b_trig_L2_ph_n;   //!
   TBranch        *b_trig_L2_ph_E;   //!
   TBranch        *b_trig_L2_ph_Et;   //!
   TBranch        *b_trig_L2_ph_pt;   //!
   TBranch        *b_trig_L2_ph_eta;   //!
   TBranch        *b_trig_L2_ph_phi;   //!
   TBranch        *b_trig_L2_ph_RoIWord;   //!
   TBranch        *b_trig_EF_emcl_n;   //!
   TBranch        *b_trig_EF_emcl_pt;   //!
   TBranch        *b_trig_EF_emcl_eta;   //!
   TBranch        *b_trig_EF_emcl_phi;   //!
   TBranch        *b_trig_EF_emcl_E_em;   //!
   TBranch        *b_trig_EF_emcl_E_had;   //!
   TBranch        *b_trig_EF_emcl_slw_n;   //!
   TBranch        *b_trig_EF_emcl_slw_pt;   //!
   TBranch        *b_trig_EF_emcl_slw_eta;   //!
   TBranch        *b_trig_EF_emcl_slw_phi;   //!
   TBranch        *b_trig_EF_emcl_slw_E_em;   //!
   TBranch        *b_trig_EF_emcl_slw_E_had;   //!
   TBranch        *b_trig_EF_el_n;   //!
   TBranch        *b_trig_EF_el_E;   //!
   TBranch        *b_trig_EF_el_Et;   //!
   TBranch        *b_trig_EF_el_pt;   //!
   TBranch        *b_trig_EF_el_m;   //!
   TBranch        *b_trig_EF_el_eta;   //!
   TBranch        *b_trig_EF_el_phi;   //!
   TBranch        *b_trig_EF_el_px;   //!
   TBranch        *b_trig_EF_el_py;   //!
   TBranch        *b_trig_EF_el_pz;   //!
   TBranch        *b_trig_EF_el_charge;   //!
   TBranch        *b_trig_EF_el_author;   //!
   TBranch        *b_trig_EF_el_isEM;   //!
   TBranch        *b_trig_EF_el_isEMLoose;   //!
   TBranch        *b_trig_EF_el_isEMMedium;   //!
   TBranch        *b_trig_EF_el_isEMTight;   //!
   TBranch        *b_trig_EF_el_loose;   //!
   TBranch        *b_trig_EF_el_looseIso;   //!
   TBranch        *b_trig_EF_el_medium;   //!
   TBranch        *b_trig_EF_el_mediumIso;   //!
   TBranch        *b_trig_EF_el_mediumWithoutTrack;   //!
   TBranch        *b_trig_EF_el_mediumIsoWithoutTrack;   //!
   TBranch        *b_trig_EF_el_tight;   //!
   TBranch        *b_trig_EF_el_tightIso;   //!
   TBranch        *b_trig_EF_el_tightWithoutTrack;   //!
   TBranch        *b_trig_EF_el_tightIsoWithoutTrack;   //!
   TBranch        *b_trig_EF_el_loosePP;   //!
   TBranch        *b_trig_EF_el_loosePPIso;   //!
   TBranch        *b_trig_EF_el_mediumPP;   //!
   TBranch        *b_trig_EF_el_mediumPPIso;   //!
   TBranch        *b_trig_EF_el_tightPP;   //!
   TBranch        *b_trig_EF_el_tightPPIso;   //!
   TBranch        *b_trig_EF_el_vertweight;   //!
   TBranch        *b_trig_EF_el_hastrack;   //!
   TBranch        *b_trig_EF_ph_n;   //!
   TBranch        *b_trig_EF_ph_E;   //!
   TBranch        *b_trig_EF_ph_Et;   //!
   TBranch        *b_trig_EF_ph_pt;   //!
   TBranch        *b_trig_EF_ph_m;   //!
   TBranch        *b_trig_EF_ph_eta;   //!
   TBranch        *b_trig_EF_ph_phi;   //!
   TBranch        *b_trig_EF_ph_px;   //!
   TBranch        *b_trig_EF_ph_py;   //!
   TBranch        *b_trig_EF_ph_pz;   //!
   TBranch        *b_trig_EF_ph_author;   //!
   TBranch        *b_trig_EF_ph_isRecovered;   //!
   TBranch        *b_trig_EF_ph_isEM;   //!
   TBranch        *b_trig_EF_ph_isEMLoose;   //!
   TBranch        *b_trig_EF_ph_isEMMedium;   //!
   TBranch        *b_trig_EF_ph_isEMTight;   //!
   TBranch        *b_trig_EF_ph_convFlag;   //!
   TBranch        *b_trig_EF_ph_isConv;   //!
   TBranch        *b_trig_EF_ph_nConv;   //!
   TBranch        *b_trig_EF_ph_nSingleTrackConv;   //!
   TBranch        *b_trig_EF_ph_nDoubleTrackConv;   //!
   TBranch        *b_trig_EF_ph_loose;   //!
   TBranch        *b_trig_EF_ph_looseIso;   //!
   TBranch        *b_trig_EF_ph_tight;   //!
   TBranch        *b_trig_EF_ph_tightIso;   //!
   TBranch        *b_trig_EF_ph_looseAR;   //!
   TBranch        *b_trig_EF_ph_looseARIso;   //!
   TBranch        *b_trig_EF_ph_tightAR;   //!
   TBranch        *b_trig_EF_ph_tightARIso;   //!
   TBranch        *b_trig_Nav_n;   //!
   TBranch        *b_trig_Nav_chain_ChainId;   //!
   TBranch        *b_trig_Nav_chain_RoIType;   //!
   TBranch        *b_trig_Nav_chain_RoIIndex;   //!
   TBranch        *b_trig_RoI_L2_e_n;   //!
   TBranch        *b_trig_RoI_EF_e_n;   //!
   TBranch        *b_trig_DB_SMK;   //!
   TBranch        *b_trig_DB_L1PSK;   //!
   TBranch        *b_trig_DB_HLTPSK;   //!
   TBranch        *b_trig_L1_TAV;   //!
   TBranch        *b_trig_L2_passedPhysics;   //!
   TBranch        *b_trig_EF_passedPhysics;   //!
   TBranch        *b_trig_L1_TBP;   //!
   TBranch        *b_trig_L1_TAP;   //!
   TBranch        *b_trig_L2_passedRaw;   //!
   TBranch        *b_trig_EF_passedRaw;   //!
   TBranch        *b_trig_L2_truncated;   //!
   TBranch        *b_trig_EF_truncated;   //!
   TBranch        *b_trig_L2_resurrected;   //!
   TBranch        *b_trig_EF_resurrected;   //!
   TBranch        *b_trig_L2_passedThrough;   //!
   TBranch        *b_trig_EF_passedThrough;   //!
   TBranch        *b_trig_L2_wasPrescaled;   //!
   TBranch        *b_trig_L2_wasResurrected;   //!
   TBranch        *b_trig_EF_wasPrescaled;   //!
   TBranch        *b_trig_EF_wasResurrected;   //!
   TBranch        *b_trig_EF_trigmuonef_n;   //!
   TBranch        *b_trig_EF_trigmuonef_track_n;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MuonType;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_pt;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_eta;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_phi;   //!
   TBranch        *b_trig_EF_trigmuonef_track_MS_hasMS;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_pt;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_eta;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_phi;   //!
   TBranch        *b_trig_EF_trigmuonef_track_SA_hasSA;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_pt;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_eta;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_phi;   //!
   TBranch        *b_trig_EF_trigmuonef_track_CB_hasCB;   //!
   TBranch        *b_trig_EF_trigmugirl_n;   //!
   TBranch        *b_trig_EF_trigmugirl_track_n;   //!
   TBranch        *b_trig_EF_trigmugirl_track_MuonType;   //!
   TBranch        *b_trig_EF_trigmugirl_track_MS_pt;   //!
   TBranch        *b_trig_EF_trigmugirl_track_MS_eta;   //!
   TBranch        *b_trig_EF_trigmugirl_track_MS_phi;   //!
   TBranch        *b_trig_EF_trigmugirl_track_MS_hasMS;   //!
   TBranch        *b_trig_EF_trigmugirl_track_SA_pt;   //!
   TBranch        *b_trig_EF_trigmugirl_track_SA_eta;   //!
   TBranch        *b_trig_EF_trigmugirl_track_SA_phi;   //!
   TBranch        *b_trig_EF_trigmugirl_track_SA_hasSA;   //!
   TBranch        *b_trig_EF_trigmugirl_track_CB_pt;   //!
   TBranch        *b_trig_EF_trigmugirl_track_CB_eta;   //!
   TBranch        *b_trig_EF_trigmugirl_track_CB_phi;   //!
   TBranch        *b_trig_EF_trigmugirl_track_CB_hasCB;   //!
   TBranch        *b_trig_L1_mu_n;   //!
   TBranch        *b_trig_L1_mu_pt;   //!
   TBranch        *b_trig_L1_mu_eta;   //!
   TBranch        *b_trig_L1_mu_phi;   //!
   TBranch        *b_trig_L1_mu_thrName;   //!
   TBranch        *b_trig_L2_muonfeature_n;   //!
   TBranch        *b_trig_L2_muonfeature_pt;   //!
   TBranch        *b_trig_L2_muonfeature_eta;   //!
   TBranch        *b_trig_L2_muonfeature_phi;   //!
   TBranch        *b_trig_L2_muonfeaturedetails_n;   //!
   TBranch        *b_trig_L2_combmuonfeature_n;   //!
   TBranch        *b_trig_L2_combmuonfeature_pt;   //!
   TBranch        *b_trig_L2_combmuonfeature_eta;   //!
   TBranch        *b_trig_L2_combmuonfeature_phi;   //!
   TBranch        *b_trig_L2_combmuonfeature_idscantrk_index;   //!
   TBranch        *b_trig_L2_combmuonfeature_sitracktrk_index;   //!
   TBranch        *b_trig_L2_isomuonfeature_n;   //!
   TBranch        *b_trig_L2_isomuonfeature_pt;   //!
   TBranch        *b_trig_L2_isomuonfeature_eta;   //!
   TBranch        *b_trig_L2_isomuonfeature_phi;   //!
   TBranch        *b_trig_L2_isomuonfeature_EtInnerConeEC;   //!
   TBranch        *b_trig_L2_isomuonfeature_EtOuterConeEC;   //!
   TBranch        *b_trig_L2_isomuonfeature_EtInnerConeHC;   //!
   TBranch        *b_trig_L2_isomuonfeature_EtOuterConeHC;   //!
   TBranch        *b_trig_L2_isomuonfeature_NTracksCone;   //!
   TBranch        *b_trig_L2_isomuonfeature_SumPtTracksCone;   //!
   TBranch        *b_trig_L2_isomuonfeature_PtMuTracksCone;   //!
   TBranch        *b_trig_L2_tilemufeature_n;   //!
   TBranch        *b_trig_L2_tilemufeature_eta;   //!
   TBranch        *b_trig_L2_tilemufeature_phi;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_n;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_pt;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_eta;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_phi;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_tilemu_index;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_algorithmId;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_trackStatus;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_chi2Ndof;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_a0;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_z0;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_phi0;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_eta;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_pt;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_idtrkfitpar_hasidtrkfitpar;   //!
   TBranch        *b_trig_L2_tiletrackmufeature_idtrk_hasidtrk;   //!
   TBranch        *b_trig_L2_sitrack_muon_n;   //!
   TBranch        *b_trig_L2_sitrack_muon_algorithmId;   //!
   TBranch        *b_trig_L2_sitrack_muon_trackStatus;   //!
   TBranch        *b_trig_L2_sitrack_muon_chi2Ndof;   //!
   TBranch        *b_trig_L2_sitrack_muon_param_a0;   //!
   TBranch        *b_trig_L2_sitrack_muon_param_z0;   //!
   TBranch        *b_trig_L2_sitrack_muon_param_phi0;   //!
   TBranch        *b_trig_L2_sitrack_muon_param_eta;   //!
   TBranch        *b_trig_L2_sitrack_muon_param_pt;   //!
   TBranch        *b_trig_L2_idscan_muon_n;   //!
   TBranch        *b_trig_L2_idscan_muon_algorithmId;   //!
   TBranch        *b_trig_L2_idscan_muon_trackStatus;   //!
   TBranch        *b_trig_L2_idscan_muon_chi2Ndof;   //!
   TBranch        *b_trig_L2_idscan_muon_param_a0;   //!
   TBranch        *b_trig_L2_idscan_muon_param_z0;   //!
   TBranch        *b_trig_L2_idscan_muon_param_phi0;   //!
   TBranch        *b_trig_L2_idscan_muon_param_eta;   //!
   TBranch        *b_trig_L2_idscan_muon_param_pt;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_n;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_algorithmId;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_trackStatus;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_chi2Ndof;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_param_a0;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_param_z0;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_param_phi0;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_param_eta;   //!
   TBranch        *b_trig_L2_sitrack_isomuon_param_pt;   //!
   TBranch        *b_trig_L2_idscan_isomuon_n;   //!
   TBranch        *b_trig_L2_idscan_isomuon_algorithmId;   //!
   TBranch        *b_trig_L2_idscan_isomuon_trackStatus;   //!
   TBranch        *b_trig_L2_idscan_isomuon_chi2Ndof;   //!
   TBranch        *b_trig_L2_idscan_isomuon_param_a0;   //!
   TBranch        *b_trig_L2_idscan_isomuon_param_z0;   //!
   TBranch        *b_trig_L2_idscan_isomuon_param_phi0;   //!
   TBranch        *b_trig_L2_idscan_isomuon_param_eta;   //!
   TBranch        *b_trig_L2_idscan_isomuon_param_pt;   //!
   TBranch        *b_trig_roidescriptor_forID_n;   //!
   TBranch        *b_trig_roidescriptor_forID_phi;   //!
   TBranch        *b_trig_roidescriptor_forID_eta;   //!
   TBranch        *b_trig_roidescriptor_forID_zed0;   //!
   TBranch        *b_trig_L1_jet_n;   //!
   TBranch        *b_trig_L1_jet_eta;   //!
   TBranch        *b_trig_L1_jet_phi;   //!
   TBranch        *b_trig_L1_jet_thrNames;   //!
   TBranch        *b_trig_L1_jet_thrValues;   //!
   TBranch        *b_trig_L1_jet_thrPattern;   //!
   TBranch        *b_trig_L1_jet_et4x4;   //!
   TBranch        *b_trig_L1_jet_et6x6;   //!
   TBranch        *b_trig_L1_jet_et8x8;   //!
   TBranch        *b_trig_L1_jet_RoIWord;   //!
   TBranch        *b_trig_L2_jet_n;   //!
   TBranch        *b_trig_L2_jet_E;   //!
   TBranch        *b_trig_L2_jet_eta;   //!
   TBranch        *b_trig_L2_jet_phi;   //!
   TBranch        *b_trig_L2_jet_RoIWord;   //!
   TBranch        *b_trig_L2_jet_ehad0;   //!
   TBranch        *b_trig_L2_jet_eem0;   //!
   TBranch        *b_trig_L2_jet_nLeadingCells;   //!
   TBranch        *b_trig_L2_jet_hecf;   //!
   TBranch        *b_trig_L2_jet_jetQuality;   //!
   TBranch        *b_trig_L2_jet_emf;   //!
   TBranch        *b_trig_L2_jet_jetTimeCells;   //!
   TBranch        *b_trig_EF_jet_n;   //!
   TBranch        *b_trig_EF_jet_emscale_E;   //!
   TBranch        *b_trig_EF_jet_emscale_pt;   //!
   TBranch        *b_trig_EF_jet_emscale_m;   //!
   TBranch        *b_trig_EF_jet_emscale_eta;   //!
   TBranch        *b_trig_EF_jet_emscale_phi;   //!
   TBranch        *b_trig_EF_jet_a4;   //!
   TBranch        *b_trig_EF_jet_a4tc;   //!
   TBranch        *b_trig_EF_jet_a10tc;   //!
   TBranch        *b_trig_EF_jet_a6;   //!
   TBranch        *b_trig_EF_jet_a6tc;   //!
   TBranch        *b_trig_EF_jet_RoIword;   //!
   TBranch        *b_trig_RoI_L2_b_n;   //!
   TBranch        *b_trig_RoI_L2_b_type;   //!
   TBranch        *b_trig_RoI_L2_b_active;   //!
   TBranch        *b_trig_RoI_L2_b_lastStep;   //!
   TBranch        *b_trig_RoI_L2_b_TENumber;   //!
   TBranch        *b_trig_RoI_L2_b_roiNumber;   //!
   TBranch        *b_trig_RoI_L2_b_Jet_ROI;   //!
   TBranch        *b_trig_RoI_L2_b_Jet_ROIStatus;   //!
   TBranch        *b_trig_RoI_L2_b_Muon_ROI;   //!
   TBranch        *b_trig_RoI_L2_b_Muon_ROIStatus;   //!
   TBranch        *b_trig_RoI_L2_b_TrigL2BjetContainer;   //!
   TBranch        *b_trig_RoI_L2_b_TrigL2BjetContainerStatus;   //!
   TBranch        *b_trig_RoI_L2_b_TrigInDetTrackCollection_TrigSiTrack_Jet;   //!
   TBranch        *b_trig_RoI_L2_b_TrigInDetTrackCollection_TrigSiTrack_JetStatus;   //!
   TBranch        *b_trig_RoI_L2_b_TrigInDetTrackCollection_TrigIDSCAN_Jet;   //!
   TBranch        *b_trig_RoI_L2_b_TrigInDetTrackCollection_TrigIDSCAN_JetStatus;   //!
   TBranch        *b_trig_RoI_EF_b_n;   //!
   TBranch        *b_trig_RoI_EF_b_type;   //!
   TBranch        *b_trig_RoI_EF_b_active;   //!
   TBranch        *b_trig_RoI_EF_b_lastStep;   //!
   TBranch        *b_trig_RoI_EF_b_TENumber;   //!
   TBranch        *b_trig_RoI_EF_b_roiNumber;   //!
   TBranch        *b_trig_RoI_EF_b_Jet_ROI;   //!
   TBranch        *b_trig_RoI_EF_b_Jet_ROIStatus;   //!
   TBranch        *b_trig_RoI_EF_b_Muon_ROI;   //!
   TBranch        *b_trig_RoI_EF_b_Muon_ROIStatus;   //!
   TBranch        *b_trig_RoI_EF_b_TrigEFBjetContainer;   //!
   TBranch        *b_trig_RoI_EF_b_TrigEFBjetContainerStatus;   //!
   TBranch        *b_trig_RoI_EF_b_Rec__TrackParticleContainer;   //!
   TBranch        *b_trig_RoI_EF_b_Rec__TrackParticleContainerStatus;   //!
   TBranch        *b_trig_L2_bjet_n;   //!
   TBranch        *b_trig_L2_bjet_roiId;   //!
   TBranch        *b_trig_L2_bjet_valid;   //!
   TBranch        *b_trig_L2_bjet_prmVtx;   //!
   TBranch        *b_trig_L2_bjet_pt;   //!
   TBranch        *b_trig_L2_bjet_eta;   //!
   TBranch        *b_trig_L2_bjet_phi;   //!
   TBranch        *b_trig_L2_bjet_xComb;   //!
   TBranch        *b_trig_L2_bjet_xIP1D;   //!
   TBranch        *b_trig_L2_bjet_xIP2D;   //!
   TBranch        *b_trig_L2_bjet_xIP3D;   //!
   TBranch        *b_trig_L2_bjet_xCHI2;   //!
   TBranch        *b_trig_L2_bjet_xSV;   //!
   TBranch        *b_trig_L2_bjet_xMVtx;   //!
   TBranch        *b_trig_L2_bjet_xEVtx;   //!
   TBranch        *b_trig_L2_bjet_xNVtx;   //!
   TBranch        *b_trig_L2_bjet_BSx;   //!
   TBranch        *b_trig_L2_bjet_BSy;   //!
   TBranch        *b_trig_L2_bjet_BSz;   //!
   TBranch        *b_trig_L2_bjet_sBSx;   //!
   TBranch        *b_trig_L2_bjet_sBSy;   //!
   TBranch        *b_trig_L2_bjet_sBSz;   //!
   TBranch        *b_trig_L2_bjet_sBSxy;   //!
   TBranch        *b_trig_L2_bjet_BTiltXZ;   //!
   TBranch        *b_trig_L2_bjet_BTiltYZ;   //!
   TBranch        *b_trig_L2_bjet_BSstatus;   //!
   TBranch        *b_trig_EF_bjet_n;   //!
   TBranch        *b_trig_EF_bjet_roiId;   //!
   TBranch        *b_trig_EF_bjet_valid;   //!
   TBranch        *b_trig_EF_bjet_prmVtx;   //!
   TBranch        *b_trig_EF_bjet_pt;   //!
   TBranch        *b_trig_EF_bjet_eta;   //!
   TBranch        *b_trig_EF_bjet_phi;   //!
   TBranch        *b_trig_EF_bjet_xComb;   //!
   TBranch        *b_trig_EF_bjet_xIP1D;   //!
   TBranch        *b_trig_EF_bjet_xIP2D;   //!
   TBranch        *b_trig_EF_bjet_xIP3D;   //!
   TBranch        *b_trig_EF_bjet_xCHI2;   //!
   TBranch        *b_trig_EF_bjet_xSV;   //!
   TBranch        *b_trig_EF_bjet_xMVtx;   //!
   TBranch        *b_trig_EF_bjet_xEVtx;   //!
   TBranch        *b_trig_EF_bjet_xNVtx;   //!
   TBranch        *b_trig_EF_pv_n;   //!
   TBranch        *b_trig_EF_pv_x;   //!
   TBranch        *b_trig_EF_pv_y;   //!
   TBranch        *b_trig_EF_pv_z;   //!
   TBranch        *b_trig_EF_pv_type;   //!
   TBranch        *b_trig_EF_pv_err_x;   //!
   TBranch        *b_trig_EF_pv_err_y;   //!
   TBranch        *b_trig_EF_pv_err_z;   //!
   TBranch        *b_trig_EF_tau_n;   //!
   TBranch        *b_trig_EF_tau_Et;   //!
   TBranch        *b_trig_EF_tau_pt;   //!
   TBranch        *b_trig_EF_tau_m;   //!
   TBranch        *b_trig_EF_tau_eta;   //!
   TBranch        *b_trig_EF_tau_phi;   //!
   TBranch        *b_trig_EF_tau_px;   //!
   TBranch        *b_trig_EF_tau_py;   //!
   TBranch        *b_trig_EF_tau_pz;   //!
   TBranch        *b_trig_EF_tau_author;   //!
   TBranch        *b_trig_EF_tau_RoIWord;   //!
   TBranch        *b_trig_EF_tau_nProng;   //!
   TBranch        *b_trig_EF_tau_numTrack;   //!
   TBranch        *b_trig_EF_tau_seedCalo_numTrack;   //!
   TBranch        *b_trig_EF_tau_seedCalo_track_n;   //!
   TBranch        *b_trig_EF_tau_seedCalo_wideTrk_n;   //!
   TBranch        *b_trig_EF_tau_otherTrk_n;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_n;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_algorithmId;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_trackStatus;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_chi2Ndof;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_param_a0;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_param_z0;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_param_phi0;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_param_eta;   //!
   TBranch        *b_trig_L2_trk_idscan_tau_param_pt;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_n;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_algorithmId;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_trackStatus;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_chi2Ndof;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_param_a0;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_param_z0;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_param_phi0;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_param_eta;   //!
   TBranch        *b_trig_L2_trk_sitrack_tau_param_pt;   //!
   TBranch        *b_trig_L2_tau_n;   //!
   TBranch        *b_trig_L2_tau_pt;   //!
   TBranch        *b_trig_L2_tau_eta;   //!
   TBranch        *b_trig_L2_tau_phi;   //!
   TBranch        *b_trig_L2_tau_px;   //!
   TBranch        *b_trig_L2_tau_py;   //!
   TBranch        *b_trig_L2_tau_pz;   //!
   TBranch        *b_trig_L2_tau_RoIWord;   //!
   TBranch        *b_trig_L2_tau_tracks_algorithmId;   //!
   TBranch        *b_trig_L2_tau_idscan_trk_n;   //!
   TBranch        *b_trig_L2_tau_idscan_trk_index;   //!
   TBranch        *b_trig_L2_tau_sitrack_trk_n;   //!
   TBranch        *b_trig_L2_tau_sitrack_trk_index;   //!
   TBranch        *b_trig_L1_esum_thrNames;   //!
   TBranch        *b_trig_L1_esum_ExMiss;   //!
   TBranch        *b_trig_L1_esum_EyMiss;   //!
   TBranch        *b_trig_L1_esum_energyT;   //!
   TBranch        *b_trig_L1_esum_overflowX;   //!
   TBranch        *b_trig_L1_esum_overflowY;   //!
   TBranch        *b_trig_L1_esum_overflowT;   //!
   TBranch        *b_trig_L2_met_MEx;   //!
   TBranch        *b_trig_L2_met_MEy;   //!
   TBranch        *b_trig_L2_met_MEz;   //!
   TBranch        *b_trig_L2_met_sumEt;   //!
   TBranch        *b_trig_L2_met_sumE;   //!
   TBranch        *b_trig_L2_met_flag;   //!
   TBranch        *b_trig_L2_met_nameOfComponent;   //!
   TBranch        *b_trig_L2_met_MExComponent;   //!
   TBranch        *b_trig_L2_met_MEyComponent;   //!
   TBranch        *b_trig_L2_met_MEzComponent;   //!
   TBranch        *b_trig_L2_met_sumEtComponent;   //!
   TBranch        *b_trig_L2_met_sumEComponent;   //!
   TBranch        *b_trig_L2_met_componentCalib0;   //!
   TBranch        *b_trig_L2_met_componentCalib1;   //!
   TBranch        *b_trig_L2_met_sumOfSigns;   //!
   TBranch        *b_trig_L2_met_usedChannels;   //!
   TBranch        *b_trig_L2_met_status;   //!
   TBranch        *b_trig_L2_feb_met_MEx;   //!
   TBranch        *b_trig_L2_feb_met_MEy;   //!
   TBranch        *b_trig_L2_feb_met_MEz;   //!
   TBranch        *b_trig_L2_feb_met_sumEt;   //!
   TBranch        *b_trig_L2_feb_met_sumE;   //!
   TBranch        *b_trig_L2_feb_met_flag;   //!
   TBranch        *b_trig_L2_feb_met_nameOfComponent;   //!
   TBranch        *b_trig_L2_feb_met_MExComponent;   //!
   TBranch        *b_trig_L2_feb_met_MEyComponent;   //!
   TBranch        *b_trig_L2_feb_met_MEzComponent;   //!
   TBranch        *b_trig_L2_feb_met_sumEtComponent;   //!
   TBranch        *b_trig_L2_feb_met_sumEComponent;   //!
   TBranch        *b_trig_L2_feb_met_componentCalib0;   //!
   TBranch        *b_trig_L2_feb_met_componentCalib1;   //!
   TBranch        *b_trig_L2_feb_met_sumOfSigns;   //!
   TBranch        *b_trig_L2_feb_met_usedChannels;   //!
   TBranch        *b_trig_L2_feb_met_status;   //!
   TBranch        *b_trig_EF_met_MEx;   //!
   TBranch        *b_trig_EF_met_MEy;   //!
   TBranch        *b_trig_EF_met_MEz;   //!
   TBranch        *b_trig_EF_met_sumEt;   //!
   TBranch        *b_trig_EF_met_sumE;   //!
   TBranch        *b_trig_EF_met_flag;   //!
   TBranch        *b_trig_EF_met_nameOfComponent;   //!
   TBranch        *b_trig_EF_met_MExComponent;   //!
   TBranch        *b_trig_EF_met_MEyComponent;   //!
   TBranch        *b_trig_EF_met_MEzComponent;   //!
   TBranch        *b_trig_EF_met_sumEtComponent;   //!
   TBranch        *b_trig_EF_met_sumEComponent;   //!
   TBranch        *b_trig_EF_met_componentCalib0;   //!
   TBranch        *b_trig_EF_met_componentCalib1;   //!
   TBranch        *b_trig_EF_met_sumOfSigns;   //!
   TBranch        *b_trig_EF_met_usedChannels;   //!
   TBranch        *b_trig_EF_met_status;   //!
   TBranch        *b_trig_bgCode;   //!
   TBranch        *b_jet_AntiKt4TrackZ_n;   //!
   TBranch        *b_jet_AntiKt4TrackZ_E;   //!
   TBranch        *b_jet_AntiKt4TrackZ_pt;   //!
   TBranch        *b_jet_AntiKt4TrackZ_m;   //!
   TBranch        *b_jet_AntiKt4TrackZ_eta;   //!
   TBranch        *b_jet_AntiKt4TrackZ_phi;   //!
   TBranch        *b_jet_AntiKt4TrackZ_el_dr;   //!
   TBranch        *b_jet_AntiKt4TrackZ_el_matched;   //!
   TBranch        *b_jet_AntiKt4TrackZ_mu_dr;   //!
   TBranch        *b_jet_AntiKt4TrackZ_mu_matched;   //!
   TBranch        *b_jet_AntiKt4TrackZ_L1_dr;   //!
   TBranch        *b_jet_AntiKt4TrackZ_L1_matched;   //!
   TBranch        *b_jet_AntiKt4TrackZ_L2_dr;   //!
   TBranch        *b_jet_AntiKt4TrackZ_L2_matched;   //!
   TBranch        *b_jet_AntiKt4TrackZ_EF_dr;   //!
   TBranch        *b_jet_AntiKt4TrackZ_EF_matched;   //!
   TBranch        *b_bunch_configID;   //!

   susy(TTree *tree=0);
   virtual ~susy();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

