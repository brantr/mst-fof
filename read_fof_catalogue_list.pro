function read_fof_catalogue_list, fdata, l, o, d, id

  openr,1,fdata
  n = 0L
  readu,1,n
  l = lon64arr(n)
  o = lon64arr(n)
  d = fltarr(n)
  id = lon64arr(n)
  readu,1,l
  readu,1,o
  readu,1,d
  readu,1,id
  close,1

  return, n

end