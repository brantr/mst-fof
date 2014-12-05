ss = 0.7

fdata = "mst.0000.dat"
openr,1,fdata
n= long64(0)
readu,1,n
x= fltarr(3,n)
y= fltarr(3,n)
dis= fltarr(n)
eid= lon64arr(n)
idxA= lon64arr(n)
idxB= lon64arr(n)
readu,1,x
readu,1,y
readu,1,dis
readu,1,eid
readu,1,idxA
readu,1,idxB
close,1
print,'nedges = ',n

ngroups = 1
ei = sort(eid)
for i=1,n-1 do begin
  if eid(ei(i)) ne eid(ei(i-1)) then ngroups = ngroups+1
endfor
print,'ngroups = ',ngroups

group = lonarr(ngroups)
gsize = lonarr(ngroups)
grank = lonarr(n)
k = 0

group(k) = eid(ei(0))
gsize(k) = 1
grank(ei(0)) = k
for i=1,n-1 do begin
  if eid(ei(i)) ne eid(ei(i-1)) then begin
    k = k+1
    group(k) = eid(ei(i))
  endif
  gsize(k) = gsize(k) + 1
  grank(ei(i)) = k
endfor

fdata = "st.0000.dat"
openr,1,fdata
nst = long64(0)
readu,1,nst
xst= fltarr(3,nst)
yst= fltarr(3,nst)
disst= fltarr(nst)
eidst= lon64arr(nst)
idxAst= lon64arr(nst)
idxBst= lon64arr(nst)
readu,1,xst
readu,1,yst
readu,1,disst
readu,1,eidst
readu,1,idxAst
readu,1,idxBst
close,1

xmin = 1.1*min(y(0,*))
xmax = 1.1*max(y(0,*))
if abs(xmin) gt xmax then begin
  xmax = -1.0*xmin
endif else begin
  if xmin lt 0 then begin
    xmin = -1.0*xmax
  endif
endelse

ymin = 1.1*min(y(1,*))
ymax = 1.1*max(y(1,*))
if abs(ymin) gt ymax then begin
  ymax = -1.0*ymin
endif else begin
  if ymin lt 0 then begin
    ymin = -1.0*ymax
  endif
endelse

if xmax gt ymax then begin
  ymin = xmin
  ymax = xmax
endif else begin
  xmin = ymin
  xmax = ymax
endelse

zmin = 1.1*min(y(2,*))
zmax = 1.1*max(y(2,*))
if abs(zmin) gt zmax then begin
  zmax = -1.0*zmin
endif else begin
  if zmin lt 0 then begin
    zmin = -1.0*zmax
  endif
endelse

if xmax gt zmax then begin
  zmin = xmin
  zmax = xmax
endif else begin
  xmin = zmin
  xmax = zmax
  ymin = zmin
  ymax = zmax
endelse

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
xx = fltarr(2)
yy = fltarr(2)
for i=0,nst-1 do begin
  xx(0) = xst(1,i)
  xx(1) = yst(1,i)
  yy(0) = xst(2,i)
  yy(1) = yst(2,i)
  ;oplot,xx,yy
endfor
loadct,13,/silent
for i=0,n-1 do begin
	xx(0) = x(1,i)
	xx(1) = y(1,i)
	yy(0) = x(2,i)
	yy(1) = y(2,i)
	;oplot,xx,yy,color = (255.-40.)*float(i)/float(n-1)+40., thick=3
  if ngroups gt 1 then begin
    cc =  (255.-40.)*(grank(i)-min(grank))/(max(grank)-min(grank))+40.
  endif else begin
    cc = 255
  endelse
  oplot,xx,yy,color =cc, thick=3

endfor
loadct,0,/silent
oplot,x(1,*),x(2,*),psym=8,symsize=ss
oplot,y(1,*),y(2,*),psym=8,symsize=ss


window,0,xsize=600,ysize=600
plot,[0],[0],xrange=[xmin,xmax],yrange=[ymin,ymax],xstyle=1,ystyle=1,xtitle='x',ytitle='y'
xx = fltarr(2)
yy = fltarr(2)
for i=0,nst-1 do begin
  xx(0) = xst(0,i)
  xx(1) = yst(0,i)
  yy(0) = xst(1,i)
  yy(1) = yst(1,i)
  ;oplot,xx,yy
endfor
loadct,13,/silent
for i=0,n-1 do begin
  xx(0) = x(0,i)
  xx(1) = y(0,i)
  yy(0) = x(1,i)
  yy(1) = y(1,i)
  ;oplot,xx,yy,color = (255.-40.)*float(i)/float(n-1)+40., thick=3
  if ngroups gt 1 then begin
    cc =  (255.-40.)*(grank(i)-min(grank))/(max(grank)-min(grank))+40.
  endif else begin
    cc = 255
  endelse
  oplot,xx,yy,color =cc, thick=3

endfor
loadct,0,/silent
oplot,x(0,*),x(1,*),psym=8,symsize=ss
oplot,y(0,*),y(1,*),psym=8,symsize=ss


window,2,xsize=600,ysize=600
plot,[0],[0],xrange=[xmin,xmax],yrange=[zmin,zmax],xstyle=1,ystyle=1,xtitle='x',ytitle='z'
xx = fltarr(2)
yy = fltarr(2)
for i=0,nst-1 do begin
  xx(0) = xst(0,i)
  xx(1) = yst(0,i)
  yy(0) = xst(2,i)
  yy(1) = yst(2,i)
  ;oplot,xx,yy
endfor
loadct,13,/silent
for i=0,n-1 do begin
  xx(0) = x(0,i)
  xx(1) = y(0,i)
  yy(0) = x(2,i)
  yy(1) = y(2,i)
  ;oplot,xx,yy,color = (255.-40.)*float(i)/float(n-1)+40., thick=3
  if ngroups gt 1 then begin
    cc =  (255.-40.)*(grank(i)-min(grank))/(max(grank)-min(grank))+40.
  endif else begin
    cc = 255
  endelse
  oplot,xx,yy,color =cc, thick=3

endfor
loadct,0,/silent
oplot,x(0,*),x(2,*),psym=8,symsize=ss
oplot,y(0,*),y(2,*),psym=8,symsize=ss

;for i=0,n-1 do begin
;  print,idxA(i),idxB(i)
;endfor
end