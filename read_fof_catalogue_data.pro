function read_fof_catalogue_data, fdata, lg, og, ng, d, x, y, z, vx, vy, vz, l

  openr,1,fdata
  ng = 0L
  readu,1,ng
  n = long64(0)
  readu,1,n


  d = fltarr(n)
  x = fltarr(n)
  y = fltarr(n)
  z = fltarr(n)
  vx = fltarr(n)
  vy = fltarr(n)
  vz = fltarr(n)
  l  = lon64arr(n)

  for i=0,ng-1 do begin
    db = fltarr(lg(i))
    lb = lon64arr(lg(i))

    readu,1,db
    d(og(i):og(i)+lg(i)-1) = db(*)
    readu,1,db
    x(og(i):og(i)+lg(i)-1) = db(*)
    readu,1,db
    y(og(i):og(i)+lg(i)-1) = db(*)
    readu,1,db
    z(og(i):og(i)+lg(i)-1) = db(*)
    readu,1,db
    vx(og(i):og(i)+lg(i)-1) = db(*)
    readu,1,db
    vy(og(i):og(i)+lg(i)-1) = db(*)
    readu,1,db
    vz(og(i):og(i)+lg(i)-1) = db(*)
    readu,1,lb
    l(og(i):og(i)+lg(i)-1) = lb
  endfor


  close,1

  return, n

end
