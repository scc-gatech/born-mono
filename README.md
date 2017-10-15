# Born
Mono-Repo for the Born Seismic Imaging Challenge. Instructions for the challenge can be found [here](http://www.studentclustercompetition.us/2017/SeismicModel-Howto.pdf)

SCC has provided us the code as a set of git repositories, each hosted on Stanford's [gitlab](http://zapad.stanford.edu/bob/) service. This is our attempt at wrangling these different repositories into one cohesive repo, without needing to resort to the nastiness that is git submodules. 


### How to build
There is a build script in the top level directory as well as in each of the major subdirectories. Executing 
```
./build 
```
will spawn off the builds for `SEPlib`, `hypercube`, `genericIO` and then `Born`.
