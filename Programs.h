#include <G35.h>
#include <Eyes.h>

LightProgram *CreateEyes(G35& g35) { return new Eyes(g35); }
