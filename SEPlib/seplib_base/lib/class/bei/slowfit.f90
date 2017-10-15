subroutine slowfit( vsurface, alpha, t0,dt, s0,ds, scan,nt,ns, reg,   &
  & slow)
integer irange, it,is,                                  nt,ns
real num,den, t,s,  vsurface, alpha, t0,dt, s0,ds, scan(nt,ns),reg(nt)&
  &,slow(nt)
do it= 1, nt  
  t= t0 + dt*(it-1) + dt
  reg(it)  = 1./( vsurface * sqrt( (exp(alpha*t) - 1.)/(alpha*t) ))
  slow(it) = reg(it)
end do 
do irange= ns/4, 5, -1  
! shrink the fairway
  do it=     1,   nt      
    t= t0 + dt*(it-1)
    do is= 1, ns    
      s= s0 + ds*(is-1)
      if ( s > slow(it) + irange*ds) then
        scan(it,is) = 0.
      end if
      if ( s < slow(it) - irange*ds) then
        scan(it,is) = 0.
      end if
      if ( s > 1./1.6              ) then
        scan(it,is) = 0.        ! water
      end if
    end do 
    den= 0.0
    num= 0.0
    do is= 1, ns  
      s= s0 + ds*(is-1)
      num = num + scan(it,is) * s
      den = den + scan(it,is)
    end do 
    slow(it) = num / ( den + 1.e-20)
    if ( slow(it) .eq. 0.) then
      slow(it) = 1./vsurface
    end if
  end do
end do 
return
end  
