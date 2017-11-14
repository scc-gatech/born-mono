//#include <boost/filesystem.hpp>

#include <fstream>   // std::ifstream
#include <iostream>  // std::cout
#include "ioModes.h"
#include "zfp.h"
#include "zfp/macros.h"
using namespace SEP;
int main(int argc, char** argv) {
  ioModes modes(argc, argv);

  std::shared_ptr<genericIO> io = modes.getDefaultIO();
  std::shared_ptr<paramObj> par = io->getParamObj();

  std::string in = par->getString(std::string("in"));

  std::string outDir = par->getString(std::string("outdir"));

//  assert(boost::filesystem::create_directories(outDir));
  Json::Value jsonArgs;
  std::shared_ptr<genericRegFile> inp = io->getRegFile(in, usageIn);

  std::shared_ptr<hypercube> hyperIn = inp->getHyper();
  std::vector<int> ng = hyperIn->getNs();

  std::string mode = par->getString(std::string("mode"), std::string("rate"));
  float rate, tolerance;
  int precision;
  if (mode == std::string("rate")) {
    rate = par->getFloat(std::string("rate"));

  } else if (mode == std::string("accuracy")) {
    tolerance = par->getFloat(std::string("tolerance"));
  } else if (mode == std::string("precision")) {
    precision = par->getFloat(std::string("precision"));

  } else
    par->error("Only support rate, accuracy, or precission modes for now");

  long long n123 = (long long)ng[0] * (long long)ng[1] * (long long)ng[2];

  assert(inp->getDataType() == dataFloat);

  long long n45 = 1;
  jsonArgs["ndims"] = (int)ng.size();

  for (int i = 0; i < ng.size(); i++) {
    axis a = hyperIn->getAxis(i + 1);
    jsonArgs[std::string("n") + std::to_string(i + 1)] = a.n;
    jsonArgs[std::string("o") + std::to_string(i + 1)] = a.o;
    jsonArgs[std::string("d") + std::to_string(i + 1)] = a.d;
    jsonArgs[std::string("label") + std::to_string(i + 1)] = a.label;
  }

  if (ng.size() > 3) {
    for (int i = 3; i < ng.size(); i++) {
      n45 = n45 * (long long)ng[i];
    }
  }

  float* buf = new float[n123];

  jsonArgs["nfiles"] = n45;

  for (long long i = 0; i < n45; i++) {
    std::string file =
        outDir + std::string("/") + std::string("file") + std::to_string(i);

    jsonArgs[std::string("file") + std::to_string(i)] = file;
    inp->readFloatStream(buf, n123);
    zfp_type type = zfp_type_float;
    size_t typesize = 0;
    uint dims = std::min((uint)ng.size(), (uint)3);
    uint nx = ng[0], ny, nz;
    if (ng.size() > 1) ny = ng[1];
    if (ng.size() > 2) nz = ng[2];

    uint minbits = ZFP_MIN_BITS;
    uint maxbits = ZFP_MAX_BITS;
    uint maxprec = ZFP_MAX_PREC;
    int minexp = ZFP_MIN_EXP;
    int header = 0;
    int quiet = 0;
    int stats = 0;
    char* inpath = 0;
    char* zfppath = 0;
    char* outpath = 0;

    /* local variables */
    zfp_field* field = zfp_field_alloc();
    zfp_stream* zfp = zfp_stream_open(NULL);
    ;
    bitstream* stream = NULL;
    void* fo = NULL;
    void* buffer = NULL;
    size_t rawsize = 0;
    size_t zfpsize = 0;
    size_t bufsize = 0;

    zfp_field_set_type(field, type);
    zfp_field_set_pointer(field, buf);
    switch (dims) {
      case 1:
        zfp_field_set_size_1d(field, nx);
        break;
      case 2:
        zfp_field_set_size_2d(field, nx, ny);
        break;
      case 3:
        zfp_field_set_size_3d(field, nx, ny, nz);
        break;
    }

    /* set (de)compression mode */
    switch (mode.c_str()[0]) {
      case 'a':
        std::cerr << "tolerance " << tolerance << std::endl;
        zfp_stream_set_accuracy(zfp, tolerance);
        break;
      case 'p':
        std::cerr << "precision " << precision << std::endl;
        zfp_stream_set_precision(zfp, precision);
        break;
      case 'r':
        std::cerr << "rate " << rate << std::endl;
        zfp_stream_set_rate(zfp, rate, type, dims, 0);
        break;
    }

    /* allocate buffer for compressed data */
    bufsize = zfp_stream_maximum_size(zfp, field);
    if (!bufsize) par->error(std::string("invalid compression parameters"));
    buffer = malloc(bufsize);
    if (!buffer) par->error(std::string("cannot allocate memory\n"));
    ;

    /* associate compressed bit stream with memory buffer */
    stream = stream_open(buffer, bufsize);
    if (!stream) par->error("cannot open compressed stream\n");
    zfp_stream_set_bit_stream(zfp, stream);

    /* optionally // header */
    if (!zfp_write_header(zfp, field, ZFP_HEADER_FULL))
      par->error(std::string("trouble writing header"));

    /* compress data */
    zfpsize = zfp_compress(zfp, field);
    if (zfpsize == 0) par->error("compression failed");

    FILE* fileB = fopen(file.c_str(), "wb");
    if (!fileB) par->error(std::string("can not open binary file ") + file);

    if (fwrite(buffer, 1, zfpsize, fileB) != zfpsize)
      par->error(std::string("trouble writing compressed file"));
    fclose(fileB);

    /* free allocated storage */
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);
    free(buffer);
  }

  std::ofstream outps;
  std::string jsonFile = outDir + std::string("/file.json");
  outps.open(jsonFile, std::ofstream::out);

  if (!outps) {
    std::cerr << std::string("Trouble opening for write") + jsonFile
              << std::endl;
    throw std::exception();
  }
  try {
    outps << jsonArgs;
  } catch (int x) {
    std::cerr << std::string("Trouble writing JSON file ") + jsonFile
              << std::endl;
    throw std::exception();
  }

  return EXIT_SUCCESS;
}
