#include "basicIO.h"
#include <assert.h>
#include <string.h>
#include <unistd.h>
using namespace SEP;
myFileIO::myFileIO(const std::string &nm, const usage_code usage,
                   const int reelH, const int traceH, const int esize,
                   const bool swapData, std::shared_ptr<hypercube> hyper) {
  setFileParams(nm, usage, reelH, traceH, esize, swapData, hyper);
  std::string mode;
  if (_usage == usageIn || usage == usageInOut) {
    if (access(nm.c_str(), F_OK) == -1) {
      std::cerr << std::string("Can not open file ") << nm << std::endl;
      if (_usage == usageIn) assert(1 == 2);
      _myf = fopen(nm.c_str(), "w+");
    } else {
      _myf = fopen(nm.c_str(), "r+");
    }

  } else {
    _myf = fopen(nm.c_str(), "w+");
  }
}
void basicIO::readWindow(const std::vector<int> &nw, const std::vector<int> &fw,
                         const std::vector<int> &jw, void *data, void *head) {
  long long optMin = 256 * 1024, optMax = 10 * 1024 * 1024;
  std::vector<axis> axes = _hyper->returnAxes(8);
  if (_traceH + axes[0].n * _esize > optMin)
    optMin = _traceH + axes[0].n * _esize;
  long long blk = _esize * axes[0].n + _traceH;
  int naxes = 1;
  bool breakL = false;

  while (naxes < nw.size() && !breakL) {
    long long btest = blk * (long long)axes[naxes].n;
    if (btest < optMin) {
      naxes++;
      blk = btest;
    } else if (btest < optMax) {
      if (blk > optMin) {
        breakL = true;
      } else {
        naxes++;
        blk = btest;
      }
    } else
      breakL = true;
  }

  std::vector<int> nwi, fwi, jwi, nwo, fwo, jwo;
  nwi.assign(7, 1);
  nwo.assign(7, 1);
  jwi.assign(7, 1);
  jwo.assign(7, 1);
  fwi.assign(7, 0);
  fwo.assign(7, 0);
  for (int i = 0; i < naxes; i++) {
    fwi[i] = fw[i];
    jwi[i] = jw[i];
    nwi[i] = nw[i];
  }
  for (int i = naxes; i < 8; i++) {
    fwo[i - naxes] = fw[i];
    jwo[i - naxes] = jw[i];
    nwo[i - naxes] = nw[i];
  }

  readBlocks(naxes, nwo, fwo, jwo, nwi, fwi, jwi, blk, data, head);
}
void basicIO::writeWindow(const std::vector<int> &nw,
                          const std::vector<int> &fw,
                          const std::vector<int> &jw, const void *data,
                          void *head) {
  long long optMin = 256 * 1024, optMax = 10 * 1024 * 1024;
  std::vector<axis> axes = _hyper->returnAxes(9);
  if (_traceH + axes[0].n * _esize > optMin)
    optMin = _traceH + axes[0].n * _esize;
  long long blk = _esize * axes[0].n + _traceH;
  int naxes = 1;
  bool breakL = false;
  while (naxes < nw.size() && !breakL) {
    long long btest = blk * (long long)axes[naxes].n;
    if (btest < optMin) {
      naxes++;
      blk = btest;
    } else if (btest < optMax) {
      if (blk > optMin) {
        breakL = true;
      } else {
        naxes++;
        blk = btest;
      }
    } else
      breakL = true;
  }
  std::vector<int> nwi, fwi, jwi, nwo, fwo, jwo;
  nwi.assign(7, 1);
  nwo.assign(7, 1);
  jwi.assign(7, 1);
  jwo.assign(7, 1);
  fwi.assign(7, 0);
  fwo.assign(7, 0);
  for (int i = 0; i < naxes; i++) {
    fwi[i] = fw[i];
    jwi[i] = jw[i];
    nwi[i] = nw[i];
  }

  for (int i = naxes; i < nw.size(); i++) {
    fwo[i - naxes] = fw[i];
    jwo[i - naxes] = jw[i];
    nwo[i - naxes] = nw[i];
  }

  writeBlocks(naxes, nwo, fwo, jwo, nwi, fwi, jwi, blk, data, head);
}
void basicIO::readBlocks(const int naxes, const std::vector<int> &nwo,
                         const std::vector<int> &fwo,
                         const std::vector<int> &jwo,
                         const std::vector<int> &nwi,

                         const std::vector<int> &fwi,
                         const std::vector<int> &jwi, const long long sz,
                         void *dat, void *head) {
  char *buf = new char[sz];
  long long szNoHead = sz / (nwi[0] * _esize + _traceH) * (nwi[0] * _esize);
  std::vector<axis> axes = _hyper->returnAxes(7 + naxes);
  long long pto = 0;
  std::vector<long long> blk(8, 1);
  blk[0] = sz;
  for (int i = 0; i < 7; i++)
    blk[i + 1] = blk[i] * (long long)axes[i + naxes].n;

  long long add = _esize;
  for (int i = 0; i < naxes; i++) add *= nwi[i];

  for (long long i6 = 0; i6 < nwo[6]; i6++) {
    long long pt6 = ((long long)(fwo[6] + jwo[6] * i6)) * blk[6];
    for (long long i5 = 0; i5 < nwo[5]; i5++) {
      long long pt5 = pt6 + ((long long)(fwo[5] + jwo[5] * i5)) * blk[5];
      for (long long i4 = 0; i4 < nwo[4]; i4++) {
        long long pt4 = pt5 + ((long long)(fwo[4] + jwo[4] * i4)) * blk[4];
        for (long long i3 = 0; i3 < nwo[3]; i3++) {
          long long pt3 = pt4 + ((long long)(fwo[3] + jwo[3] * i3)) * blk[3];
          for (long long i2 = 0; i2 < nwo[2]; i2++) {
            long long pt2 = pt3 + ((long long)(fwo[2] + jwo[2] * i2)) * blk[2];
            for (long long i1 = 0; i1 < nwo[1]; i1++) {
              long long pt1 =
                  pt2 + ((long long)(fwo[1] + jwo[1] * i1)) * blk[1];
              for (long long i0 = 0; i0 < nwo[0]; i0++) {
                long long pti =
                    pt1 + ((long long)(fwo[0] + jwo[0] * i0)) * blk[0];
                // seekToPos(pti+_reelH);
                seekToPos(pti + _reelH);

                readStream(sz, buf);
                blockToParts(nwi, fwi, jwi, buf, &(((char *)(dat))[pto]), head);
                float *d2 = (float *)dat;
                pto += add;
                // assert(1==2);
              }
            }
          }
        }
      }
    }
  }
  delete[] buf;
}

