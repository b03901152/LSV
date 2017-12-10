// IWLS benchmark module "lif/9symml" printed on Wed May 29 16:25:53 2002
module \lif/9symml (\1 , \2 , \3 , \4 , \5 , \6 , \7 , \8 , \9 , \52 );
input
  \1 ,
  \2 ,
  \3 ,
  \4 ,
  \5 ,
  \6 ,
  \7 ,
  \8 ,
  \9 ;
output
  \52 ;
wire
  \40 ,
  \41 ,
  \42 ,
  \43 ,
  \44 ,
  \45 ,
  \46 ,
  \47 ,
  \48 ,
  \49 ,
  \50 ,
  \51 ,
  \[1] ,
  \10 ,
  \11 ,
  \12 ,
  \13 ,
  \14 ,
  \15 ,
  \16 ,
  \17 ,
  \18 ,
  \19 ,
  \20 ,
  \21 ,
  \22 ,
  \23 ,
  \24 ,
  \25 ,
  \26 ,
  \27 ,
  \28 ,
  \29 ,
  \30 ,
  \31 ,
  \32 ,
  \33 ,
  \34 ,
  \35 ,
  \36 ,
  \37 ,
  \38 ,
  \39 ;
assign
  \40  = \11  | \10 ,
  \41  = (\9  & (\7  & (~\3  & ~\2 ))) | ((\30  & (~\6  & \3 )) | (\23  & ~\9 )),
  \42  = (\25  & ~\2 ) | \17 ,
  \43  = (~\8  & (~\7  & (\6  & \4 ))) | ((\40  & (\9  & ~\3 )) | (\15  & \10 )),
  \44  = (\9  & (~\8  & \7 )) | (~\7  & (\4  & \2 )),
  \45  = (\18  & (~\6  & (\5  & \4 ))) | ((\14  & (\6  & (~\5  & \3 ))) | (\43  & \1 )),
  \46  = (~\8  & (\2  & \1 )) | (\8  & (~\6  & \4 )),
  \47  = (\20  & (\7  & (~\5  & \4 ))) | ((\46  & (~\7  & \3 )) | ((\42  & (\6  & \1 )) | (\39  & \5 ))),
  \48  = (\12  & (\6  & (~\5  & ~\1 ))) | ((\38  & ~\3 ) | (\34  & ~\6 )),
  \49  = (\35  & (~\8  & (\6  & \4 ))) | ((\44  & (~\5  & \3 )) | (\41  & \8 )),
  \50  = (\17  & (\8  & \1 )) | ((\37  & \3 ) | (\36  & \7 )),
  \51  = (\24  & (~\8  & ~\7 )) | ((\11  & (\10  & ~\6 )) | (\26  & \6 )),
  \52  = \[1] ,
  \[1]  = (\29  & (\11  & (~\6  & \1 ))) | ((\51  & \9 ) | ((\50  & ~\4 ) | ((\49  & ~\1 ) | ((\48  & \2 ) | ((\47  & ~\9 ) | (\45  & ~\2 )))))),
  \10  = (~\8  & \7 ) | (\8  & ~\7 ),
  \11  = (~\5  & \4 ) | (\5  & ~\4 ),
  \12  = (~\4  & \3 ) | (\4  & ~\3 ),
  \13  = (\9  & ~\8 ) | (\8  & ~\7 ),
  \14  = (~\4  & \1 ) | (\4  & ~\1 ),
  \15  = (~\6  & \5 ) | (~\5  & \4 ),
  \16  = (\8  & ~\1 ) | (~\5  & \1 ),
  \17  = (~\7  & (~\5  & \2 )) | (\7  & (~\3  & ~\2 )),
  \18  = (~\9  & \3 ) | ((\9  & ~\1 ) | (~\3  & \1 )),
  \19  = (~\7  & \4 ) | ((\7  & ~\2 ) | (~\5  & \3 )),
  \20  = (~\8  & \6 ) | ((\8  & ~\2 ) | (~\6  & \3 )),
  \21  = (~\8  & \7 ) | ((~\7  & \6 ) | (~\6  & \2 )),
  \22  = (~\7  & \5 ) | ((\7  & ~\5 ) | (~\6  & \4 )),
  \23  = (\7  & (~\4  & \3 )) | ((\5  & (\4  & ~\3 )) | (\19  & \6 )),
  \24  = (\6  & (\5  & ~\1 )) | ((\15  & \3 ) | (\12  & \2 )),
  \25  = (\8  & ~\5 ) | (\5  & ~\4 ),
  \26  = (~\8  & (~\5  & (~\4  & \3 ))) | ((\14  & (~\7  & ~\3 )) | (\10  & (~\2  & ~\1 ))),
  \27  = (~\6  & \4 ) | (\6  & ~\3 ),
  \28  = (~\8  & \3 ) | (\7  & ~\3 ),
  \29  = (~\3  & \2 ) | (\3  & ~\2 ),
  \30  = (\9  & ~\5 ) | (~\7  & \5 ),
  \31  = (~\8  & \7 ) | (~\7  & \5 ),
  \32  = (\8  & ~\4 ) | (\4  & ~\1 ),
  \33  = (~\5  & \3 ) | (~\3  & \2 ),
  \34  = (\7  & (~\5  & (~\4  & \1 ))) | ((\31  & (\9  & ~\3 )) | (\28  & (\4  & ~\1 ))),
  \35  = (~\9  & \3 ) | (\5  & ~\3 ),
  \36  = (~\8  & (\5  & (~\3  & \2 ))) | ((\33  & (~\9  & \6 )) | (\16  & (\3  & ~\2 ))),
  \37  = (\21  & (\5  & ~\1 )) | ((\13  & (~\2  & \1 )) | (\10  & (~\9  & \2 ))),
  \38  = (\22  & (~\9  & \8 )) | ((\16  & (\9  & ~\4 )) | (\13  & (\6  & ~\5 ))),
  \39  = (\32  & (\7  & ~\6 )) | ((\27  & (~\8  & \2 )) | (\10  & (\6  & ~\2 )));
endmodule

