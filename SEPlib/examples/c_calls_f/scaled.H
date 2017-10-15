in="/bob/data/saltf@@"
n1=676 n2=676 n3=210
o1=0.0 o2=0.0 o3=0.0
d1=20.0 d2=20.0 d3=20.0
esize=4


Transp:   bob@kana   Sun Jul  6 10:57:08 1997
		sepstr_ready_out() : sets next in="stdin"
#	Creating work file /scrka1/bob/Transpa001Ft
# Transp: input n1=676  n2=676  n3=210 esize=4
#		o2=0.000000 o3=0.000000
#		d2=20.000000 d3=20.000000
Transp: output n1=676  n2=210  n3=676 esize=4
		o2=0.000000 o3=0.000000
		d2=20.000000 d3=20.000000
#	output tag is "out" 
		data_format="xdr_float"


Transp:   bob@kana   Sun Jul  6 10:57:08 1997
		sets next: in="/scrka1/bob/vel_trans.H@"
#	Creating work file /scrka1/bob/Transpa001FY
# Transp: input n1=676  n2=210  n3=676 esize=4
#		o1=0.000000 o2=0.000000
#		d1=20.000000 d2=20.000000
Transp: output n1=210  n2=676  n3=676 esize=4
		o1=0.000000 o2=0.000000
		d1=20.000000 d2=20.000000
#	output tag is "out" 
		data_format="xdr_float"



/bob/bin/SGI64/spike.x:   bob@kana   Sun Jul  6 11:32:49 1997
		sets next: in="/scrka1/bob/junk1.H@"
		From history: d1=20
		From history: n1=210
		From history: n2=676
		From history: n3=676
		From history: esize=4
		From par: max_search=3
#	output tag is "out" 
		data_format="xdr_float"


/bob/bin/SGI64/despike2.x:   bob@kana   Sun Jul  6 11:35:16 1997
		sets next: in="/scrka1/bob/despike.H@"
		From history: n1=210
		From history: n2=676
		From history: n3=676
		From par: size=3
		From par: invert=1
#	output tag is "out" 
		data_format="xdr_float"


/bob/bin/SGI64/background.x:   bob@kana   Sun Jul  6 16:18:10 1997
		sets next: in="/scrka1/bob/background.H@"
		From par: low2=80
		From par: low3=100
		From par: high2=570
		From par: high3=620
		From history: n1=210
		From history: n2=676
		From history: n3=676
		n1=2
		n2=5
		n3=21
#	output tag is "out" 
		data_format="xdr_float"


Cp:   bob@kana   Mon Jul  7 14:37:43 1997
		sepstr_ready_out() : sets next in="stdin"
#	output tag is "out" 
		data_format="xdr_float"



./scale_it.x:   bob@kana   Mon Oct 25 11:05:48 1999
		sets next: in="/scrka3/bob/scaled.H@"
		scale=1000
		data_format="xdr_float"

