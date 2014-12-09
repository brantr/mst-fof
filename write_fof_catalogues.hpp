#ifndef  WRITE_FOF_CATALOGUES_H
#define  WRITE_FOF_CATALOGUES_H

#include "shock_data_types.hpp"

void write_fof_list(char fname[], vector<shock> s);
void write_fof_data(char fname[], vector<shock> s, vector<tracer> t);
void write_null_fof_list(char fname[]);
void write_null_fof_data(char fname[]);


#endif /*WRITE_FOF_CATALOGUES_H*/
