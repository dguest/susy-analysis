#ifndef BOSON_PT_REWEIGHTER_HH
#define BOSON_PT_REWEIGHTER_HH

// right now just a wrapper for Mark's pt correction function, may be more
// advanced in the future

class McParticleBuffer;

class BosonPtReweighter
{
public:
  float get_boson_weight(const McParticleBuffer&);
  float get_boson_pt(const McParticleBuffer&);
};

#endif
