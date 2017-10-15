module oc_tags_mod
  !!=======================
  !! Paul Sava    (C) 2000
  !! paul@sep.stanford.edu
  !! oclib v2.0
  !!=======================

  implicit none

  !! SIMPLE SOLVER
  character(len=128), public, parameter :: SS_Rd = '_Rd.SS'

  character(len=128), public, parameter :: SS_g  =  '_g.SS'
  character(len=128), public, parameter :: SS_Gd = '_Gd.SS'

  character(len=128), public, parameter :: SS_s  =  '_s.SS'
  character(len=128), public, parameter :: SS_Sd = '_Sd.SS'

  character(len=128), public, parameter :: SS_Mmovie  =  '_Mmovie'
  character(len=128), public, parameter :: SS_Dmovie  =  '_Dmovie'


  !! REGULARIZED SOLVER
  character(len=128), public, parameter :: RS_Rm = '_Rm.RS'
  character(len=128), public, parameter :: RS_Rd = '_Rd.RS'

  character(len=128), public, parameter :: RS_g  =  '_g.RS'
  character(len=128), public, parameter :: RS_Gm = '_Gm.RS'
  character(len=128), public, parameter :: RS_Gd = '_Gd.RS'

  character(len=128), public, parameter :: RS_s  =  '_s.RS'
  character(len=128), public, parameter :: RS_Sm = '_Sm.RS'
  character(len=128), public, parameter :: RS_Sd = '_Sd.RS'

  character(len=128), public, parameter :: RS_Mmovie  =  '_Mmovie'
  character(len=128), public, parameter :: RS_Dmovie  =  '_Dmovie'

  character(len=128), public, parameter :: RS_dd = '_dd.RS'
  character(len=128), public, parameter :: RS_rr = '_rr.RS'


  !! PRECONDITIONED SOLVER
  character(len=128), public, parameter :: PS_p  =  '_p.PS'
  character(len=128), public, parameter :: PS_Rm = '_Rm.PS'
  character(len=128), public, parameter :: PS_Rd = '_Rd.PS'

  character(len=128), public, parameter :: PS_g  =  '_g.PS'
  character(len=128), public, parameter :: PS_Gm = '_Gm.PS'
  character(len=128), public, parameter :: PS_Gd = '_Gd.PS'

  character(len=128), public, parameter :: PS_s  =  '_s.PS'
  character(len=128), public, parameter :: PS_Sm = '_Sm.PS'
  character(len=128), public, parameter :: PS_Sd = '_Sd.PS'

  character(len=128), public, parameter :: PS_Mmovie  =  '_Mmovie'
  character(len=128), public, parameter :: PS_Dmovie  =  '_Dmovie'

  character(len=128), public, parameter :: PS_dd = '_dd.PS'
  character(len=128), public, parameter :: PS_rr = '_rr.PS'


  !! SIMPLE LSQR
  character(len=128), public, parameter :: SL_U = '_U.SL'
  character(len=128), public, parameter :: SL_v = '_v.SL'
  character(len=128), public, parameter :: SL_w = '_w.SL'

  character(len=128), public, parameter :: SL_Mmovie = '_Mmovie'


  !! REGULARIZED LSQR
  character(len=128), public, parameter :: RL_Ud = '_Ud.RL'
  character(len=128), public, parameter :: RL_Um = '_Um.RL'

  character(len=128), public, parameter :: RL_v = '_v.RL'
  character(len=128), public, parameter :: RL_w = '_w.RL'

  character(len=128), public, parameter :: RL_Mmovie = '_Mmovie'


  !! PRECONDITIONED LSQR
  character(len=128), public, parameter :: PL_Ud = '_Ud.PL'
  character(len=128), public, parameter :: PL_Um = '_Um.PL'  

  character(len=128), public, parameter :: PL_v = '_v.PL'
  character(len=128), public, parameter :: PL_w = '_w.PL'

  character(len=128), public, parameter :: PL_p = '_p.PL'

  character(len=128), public, parameter :: PL_Mmovie = '_Mmovie'


  !! EPSILON ESTIMATOR
  character(len=128), public, parameter :: EPS_Rm = '_Rm.EPS'
  character(len=128), public, parameter :: EPS_Rd = '_Rd.EPS'

  character(len=128), public, parameter :: EPS_gm = '_gm.EPS'
  character(len=128), public, parameter :: EPS_gd = '_gd.EPS'

  character(len=128), public, parameter :: EPS_m  = '_m.EPS'
  character(len=128), public, parameter :: EPS_d  = '_d.EPS'


end module oc_tags_mod













