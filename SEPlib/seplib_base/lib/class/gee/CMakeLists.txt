cmake_minimum_required(VERSION 2.8)
project(SEPLIB-base-lib-class-gee)


include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../../corelibs/sep)


set (SOURCES 
arnoldi.f90	      helicon.f90	mask1.f90	print.f90
autocorr.f90	      helixcartmod.f90	mask2i.f90	quantile.f90
bigsolver.f90	      helix.f90		matmult.f90	random2.f90
bin2.f90	      helocut.f90	medbin2.f90	random.f90
binpull1.f90	      hipass.f90	micaf1.f90	rediameter.f90
binpull2.f90	      hypotenuse.f90	mis1.f90	refine2.f90
bound.f90	      hypotenusei.f90	mis2.f90	regrid.f90
box.f90		      icaf1.f90		misif.f90	regsolver.f90
broyden.f90	      icai1.f90		misinput.f90	scaletrace.f90
cartesian.f90	      igrad1.f90	mkwallwt.f90	signoi.f90
causint.f90	      igrad2.f90	mshconest.f90	smallchain.f90
cdoubint.f90	      imospray.f90	mshelicon.f90	smallsolver.f90
cdstep.f90	      int1.f90		mshelix.f90	solver_prc.f90
cgmeth.f90	      int2.f90		msmis2.f90	solver_reg.f90
cgstep.f90	      interp.f90	mspef.f90	solver_report.f90
chain.f90	      interp_new.f90	nbound.f90	solver_smp.f90
combine.f90	      invint1.f90	nfilt_io.f90	solver_tiny.f90
compress.f90	      invint2.f90	nhconest.f90	spraysum.f90
conjgrad.f90	      invstack.f90	nhelicon.f90	steepdip.f90
conv.f90	      irls.f90		nhelix.f90	steering.f90
copy.f90	      iwilson.f90	nicaf1.f90	tcaf1.f90
createhelixmod.f90    lace.f90		nmis2.f90	tcai1.f90
createmshelixmod.f90  lapfac.f90	nmisinput.f90	tcai2.f90
createnhelixmod.f90   lapfill2.f90	normbin.f90	tent.f90
crosswilson.f90       laplac2.f90	npef.f90	triangle1.f90
ddot.f90	      leakint2.f90	npolydiv2.f90	tridiag.f90
deburst1.f90	      leakint.f90	npolydiv.f90	triple.f90
dottest.f90	      levint.f90	parcel.f90	tvdec.f90
double.f90	      light.f90		partan.f90	tvdecon.f90
fixbad.f90	      lint1.f90		patch.f90	unbox.f90
fold.f90	      lint2.f90		patching.f90	unwrap.f90
gauss.f90	      lint.f90		paul.f90	vrms2int.f90
gdecon.f90	      llist.f90		pefconv.f90	vrsolver.f90
gmres.f90	      loconvol.f90	pefest.f90	weight.f90
grad2fill.f90	      lolace.f90	pef.f90		weightsolver.f90
hconest.f90	      lomis.f90		peftc.f90	wilson.f90
helderiv.f90	      lopef.f90		polydiv.f90
heliarr.f90	      lsqr.f90		precsolver.f90)





add_library(sepgeef90 ${SOURCES})
target_link_libraries(sepgeef90 sepclassutil)
install(TARGETS sepgeef90 DESTINATION lib)
