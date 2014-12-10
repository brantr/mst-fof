#ifndef LOAD_PARTICLES_H
#define LOAD_PARTICLES_H
#include "shock_data_types.hpp"
long load_particles(vector<tracer> *t, long n_groups, long n_per_group);
long fake_particles(vector<tracer> *t, long n_groups, long n_per_group);
#endif /*LOAD_PARTICLES_H*/
