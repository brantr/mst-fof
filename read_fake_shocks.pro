function read_fake_shocks, fdata, n

  openr,1,fdata
  n = 0L
  readu,1,n
  fs = replicate({id:0L, n:0L, x:fltarr(3), theta:fltarr(3), Sigma:0.0, d:0.0, h:0.0, dmax:0.0, idmax:0L},n)

  for i=0,n-1 do begin
    id = 0L
    ni = 0L
    x = fltarr(3)
    t = fltarr(3)
    sigma = 0.0
    d =0.0
    h = 0.0
    dmax = 0.0
    idmax = 0L


    readu,1,id
    readu,1,ni
    readu,1,x
    readu,1,t
    readu,1,sigma
    readu,1,d
    readu,1,h
    readu,1,dmax
    readu,1,idmax

    fs(i).id = id
    fs(i).n  = ni
    fs(i).x  = x
    fs(i).theta = t
    fs(i).Sigma = sigma
    fs(i).d     = d
    fs(i).h     = h
    fs(i).dmax  = dmax
    fs(i).idmax = idmax


  endfor


  ;int id;
  ;int n;
  ;float x[3];
  ;float theta[3];
  ;float Sigma;
  ;float d;
  ;float h;
  ;float dmax;
  ;int idmax;

  close,1

  return,fs

end