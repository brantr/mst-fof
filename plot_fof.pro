fdata = 'fake_shocks.dat'

fs = read_fake_shocks(fdata,n_fs)

fdata = 'fof_list.dat'
nfg = read_fof_catalogue_list(fdata, lg, og, dg, idg)


fdata = 'fof_data.dat'
nfp = read_fof_catalogue_data(fdata,lg,og,ngfp,dfp,xfp,yfp,zfp,vxfp,vyfp,vzfp,lfp)



ss = 0.7


xmin = 0
xmax = 1.
ymin = 0.
ymax = 1.
zmin = 0.
zmax = 1.

test = set_psym_circle()
device,decompose=0
window,1,xsize=600,ysize=600
plot,[0],[0],xrange=[ymin,ymax],yrange=[zmin,zmax],xstyle=1,ystyle=1,xtitle='y',ytitle='z'
oplot,xfp,yfp,psym=8,symsize=ss

end