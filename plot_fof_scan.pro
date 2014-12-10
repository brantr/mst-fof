fdata = 'fof_scan.txt'
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

end