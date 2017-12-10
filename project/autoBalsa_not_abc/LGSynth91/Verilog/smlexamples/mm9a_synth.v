// IWLS benchmark module "Min_Max9_4" printed on Wed May 29 22:12:27 2002
module Min_Max9_4(\1 , \2 , \3 , \4 , \5 , \6 , \7 , \8 , \9 , \10 , \11 , \12 , \40 , \41 , \42 , \43 , \44 , \45 , \46 , \47 , \48 );
input
  \1 ,
  \2 ,
  \3 ,
  \4 ,
  \5 ,
  \6 ,
  \7 ,
  \8 ,
  \9 ,
  \10 ,
  \11 ,
  \12 ;
output
  \40 ,
  \41 ,
  \42 ,
  \43 ,
  \44 ,
  \45 ,
  \46 ,
  \47 ,
  \48 ;
reg
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
wire
  \405 ,
  \413 ,
  \[45] ,
  \421 ,
  \[70] ,
  \[46] ,
  \426 ,
  \431 ,
  \[71] ,
  \[47] ,
  \436 ,
  \441 ,
  \[72] ,
  \[48] ,
  \446 ,
  \451 ,
  \[49] ,
  \456 ,
  \461 ,
  \466 ,
  \[75] ,
  \[76] ,
  \[50] ,
  \[51] ,
  \[52] ,
  \58 ,
  \59 ,
  \[53] ,
  \60 ,
  \61 ,
  \62 ,
  \63 ,
  \64 ,
  \65 ,
  \66 ,
  \[54] ,
  \[55] ,
  \83 ,
  \85 ,
  \87 ,
  \89 ,
  \[56] ,
  \91 ,
  \93 ,
  \97 ,
  \[57] ,
  \[58] ,
  \546 ,
  \[59] ,
  \554 ,
  \562 ,
  \570 ,
  \[85] ,
  \578 ,
  \[86] ,
  \586 ,
  \594 ,
  \322 ,
  \[60] ,
  \323 ,
  \[36] ,
  \327 ,
  \328 ,
  \329 ,
  \330 ,
  \331 ,
  \332 ,
  \[61] ,
  \333 ,
  \[37] ,
  \334 ,
  \335 ,
  \336 ,
  \337 ,
  \338 ,
  \339 ,
  \340 ,
  \341 ,
  \342 ,
  \[62] ,
  \343 ,
  \[38] ,
  \344 ,
  \345 ,
  \346 ,
  \347 ,
  \348 ,
  \350 ,
  \[63] ,
  \[39] ,
  \357 ,
  \602 ,
  \[64] ,
  \365 ,
  \610 ,
  \373 ,
  \381 ,
  \389 ,
  \397 ,
  \103 ,
  \105 ,
  \107 ,
  \109 ,
  \111 ,
  \[69] ,
  \113 ,
  \117 ,
  \[40] ,
  \[41] ,
  \[42] ,
  \[43] ,
  \[44] ;