void basicIO::writeBlocks(const int naxes, const std::vector<int> &nwo,
                          const std::vector<int> &fwo,
                          const std::vector<int> &jwo,
                          const std::vector<int> &nwi,
                          const std::vector<int> &fwi,
                          const std::vector<int> &jwi, const long long sz,
                          const void *dat, const void *head) {
  long long add = _esize;
  for (int i = 0; i < naxes; i++) add *= nwi[i];
  char *buf = new char[sz];
  std::vector<axis> axes = _hyper->returnAxes(7 + naxes);
  long long pto = 0;
  long long eachBlock = _esize;
  for (int i = 0; i < 7; i++) eachBlock *= nwi[i];
  std::vector<long long> blk(8, 1);
  blk[0] = sz;
  for (int i = 0; i < 7; i++)
    blk[i + 1] = blk[i] * (long long)axes[i + naxes].n;
  for (long long i6 = 0; i6 < nwo[6]; i6++) {
    long long pt6 = ((long long)(fwo[6] + jwo[6] * i6)) * blk[6];
    for (long long i5 = 0; i5 < nwo[5]; i5++) {
      long long pt5 = pt6 + ((long long)(fwo[5] + jwo[5] * i5)) * blk[5];
      for (long long i4 = 0; i4 < nwo[4]; i4++) {
        long long pt4 = pt5 + ((long long)(fwo[4] + jwo[4] * i4)) * blk[4];
        for (long long i3 = 0; i3 < nwo[3]; i3++) {
          long long pt3 = pt4 + ((long long)(fwo[3] + jwo[3] * i3)) * blk[3];
          for (long long i2 = 0; i2 < nwo[2]; i2++) {
            long long pt2 = pt3 + ((long long)(fwo[2] + jwo[2] * i2)) * blk[2];
            for (long long i1 = 0; i1 < nwo[1]; i1++) {
              long long pt1 =
                  pt2 + ((long long)(fwo[1] + jwo[1] * i1)) * blk[1];
              for (long long i0 = 0; i0 < nwo[0]; i0++) {
                long long pti =
                    pt1 + ((long long)(fwo[0] + jwo[0] * i0)) * blk[0];
                partsToBlock(nwi, fwi, jwi, buf, (char *)dat + pto, head);
                pto += add;

                seekToPos(pti + _reelH);
                writeStream(sz, buf);
              }
            }
          }
        }
      }
    }
  }
  delete[] buf;
}
void basicIO::readTraceStream(const long long sz, void *dat, void *head) {
  long long cp = getCurrentPos();
  long long n = _hyper->getAxis(1).n;
  if (_traceH > 0) {
    assert((cp - (long long)_reelH) % (n + _traceH) == 0);
    assert(sz % n == 0);
    long long ntrace = sz / n;
    long long nH = _traceH + n * _esize;
    long long sz2 = ntrace * nH;
    char *buf = new char[sz2];
    readStream(sz2, buf);
    char *headC = (char *)head;
    char *datC = (char *)dat;
    for (long long i2 = 0; i2 < ntrace; i2++) {
      if (head != 0) memcpy(&headC[i2 * _traceH], &buf[nH * i2], _traceH);
      memcpy(&datC[i2 * n], &buf[nH * i2 + _traceH], n * _esize);
    }
    delete[] buf;

  } else
    readStream(sz * _esize, dat);
}
void basicIO::swap_float_bytes(int n, float *buf) {
  /*
     int *tni4;
     for(int i=0; i< n; i++){
     float *tnf4=&buf[i];
     int *tni4=(int *)tnf4;
   * tni4=(((*tni4>>24)&0xff) | ((*tni4&0xff)<<24) |
             ((*tni4>>8)&0xff00) | ((*tni4&0xff00)<<8));

     }
   */
  int *buf2 = (int *)buf;
  register int fconv, fmant, i, t;
  int myendian = 0;
  for (i = 0; i < n; ++i) {
    fconv = buf2[i];

    /* if little endian, i.e. endian=0 do this */
    if (myendian == 0)
      fconv = (fconv << 24) | ((fconv >> 24) & 0xff) | ((fconv & 0xff00) << 8) |
              ((fconv & 0xff0000) >> 8);
    bool breakit = false;
    if (fconv || breakit) {
      fmant = 0x00ffffff & fconv;
      /* The next two lines were added by Toralf Foerster */
      /* to trap non-IBM format data i.e. conv=0 data  */
      if (fmant == 0)
        breakit = true;
      else {
        t = (int)((0x7f000000 & fconv) >> 22) - 130;
        while (!(fmant & 0x00800000)) {
          --t;
          fmant <<= 1;
        }
        if (t > 254)
          fconv = (0x80000000 & fconv) | 0x7f7fffff;
        else if (t <= 0)
          fconv = 0;
        else
          fconv = (0x80000000 & fconv) | (t << 23) | (0x007fffff & fmant);
      }
    }
    buf2[i] = fconv;
  }
}
void basicIO::writeTraceStream(const long long sz, const void *dat,
                               const void *head) {
  long long cp = getCurrentPos();
  long long n = _hyper->getAxis(1).n;
  if (_traceH > 0) {
    assert((cp - (long long)_reelH) % (n + _traceH) == 0);
    assert(sz % n == 0);
    long long ntrace = sz / n;
    long long nH = _traceH + n * _esize;
    long long sz2 = ntrace * nH;
    char *buf = new char[sz2];
    char *headC;
    if (head != 0)
      headC = (char *)head;
    else
      headC = new char[ntrace * _traceH];
    char *datC = (char *)dat;
    for (long long i2 = 0; i2 < ntrace; i2++) {
      memcpy(&buf[nH * i2], &headC[i2 * _traceH], _traceH);
      memcpy(&buf[nH * i2 + _traceH], &datC[i2 * n], n * _esize);
    }
    if (head == 0) delete[] headC;
    writeStream(sz2, buf);
    delete[] buf;
  } else
    writeStream(sz * _esize, dat);
}
void basicIO::writeReelHead(const void *reelH) {
  assert(_reelH == fwrite(reelH, 1, _reelH, _myf));
}
void myFileIO::readStream(const long long sz, void *data) {
  float *d2 = (float *)data;
  int *d3 = (int *)data;
  long long sz2 = fread(data, 1, sz, _myf);
  if (sz != sz2) {
    fprintf(stderr, "tried to read %d bytes, read %d \n", sz2, sz);
  }
  assert(sz == sz2);
  if (_swapData) swap_float_bytes(sz / 4, d2);
}
void myFileIO::writeStream(const long long sz, const void *data) {
  float *d2 = (float *)data;
  if (_swapData) swap_float_bytes(sz / 4, (float *)d2);
  long long ierr = fwrite(data, 1, sz, _myf);
  assert(sz == ierr);
}
void basicIO::blockToParts(const std::vector<int> &nwo,
                           const std::vector<int> &fwo,
                           const std::vector<int> &jwo, const void *in,
                           void *out, void *head) {
  std::vector<axis> axes = _hyper->returnAxes(8);
  char *inH = (char *)in, *outH = (char *)out, *headH = (char *)head;
  long long ih = 0, id = 0;
  float *of = (float *)out;
  std::vector<long long> blk(8, 1);
  blk[0] = _esize;
  for (int i = 0; i < 7; i++) blk[i + 1] = blk[i] * (long long)axes[i].n;
  for (long long i6 = 0; i6 < nwo[6]; i6++) {
    long long pt6 = ((long long)(fwo[6] + jwo[6] * i6)) * blk[6];
    for (long long i5 = 0; i5 < nwo[5]; i5++) {
      long long pt5 = pt6 + ((long long)(fwo[5] + jwo[5] * i5)) * blk[5];
      for (long long i4 = 0; i4 < nwo[4]; i4++) {
        long long pt4 = pt5 + ((long long)(fwo[4] + jwo[4] * i4)) * blk[4];
        for (long long i3 = 0; i3 < nwo[3]; i3++) {
          long long pt3 = pt4 + ((long long)(fwo[3] + jwo[3] * i3)) * blk[3];
          for (long long i2 = 0; i2 < nwo[2]; i2++) {
            long long pt2 = pt3 + ((long long)(fwo[2] + jwo[2] * i2)) * blk[2];
            for (long long i1 = 0; i1 < nwo[1]; i1++) {
              long long pt1 =
                  pt2 + ((long long)(fwo[1] + jwo[1] * i1)) * blk[1];
              /* we are now at the trace level*/
              if (head != 0) {
                memcpy(&headH[ih], &inH[pt1], _traceH);
                ih += _traceH;
              }
              pt1 += _traceH;

              for (int i0 = 0; i0 < nwo[0]; i0++, id += _esize) {
                memcpy(&outH[id], &inH[pt1 + (fwo[0] + jwo[0] * i0) * blk[0]],
                       _esize);
              }
            }
          }
        }
      }
    }
  }
}
void basicIO::partsToBlock(const std::vector<int> &nwo,
                           const std::vector<int> &fwo,
                           const std::vector<int> &jwo, void *in,
                           const void *out, const void *head) {
  std::vector<axis> axes = _hyper->returnAxes(8);
  char *inH = (char *)in, *outH = (char *)out, *headH = (char *)head;
  long long ih = 0, id = 0;

  assert(_traceH == 0);
  std::vector<long long> blk(8, 1);
  blk[0] = _esize;
  for (int i = 0; i < 7; i++) blk[i + 1] = blk[i] * (long long)axes[i].n;
  for (long long i6 = 0; i6 < nwo[6]; i6++) {
    long long pt6 = ((long long)(fwo[6] + jwo[6] * i6)) * blk[6];
    for (long long i5 = 0; i5 < nwo[5]; i5++) {
      long long pt5 = pt6 + ((long long)(fwo[5] + jwo[5] * i5)) * blk[5];
      for (long long i4 = 0; i4 < nwo[4]; i4++) {
        long long pt4 = pt5 + ((long long)(fwo[4] + jwo[4] * i4)) * blk[4];
        for (long long i3 = 0; i3 < nwo[3]; i3++) {
          long long pt3 = pt4 + ((long long)(fwo[3] + jwo[3] * i3)) * blk[3];
          for (long long i2 = 0; i2 < nwo[2]; i2++) {
            long long pt2 = pt3 + ((long long)(fwo[2] + jwo[2] * i2)) * blk[2];
            for (long long i1 = 0; i1 < nwo[1]; i1++) {
              long long pt1 =
                  pt2 + ((long long)(fwo[1] + jwo[1] * i1)) * blk[1];
              /* we are now at the trace level*/
              if (head != 0) {
                memcpy(&headH[ih], &inH[pt1], _traceH);
                ih += _traceH;
              }
              pt1 += _traceH;
              for (int i0 = 0; i0 < nwo[0]; i0++, id++) {
                memcpy(&inH[pt1 + (fwo[0] + jwo[0] * i0) * blk[0]], &outH[id],
                       _esize);
              }
            }
          }
        }
      }
    }
  }
}
void basicIO::setFileParams(const std::string nm, const usage_code usage,
                            const int reelH, const int traceH, const int esize,
                            const bool swapData,
                            std::shared_ptr<hypercube> hyper) {
  _swapData = swapData;
  _nm = nm;
  _usage = usage;
  _reelH = reelH;
  _traceH = traceH;
  _esize = esize;
  _hyper = hyper;
}
