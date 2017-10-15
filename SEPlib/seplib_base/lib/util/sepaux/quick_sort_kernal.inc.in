integer, parameter :: nn=15,nstack=100
integer :: k,i,j,jstack,l,r
integer,dimension(nstack) :: istack



jstack=0
l=1
r=n2
do {
	if(r-l < nn){
		do j=l+1,r{
			hold=array(:,j)
			do i=j-1,l,-1{
				if(array(key,i) <= hold(key)) exit
				array(:,i+1)=array(:,i)
			}
			array(:,i+1)=hold	
		}
		if(jstack == 0) return
		r=istack(jstack)
		l=istack(jstack-1)
		jstack=jstack-2
	}
	else{
		k=(l+r)/2
		hold=array(:,k); array(:,k)=array(:,l+1); array(:,l+1)=hold
		if(array(key,l)>array(key,r)) {
			hold=array(:,l); array(:,l)=array(:,r); array(:,r)=hold}
		if(array(key,l+1)>array(key,r)) {
			hold=array(:,l+1); array(:,l+1)=array(:,r); array(:,r)=hold}
		if(array(key,l+1)<array(key,l)) {
			hold=array(:,l+1); array(:,l+1)=array(:,l); array(:,l)=hold}
		i=l+1;j=r; hold=array(:,l+1)
		do{
			do{
				i+=1
				if(array(key,i) >= hold(key)) exit
			}
			do{
				j-=1
				if(array(key,j) <= hold(key)) exit
			}
			if(j<i) exit
			hold2=array(:,i); array(:,i)=array(:,j); array(:,j)=hold2
		}
		array(:,l+1)=array(:,j)
		array(:,j) = hold
		jstack+=2
		if(jstack > nstack) 
			call erexit("need to increase size of stack in quick sort")
		if(r-i+1 >= j-1){
			istack(jstack)=r
			istack(jstack-1)=i
			r=j-1
		}
		else{
			istack(jstack)=j-1
			istack(jstack-1)=l
			l=i
		}
	}
}

