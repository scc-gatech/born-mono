
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

  std::string out = par->getString(std::string("out"));
  std::string inDir = par->getString(std::string("indir"));
  std::string jsonFile = inDir + std::string("/file.json");
  Json::Value jsonArgs;

  std::ifstream inps;
  inps.open(jsonFile, std::ifstream::in);

  if (!inps) {
    std::cerr << std::string("Trouble opening1 " + jsonFile) << std::endl;
    throw std::exception();
  }
  try {
    inps >> jsonArgs;
  } catch (int x) {
    std::cerr << std::string("Trouble parsing JSON file " + jsonFile)
              << std::endl;
    throw std::exception();
  }
  std::vector<axis> axes;

  for (int i = 0; i < jsonArgs.get("ndims", 1).asInt(); i++) {
    axis a;
    a.n = jsonArgs.get(std::string("n") + std::to_string(i + 1), 1).asInt();
    a.o = jsonArgs.get(std::string("o") + std::to_string(i + 1), 1).asFloat();
    a.d = jsonArgs.get(std::string("d") + std::to_string(i + 1), 1).asFloat();
    a.label = jsonArgs.get(std::string("label") + std::to_string(i + 1), 1)
                  .asString();
    axes.push_back(a);
  }

  std::shared_ptr<genericRegFile> outp = io->getRegFile(out, usageOut);

  std::shared_ptr<hypercube> hyperOut(new hypercube(axes));

  outp->setHyper(hyperOut);

  std::vector<int> ng = hyperOut->getNs();

  outp->writeDescription();

  int n45 = jsonArgs.get("ndim", 1).asInt();

  for (long long i = 0; i < n45; i++) {
    std::string xx = std::string("file") + std::to_string(i);
    std::string fileS = jsonArgs.get(xx, 1).asString();

    /* default settings */
    zfp_type type = zfp_type_none;
    size_t typesize = 0;
    uint dims = 0;
    uint nx = 0;
    uint ny = 0;
    uint nz = 0;
    double rate = 0;
    uint precision = 0;
    double tolerance = 0;
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
    char mode = 0;

    /* local variables */
    zfp_field* field = NULL;
    zfp_stream* zfp = NULL;
    bitstream* stream = NULL;
    void* fi = NULL;
    void* fo = NULL;
    void* buffer = NULL;
    size_t rawsize = 0;
    size_t zfpsize = 0;
    size_t bufsize = 0;

    type = zfp_type_float;

    typesize = zfp_type_size(type);

    zfp = zfp_stream_open(NULL);
    field = zfp_field_alloc();

    /* read uncompressed or compressed file */

    /* read compressed input file in increasingly large chunks */
    FILE* file = fopen(fileS.c_str(), "rb");
    if (!file) {
      fprintf(stderr, "cannot open compressed file\n");
      return EXIT_FAILURE;
    }
    bufsize = 0x100;
    do {
      bufsize *= 2;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "cannot allocate memory\n");
        return EXIT_FAILURE;
      }
      zfpsize += fread((uchar*)buffer + zfpsize, 1, bufsize - zfpsize, file);
    } while (zfpsize == bufsize);
    if (ferror(file)) {
      fprintf(stderr, "cannot read compressed file\n");
      return EXIT_FAILURE;
    }
    fclose(file);

    /* associate bit stream with buffer */
    stream = stream_open(buffer, bufsize);
    if (!stream) {
      fprintf(stderr, "cannot open compressed stream\n");
      return EXIT_FAILURE;
    }
    zfp_stream_set_bit_stream(zfp, stream);

    zfp_stream_rewind(zfp);

    if (!zfp_read_header(zfp, field, ZFP_HEADER_FULL))
      par->error(std::string("incorrect or missing header\n"));

    type = field->type;
    typesize = sizeof(float);

    nx = MAX(field->nx, 1u);
    ny = MAX(field->ny, 1u);
    nz = MAX(field->nz, 1u);
    long long n123 = nx * ny * nz;

    /* allocate memory for decompressed data */
    rawsize = typesize * nx * ny * nz;
    fo = malloc(rawsize);
    if (!fo) par->error(std::string("cannot allocate memory\n"));

    zfp_field_set_pointer(field, fo);

    /* decompress data */
    if (!zfp_decompress(zfp, field))
      par->error(std::string("decompression failed\n"));
std::cerr<<"before write"<<std::endl;
    outp->writeFloatStream((float*)fo, n123);
std::cerr<<"after write"<<std::endl;

    /* free allocated storage */
    zfp_field_free(field);
    zfp_stream_close(zfp);
    stream_close(stream);
    free(buffer);
    free(fo);
  }

  return EXIT_SUCCESS;
}
