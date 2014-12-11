;fdata = 'fof_scan.A.txt'
;fdata = 'fof_scan.B.txt'
;fdata = 'fof_scan.C.txt'
fdata = 'fof_scan.D.txt'
;fdata = 'fof_scan.F.txt'

openr,1,fdata
n = 0L
readf,1,n
bsq = fltarr(n)
ng  = fltarr(n)
nt = fltarr(n)
ngr = fltarr(n)
ntr = fltarr(n)
for i=0,n-1 do begin
  bb = 0.0
  ngb = 0L
  ntb = 0L
  ngrb = 0L
  ntrb = 0L
  readf,1,bb
  bsq(i) = bb
  readf,1,ngb,ntb
  ng(i) = ngb
  nt(i) = ntb
  readf,1,ngrb,ntrb
  ngr(i) =ngrb
  ntr(i) = ntrb
endfor
close,1

bi = sort(bsq)
ui = uniq(bsq(bi))

bsort = bsq(bi(ui))
nbs = n_elements(bsort)
print,nbs

nt_min = fltarr(nbs)
nt_med = fltarr(nbs)
nt_max = fltarr(nbs)

ng_min = fltarr(nbs)
ng_med = fltarr(nbs)
ng_max = fltarr(nbs)

for i=0,nbs-1 do begin
  xi = where(bsq eq bsort(i))
  nt_min(i) = min(ntr(xi)/nt(xi))
  nt_max(i) = max(ntr(xi)/nt(xi))
  nt_med(i) = median(ntr(xi)/nt(xi))
  ng_min(i) = min(ngr(xi)/ng(xi))
  ng_max(i) = max(ngr(xi)/ng(xi))
  ng_med(i) = median(ngr(xi)/ng(xi))
endfor

fi = where(ng_min eq 0, nfi)
if nfi gt 0 then ng_min(fi) = 1.0e-3
fi = where(nt_min eq 0, nfi)
if nfi gt 0 then nt_min(fi) = 1.0e-3

window,2,xsize=600,ysize=600
plot,sqrt(bsort),nt_min,/ylog,yrange=[1.0e-2,10],ystyle=1,/xlog
oplot,sqrt(bsort),nt_max
oplot,sqrt(bsort),nt_med,linestyle=2
oplot,sqrt(bsort),ng_min
oplot,sqrt(bsort),ng_max
oplot,sqrt(bsort),ng_med,linestyle=2

end