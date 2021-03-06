// IWLS benchmark module "MinMax4" printed on Wed May 29 22:12:22 2002
module MinMax4(a, b, c, d, e, f, g, t, u, v, w);
input
  a,
  b,
  c,
  d,
  e,
  f,
  g;
output
  t,
  u,
  v,
  w;
reg
  h,
  i,
  j,
  k,
  l,
  m,
  n,
  o,
  p,
  q,
  r,
  s;
wire
  \[37] ,
  \[25] ,
  \[13] ,
  \[38] ,
  \[26] ,
  e13,
  \[14] ,
  \[39] ,
  \[27] ,
  \[15] ,
  \[16] ,
  o5,
  \[17] ,
  p1,
  \[18] ,
  \[19] ,
  \x ,
  y,
  z,
  a0,
  a2,
  \[40] ,
  b0,
  t5,
  \[41] ,
  c0,
  c5,
  d0,
  d5,
  e0,
  x4,
  f0,
  \[32] ,
  f5,
  \[20] ,
  \[45] ,
  y4,
  g0,
  g5,
  \[21] ,
  \[46] ,
  z4,
  h0,
  h5,
  \[22] ,
  i0,
  \[23] ,
  \[24] ,
  \[12] ;
assign
  \[37]  = p1,
  \[25]  = g0,
  \[13]  = (f5 & (h5 & h0)) | ((\[41]  & i) | ((~x4 & f5) | ((~x4 & h5) | ((~x4 & h0) | (y & c))))),
  \[38]  = ~p1 & ~e13,
  \[26]  = h0,
  e13 = \[32]  | c,
  \[14]  = (y4 & (~x4 & ~e0)) | ((\[41]  & j) | ((~y4 & x4) | ((x4 & z4) | (z & c)))),
  \[39]  = a2,
  \[27]  = i0,
  \[15]  = (\[41]  & k) | ((x4 & i0) | (e0 & a0)),
  \[16]  = \x ,
  o5 = (\[38]  & p) | (\[37]  & \x ),
  \[17]  = y,
  p1 = (\[45]  & (~e13 & ~s)) | ((\[45]  & (~e13 & a0)) | (~e13 & (~s & a0))),
  \[18]  = z,
  \[19]  = a0,
  t = \[12] ,
  u = \[13] ,
  v = \[14] ,
  w = \[15] ,
  \x  = ~\[32]  & d,
  y = ~\[32]  & e,
  z = ~\[32]  & f,
  a0 = ~\[32]  & g,
  a2 = (\[46]  & (~e13 & ~o)) | ((\[46]  & (~e13 & a0)) | (~e13 & (~o & a0))),
  \[40]  = ~a2 & ~c,
  b0 = e13 | t5,
  t5 = (\[40]  & \x ) | (\[39]  & l),
  \[41]  = ~b & ~a,
  c0 = d5 | e13,
  c5 = (\[38]  & q) | (\[37]  & y),
  d0 = h5 | e13,
  d5 = (\[40]  & y) | (\[39]  & m),
  e0 = z4 | e13,
  x4 = (h5 & g5) | ((h5 & f5) | (g5 & f5)),
  f0 = o5,
  \[32]  = ~b | a,
  f5 = (o5 & (c5 & t5)) | ((o5 & (d5 & t5)) | (c5 & d5)),
  \[20]  = b0,
  \[45]  = (~r & (~q & (~p & \x ))) | ((~r & (~p & (y & \x ))) | ((~q & (~p & (z & \x ))) | ((~p & (z & (y & \x ))) | ((~r & (~q & y)) | ((~q & (z & y)) | (~r & z)))))),
  y4 = (\[38]  & s) | (\[37]  & a0),
  g0 = c5,
  g5 = (\[38]  & r) | (\[37]  & z),
  \[21]  = c0,
  \[46]  = (~n & (~m & (~l & \x ))) | ((~n & (~l & (y & \x ))) | ((~m & (~l & (z & \x ))) | ((~l & (z & (y & \x ))) | ((~n & (~m & y)) | ((~m & (z & y)) | (~n & z)))))),
  z4 = (\[40]  & a0) | (\[39]  & o),
  h0 = g5,
  h5 = (\[40]  & z) | (\[39]  & n),
  \[22]  = d0,
  i0 = y4,
  \[23]  = e0,
  \[24]  = f0,
  \[12]  = (o5 & (d5 & (t5 & g0))) | ((~f5 & (t5 & f0)) | ((\[41]  & h) | ((~f5 & d5) | ((~f5 & g0) | (\x  & c)))));
always begin
  h = \[16] ;
  i = \[17] ;
  j = \[18] ;
  k = \[19] ;
  l = \[20] ;
  m = \[21] ;
  n = \[22] ;
  o = \[23] ;
  p = \[24] ;
  q = \[25] ;
  r = \[26] ;
  s = \[27] ;
end
initial begin
  l = 1;
  m = 1;
  n = 1;
  o = 1;
  p = 0;
  q = 0;
  r = 0;
  s = 0;
end
endmodule