assign
  \405  = (~\[64]  & (\347  & ~\348 )) | ((\602  & (\347  & \345 )) | ((\345  & (~\348  & ~\65 )) | ((\[63]  & \456 ) | (\602  & ~\348 )))),
  \413  = (~\[64]  & (~\610  & (\350  & ~\348 ))) | ((~\610  & (\348  & ~\66 )) | ((\610  & (\350  & \348 )) | ((\610  & (~\348  & ~\66 )) | (\[63]  & \461 )))),
  \[45]  = \58 ,
  \421  = (~\[64]  & (\350  & \348 )) | ((\[63]  & \466 ) | ((\610  & \350 ) | (\610  & \348 ))),
  \[70]  = \2  & ~\1 ,
  \[46]  = \59 ,
  \426  = (\[70]  & \4 ) | (\[69]  & \13 ),
  \431  = (\[70]  & \5 ) | (\[69]  & \14 ),
  \[71]  = ~\[64]  & ~\322 ,
  \[47]  = \60 ,
  \436  = (\[70]  & \6 ) | (\[69]  & \15 ),
  \441  = (\[70]  & \7 ) | (\[69]  & \16 ),
  \[72]  = ~\[64]  & \322 ,
  \[48]  = \61 ,
  \446  = (\[70]  & \8 ) | (\[69]  & \17 ),
  \451  = (\[70]  & \9 ) | (\[69]  & \18 ),
  \[49]  = \62 ,
  \456  = (\[70]  & \10 ) | (\[69]  & \19 ),
  \461  = (\[70]  & \11 ) | (\[69]  & \20 ),
  \466  = (\[70]  & \12 ) | (\[69]  & \21 ),
  \[75]  = ~\[64]  & ~\323 ,
  \[76]  = ~\[64]  & \323 ,
  \[50]  = \63 ,
  \[51]  = \64 ,
  \40  = \357 ,
  \41  = \365 ,
  \42  = \373 ,
  \43  = \381 ,
  \44  = \389 ,
  \45  = \397 ,
  \46  = \405 ,
  \47  = \413 ,
  \48  = \421 ,
  \[52]  = \65 ,
  \58  = (~\323  & \4 ) | ((\323  & \22 ) | \[64] ),
  \59  = \[64]  | \329 ,
  \[53]  = \66 ,
  \60  = \[64]  | \332 ,
  \61  = \[64]  | \335 ,
  \62  = \[64]  | \338 ,
  \63  = \[64]  | \341 ,
  \64  = \[64]  | \344 ,
  \65  = \[64]  | \347 ,
  \66  = \[64]  | \350 ,
  \[54]  = \546 ,
  \[55]  = \554 ,
  \83  = (~\12  & \39 ) | (\12  & ~\39 ),
  \85  = (~\11  & \38 ) | (\11  & ~\38 ),
  \87  = (~\10  & \37 ) | (\10  & ~\37 ),
  \89  = (~\9  & \36 ) | (\9  & ~\36 ),
  \[56]  = \562 ,
  \91  = (~\8  & \35 ) | (\8  & ~\35 ),
  \93  = (~\7  & \34 ) | (\7  & ~\34 ),
  \97  = (~\5  & \32 ) | (\5  & ~\32 ),
  \[57]  = \570 ,
  \[58]  = \578 ,
  \546  = (\[72]  & \4 ) | (\[71]  & \31 ),
  \[59]  = \586 ,
  \554  = ~\[64]  & \328 ,
  \562  = ~\[64]  & \331 ,
  \570  = ~\[64]  & \334 ,
  \[85]  = (~\97  & (~\93  & (~\91  & (~\89  & (~\87  & (~\85  & (~\33  & (~\31  & (~\6  & \4 ))))))))) | ((~\97  & (~\93  & (~\91  & (~\89  & (~\87  & (~\85  & (\33  & (~\31  & (\6  & \4 ))))))))) | ((\97  & (~\93  & (~\91  & (~\89  & (~\87  & (~\85  & (~\33  & (~\6  & \5 )))))))) | ((\97  & (~\93  & (~\91  & (~\89  & (~\87  & (~\85  & (\33  & (\6  & \5 )))))))) | ((~\93  & (~\91  & (~\89  & (~\87  & (~\85  & (~\33  & \6 )))))) | ((\93  & (~\91  & (~\89  & (~\87  & (~\85  & \7 ))))) | ((\91  & (~\89  & (~\87  & (~\85  & \8 )))) | ((\89  & (~\87  & (~\85  & \9 ))) | ((\87  & (~\85  & \10 )) | (\85  & \11 ))))))))),
  \578  = ~\[64]  & \337 ,
  \[86]  = (~\117  & (~\113  & (~\111  & (~\109  & (~\107  & (~\105  & (~\24  & (~\22  & (~\6  & \4 ))))))))) | ((~\117  & (~\113  & (~\111  & (~\109  & (~\107  & (~\105  & (\24  & (~\22  & (\6  & \4 ))))))))) | ((\117  & (~\113  & (~\111  & (~\109  & (~\107  & (~\105  & (~\24  & (~\6  & \5 )))))))) | ((\117  & (~\113  & (~\111  & (~\109  & (~\107  & (~\105  & (\24  & (\6  & \5 )))))))) | ((~\113  & (~\111  & (~\109  & (~\107  & (~\105  & (~\24  & \6 )))))) | ((\113  & (~\111  & (~\109  & (~\107  & (~\105  & \7 ))))) | ((\111  & (~\109  & (~\107  & (~\105  & \8 )))) | ((\109  & (~\107  & (~\105  & \9 ))) | ((\107  & (~\105  & \10 )) | (\105  & \11 ))))))))),
  \586  = ~\[64]  & \340 ,
  \594  = ~\[64]  & \343 ,
  \322  = (\[85]  & (~\[64]  & ~\83 )) | (~\[64]  & (\83  & \12 )),
  \[60]  = \594 ,
  \323  = (\[86]  & (~\[64]  & ~\103 )) | (~\[64]  & (\103  & \12 )),
  \[36]  = \426 ,
  \327  = \546  & \58 ,
  \328  = (\[72]  & \5 ) | (\[71]  & \32 ),
  \329  = (\[76]  & \23 ) | (\[75]  & \5 ),
  \330  = (\329  & \328 ) | ((\329  & \327 ) | (\328  & \327 )),
  \331  = (\[72]  & \6 ) | (\[71]  & \33 ),
  \332  = (\[76]  & \24 ) | (\[75]  & \6 ),
  \[61]  = \602 ,
  \333  = (\332  & \331 ) | ((\332  & \330 ) | (\331  & \330 )),
  \[37]  = \431 ,
  \334  = (\[72]  & \7 ) | (\[71]  & \34 ),
  \335  = (\[76]  & \25 ) | (\[75]  & \7 ),
  \336  = (\335  & \334 ) | ((\335  & \333 ) | (\334  & \333 )),
  \337  = (\[72]  & \8 ) | (\[71]  & \35 ),
  \338  = (\[76]  & \26 ) | (\[75]  & \8 ),
  \339  = (\338  & \337 ) | ((\338  & \336 ) | (\337  & \336 )),
  \340  = (\[72]  & \9 ) | (\[71]  & \36 ),
  \341  = (\[76]  & \27 ) | (\[75]  & \9 ),
  \342  = (\341  & \340 ) | ((\341  & \339 ) | (\340  & \339 )),
  \[62]  = \610 ,
  \343  = (\[72]  & \10 ) | (\[71]  & \37 ),
  \[38]  = \436 ,
  \344  = (\[76]  & \28 ) | (\[75]  & \10 ),
  \345  = (\344  & \343 ) | ((\344  & \342 ) | (\343  & \342 )),
  \346  = (\[72]  & \11 ) | (\[71]  & \38 ),
  \347  = (\[76]  & \29 ) | (\[75]  & \11 ),
  \348  = (\347  & \346 ) | ((\347  & \345 ) | (\346  & \345 )),
  \350  = (\[76]  & \30 ) | (\[75]  & \12 ),
  \[63]  = \3  | ~\2 ,
  \[39]  = \441 ,
  \357  = (~\[64]  & (~\330  & \329 )) | ((\554  & (\329  & \327 )) | ((~\330  & (\327  & ~\59 )) | ((\[63]  & \426 ) | (\554  & ~\330 )))),
  \602  = ~\[64]  & \346 ,
  \[64]  = \[63]  | \1 ,
  \365  = (~\[64]  & (~\333  & \332 )) | ((\562  & (\332  & \330 )) | ((~\333  & (\330  & ~\60 )) | ((\[63]  & \431 ) | (\562  & ~\333 )))),
  \610  = (\[72]  & \12 ) | (\[71]  & \39 ),
  \373  = (~\[64]  & (~\336  & \335 )) | ((\570  & (\335  & \333 )) | ((~\336  & (\333  & ~\61 )) | ((\[63]  & \436 ) | (\570  & ~\336 )))),
  \381  = (~\[64]  & (~\339  & \338 )) | ((\578  & (\338  & \336 )) | ((~\339  & (\336  & ~\62 )) | ((\[63]  & \441 ) | (\578  & ~\339 )))),
  \389  = (~\[64]  & (~\342  & \341 )) | ((\586  & (\341  & \339 )) | ((~\342  & (\339  & ~\63 )) | ((\[63]  & \446 ) | (\586  & ~\342 )))),
  \397  = (~\[64]  & (~\345  & \344 )) | ((\594  & (\344  & \342 )) | ((~\345  & (\342  & ~\64 )) | ((\[63]  & \451 ) | (\594  & ~\345 )))),
  \103  = (~\12  & \30 ) | (\12  & ~\30 ),
  \105  = (~\11  & \29 ) | (\11  & ~\29 ),
  \107  = (~\10  & \28 ) | (\10  & ~\28 ),
  \109  = (~\9  & \27 ) | (\9  & ~\27 ),
  \111  = (~\8  & \26 ) | (\8  & ~\26 ),
  \[69]  = ~\2  & ~\1 ,
  \113  = (~\7  & \25 ) | (\7  & ~\25 ),
  \117  = (~\5  & \23 ) | (\5  & ~\23 ),
  \[40]  = \446 ,
  \[41]  = \451 ,
  \[42]  = \456 ,
  \[43]  = \461 ,
  \[44]  = \466 ;
always begin
  \13  = \[36] ;
  \14  = \[37] ;
  \15  = \[38] ;
  \16  = \[39] ;
  \17  = \[40] ;
  \18  = \[41] ;
  \19  = \[42] ;
  \20  = \[43] ;
  \21  = \[44] ;
  \22  = \[45] ;
  \23  = \[46] ;
  \24  = \[47] ;
  \25  = \[48] ;
  \26  = \[49] ;
  \27  = \[50] ;
  \28  = \[51] ;
  \29  = \[52] ;
  \30  = \[53] ;
  \31  = \[54] ;
  \32  = \[55] ;
  \33  = \[56] ;
  \34  = \[57] ;
  \35  = \[58] ;
  \36  = \[59] ;
  \37  = \[60] ;
  \38  = \[61] ;
  \39  = \[62] ;
end
initial begin
  \22  = 1;
  \23  = 1;
  \24  = 1;
  \25  = 1;
  \26  = 1;
  \27  = 1;
  \28  = 1;
  \29  = 1;
  \30  = 1;
  \31  = 0;
  \32  = 0;
  \33  = 0;
  \34  = 0;
  \35  = 0;
  \36  = 0;
  \37  = 0;
  \38  = 0;
  \39  = 0;
end
endmodule
