subroutine rms2int(  inverse, dt, vrms, nt,  vint )
use sep_func_mod
integer it, wide,    inverse,           nt
real    vmin,                 dt, vrms( nt), vint( nt)
real                     sum( nt), vis ( nt)
if ( inverse .eq. 0) then
  do it= 1, nt
    sum(it)= ((it-1)*dt) * amax1( vrms(it)**2, 1.5**2 )
  end do 
  vis(1) = amin1( vrms(1) ** 2 , 1.5 ** 2 )
  do it= 2, nt
    vis(it) = ( sum(it) - sum(it-1) )/ dt
  end do 
  wide= 2
  do   
    vmin = vis(1)
    do it=1,nt  
      if ( vis(it)<vmin) then
        vmin = vis(it) 
      end if
    end do 
    if ( vmin > 0 ) then
      exit
    end if
    call triangle( wide, 1, nt, vis, vis)           ! smooth vis()
    wide = wide + 1
    if ( wide >= nt/3) then
      call erexit('negative velocity')
    end if
  end do 
  do it= 1, nt
    vint(it) = sqrt( vis(it))
  end do
else
  do it= 1, nt
    vis(it) = vint(it) ** 2
  end do 
  sum(1) = 0.
  do it= 2, nt
    sum(it) = sum(it-1) + vis(it) * dt
  end do 
  vrms(1) = vint(1)
  do it= 2, nt
    vrms(it) = sqrt( sum(it) / ((it-1)*dt) )
  end do
end if 
return
end  