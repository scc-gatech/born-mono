#include "region_data.h"
#include "float_buffer.h"
#include "byte_buffer.h"

region_data::region_data(hypercube *g, param_func *p, int in){


    buf.push_back(new region(g));
    reg=(region*) buf[0];
    set_basics("region","region",g,reg->io,p,in,1);
    
    datas=io->return_hyper();
}

