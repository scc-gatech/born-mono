module pctrist_mod
  !! Parallel Complex TRIdiagonal STretch

  use utilities_mod
  use ptridiag_mod
  implicit none

  integer, private :: nm,nd
  real,    private :: om,od
  real,    private :: dm,dd
  logical, private :: push
  real,    private :: eps

  integer, private, dimension(:,:), pointer  :: index
  logical, private, dimension(:,:), pointer  :: flag 
  complex, private, dimension(:,:), pointer  :: weight

  complex, private, dimension(:,:), pointer  :: diag, offd
contains
  !----------------------------------------------------------------
  subroutine pctrist_init(nth,ad,am,pushi,epsi)
    integer,     intent(in) :: nth
    type(paxis), intent(in) :: ad,am !! data/model axes
    logical,     optional   :: pushi
    real,        optional   :: epsi

    !! data params
    nd = ad%n
    od = ad%o
    dd = ad%d

    !! model params
    nm = am%n
    om = am%o
    dm = am%d

    eps  = 0.0;    if(present(  epsi))   eps=epsi
    push =.true.;  if(present( pushi))  push=pushi

    if(push) then
       allocate(index(nth,nd),flag(nth,nd),weight(nth,nd))
    else
       allocate(index(nth,nm),flag(nth,nm),weight(nth,nm))
    end if

    allocate(diag(nth,nm),offd(nth,nm))

    call ptridiag_init(nth,nm)
  end subroutine pctrist_init
  !----------------------------------------------------------------
  subroutine pctrist_define(ith,coord)
    integer,                 intent(in)  :: ith
    real,    dimension(:,:), intent(in)  :: coord
    integer :: id, im, il, iu
    real    :: rd, rm, wl, wu

    diag(ith,:)=0.
    offd(ith,:)=0.

    if(push) then
       !! for every data point
       !! - find the nearest lower index in the model   (index)
       !! - find the weights to the lower/upper indexes (weight)

       !!
       !!        d
       !!        |
       !!        v
       !!....*.......*.......* MODEL
       !!       rd
       !!    il      iu

       do id=1,nd

          rd=1+(coord(ith,id)-om)/dm

          il=rd     !! lower index
          wu=rd-il  !! upper weight (0 if the point is on the grid)

          !! ignore point if out of the grid
          if (il<1 .or. il>nm-1) then
             flag(ith,id) = .true. 
             cycle
          end if

          !! for every point in the data, keep:
          flag  (ith,id) = .false.  !! an 'in bounds' flag
          index (ith,id) = il       !! the lower index
          weight(ith,id) = wu       !! the upper weight

          iu = il + 1
          wl = 1. - wu

          !! create tridiagonal (Lt L)
          diag(ith,il) = diag(ith,il) + wl*wl
          diag(ith,iu) = diag(ith,iu) + wu*wu
          offd(ith,il) = offd(ith,il) + wl*wu

       end do
    else !! pull
       !! for every model point
       !! - find the nearest lower index in the data
       !! - find the weights to the lower/upper indexes (weight)

       !!      il      iu
       !!           rm
       !!......*.......*...... DATA
       !!            ^
       !!            |
       !!            m
       !!

       do im=1,nm
          rm=1+(coord(ith,im)-od)/dd

          il=rm     !! lower index
          wu=rm-il  !! upper weight (0 if the point is on the grid)

          !! ignore point if out of the grid
          if (il<1 .or. il>nd-1) then
             flag(ith,im) = .true. 
             cycle
          end if

          !! for every point in the model, keep:
          flag  (ith,im) = .false.  !! an 'in bounds' flag
          index (ith,im) = il       !! the lower index
          weight(ith,im) = wu       !! the upper weight

          iu = il + 1
          wl = 1. - wu

          !! create tridiagonal (Lt L)
          diag(ith,im) = diag(ith,im) + wl*wl
          diag(ith,im) = diag(ith,im) + wu*wu
       end do
    end if

    !! add regularization: (Lt L) + eps (At A)
    diag(ith,:) = diag(ith,:) + 2.*eps
    offd(ith,:) = offd(ith,:) -    eps

  end subroutine pctrist_define
  !----------------------------------------------------------------
  subroutine pctrist_noinv(ith,coord)
    integer,                 intent(in)  :: ith
    real,    dimension(:,:), intent(in)  :: coord
    integer :: id, im, il, iu
    real    :: rd, rm, wl, wu

    diag(ith,:)=1.
    offd(ith,:)=0.

    if(push) then
       !! for every data point
       !! - find the nearest lower index in the model   (index)
       !! - find the weights to the lower/upper indexes (weight)

       !!
       !!        d
       !!        |
       !!        v
       !!....*.......*.......* MODEL
       !!       rd
       !!    il      iu

       do id=1,nd

          rd=1+(coord(ith,id)-om)/dm

          il=rd     !! lower index
          wu=rd-il  !! upper weight (0 if the point is on the grid)

          !! ignore point if out of the grid
          if (il<1 .or. il>nm-1) then
             flag(ith,id) = .true. 
             cycle
          end if

          !! for every point in the data, keep:
          flag  (ith,id) = .false.  !! an 'in bounds' flag
          index (ith,id) = il       !! the lower index
          weight(ith,id) = wu       !! the upper weight

       end do
    else !! pull
       !! for every model point
       !! - find the nearest lower index in the data
       !! - find the weights to the lower/upper indexes (weight)

       !!      il      iu
       !!           rm
       !!......*.......*...... DATA
       !!            ^
       !!            |
       !!            m
       !!

       do im=1,nm
          rm=1+(coord(ith,im)-od)/dd

          il=rm     !! lower index
          wu=rm-il  !! upper weight (0 if the point is on the grid)

          !! ignore point if out of the grid
          if (il<1 .or. il>nd-1) then
             flag(ith,im) = .true. 
             cycle
          end if

          !! for every point in the model, keep:
          flag  (ith,im) = .false.  !! an 'in bounds' flag
          index (ith,im) = il       !! the lower index
          weight(ith,im) = wu       !! the upper weight

       end do
    end if

    !! add regularization: (Lt L) + eps (At A)
    diag(ith,:) = diag(ith,:) + 2.*eps
    offd(ith,:) = offd(ith,:) -    eps

  end subroutine pctrist_noinv
  !----------------------------------------------------------------
 subroutine pctrist_apply(ith,model,data)
    integer,                 intent(in)  :: ith
    complex, dimension(:,:), intent(out) :: model
    complex, dimension(:,:), intent(in)  :: data 
    integer :: id, im, il, iu
    real    :: wl, wu

    model(ith,:)=0.

    !! compute m=Lt d
    if(push) then
       do id=1,nd
          if(flag(ith,id)) cycle

          il = index (ith,id)  !! lower index
          wu = weight(ith,id)  !! upper weight
          
          iu = il + 1  !! upper index
          wl = 1. - wu !! lower weight

          model(ith,il) = model(ith,il) + wl * data(ith,id)
          model(ith,iu) = model(ith,iu) + wu * data(ith,id)
       end do
    else
       do im=1,nm
          if(flag(ith,im)) cycle
          
          il = index (ith,im)  !! lower index
          wu = weight(ith,im)  !! upper weight
          
          iu = il + 1  !! upper index
          wl = 1. - wu !! lower weight

          model(ith,im) = model(ith,im) + data(ith,il) * wl
          model(ith,im) = model(ith,im) + data(ith,iu) * wu
       end do
    end if

    !! solve (LtL + e^2 AtA) m = Lt d
    call ptridiag(ith,diag,offd,model)

  end subroutine pctrist_apply
  !----------------------------------------------------------------  
  subroutine pctrist_scale(ith,model,data)
    integer,                 intent(in)  :: ith
    complex, dimension(:,:), intent(out) :: model
    complex, dimension(:,:), intent(in)  :: data
    real :: kd,kn,k 

    k=1.0
    !! output energy = input energy
    kn=sqrt(abs(sum( data(ith,:)*conjg( data(ith,:)))))
    kd=sqrt(abs(sum(model(ith,:)*conjg(model(ith,:)))))
    if(kd/=0.) k=kn/kd !! avoid division by 0
    model(ith,:)=k*model(ith,:)

  end subroutine pctrist_scale
  !----------------------------------------------------------------
  subroutine pctrist_close()
    call ptridiag_close()
    deallocate(index,flag,weight,diag,offd)
  end subroutine pctrist_close
  !----------------------------------------------------------------  
end module pctrist_mod
