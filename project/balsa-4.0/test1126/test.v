/*
    `test.v'
    Balsa Verilog netlist file
    Created: Tue Nov 28 12:33:38 CST 2017
    By: yoyo@yoyoChen (Linux)
    With net-verilog (balsa-netlist) version: 4.0
    Using technology: example/four_b_rb
    Command line : (balsa-netlist test.breeze)

    Using `propagate-globals'
    The design contains the following global nets
		global-signal:  initialise input 1
*/

`timescale 1ns/1ps

module BrzBinaryFunc_8_8_8_s3_And_s5_false_s5_fal_m2m (
  out_0r, out_0a, out_0d,
  inpA_0r, inpA_0a, inpA_0d,
  inpB_0r, inpB_0a, inpB_0d
);
  input out_0r;
  output out_0a;
  output [7:0] out_0d;
  output inpA_0r;
  input inpA_0a;
  input [7:0] inpA_0d;
  output inpB_0r;
  input inpB_0a;
  input [7:0] inpB_0d;
  wire start_0n;
  wire nStart_0n;
  wire [8:0] nCv_0n;
  wire [8:0] c_0n;
  wire [7:0] eq_0n;
  wire [7:0] addOut_0n;
  wire [7:0] w_0n;
  wire [7:0] n_0n;
  wire v_0n;
  wire z_0n;
  wire nz_0n;
  wire nxv_0n;
  wire done_0n;
  AND2 I0 (out_0d[0], n_0n[0], w_0n[0]);
  AND2 I1 (out_0d[1], n_0n[1], w_0n[1]);
  AND2 I2 (out_0d[2], n_0n[2], w_0n[2]);
  AND2 I3 (out_0d[3], n_0n[3], w_0n[3]);
  AND2 I4 (out_0d[4], n_0n[4], w_0n[4]);
  AND2 I5 (out_0d[5], n_0n[5], w_0n[5]);
  AND2 I6 (out_0d[6], n_0n[6], w_0n[6]);
  AND2 I7 (out_0d[7], n_0n[7], w_0n[7]);
  assign done_0n = start_0n;
  assign n_0n[0] = inpB_0d[0];
  assign n_0n[1] = inpB_0d[1];
  assign n_0n[2] = inpB_0d[2];
  assign n_0n[3] = inpB_0d[3];
  assign n_0n[4] = inpB_0d[4];
  assign n_0n[5] = inpB_0d[5];
  assign n_0n[6] = inpB_0d[6];
  assign n_0n[7] = inpB_0d[7];
  assign w_0n[0] = inpA_0d[0];
  assign w_0n[1] = inpA_0d[1];
  assign w_0n[2] = inpA_0d[2];
  assign w_0n[3] = inpA_0d[3];
  assign w_0n[4] = inpA_0d[4];
  assign w_0n[5] = inpA_0d[5];
  assign w_0n[6] = inpA_0d[6];
  assign w_0n[7] = inpA_0d[7];
  assign out_0a = done_0n;
  C2 I26 (start_0n, inpA_0a, inpB_0a);
  assign inpA_0r = out_0r;
  assign inpB_0r = out_0r;
endmodule

module BrzFetch_8_s5_false (
  activate_0r, activate_0a,
  inp_0r, inp_0a, inp_0d,
  out_0r, out_0a, out_0d
);
  input activate_0r;
  output activate_0a;
  output inp_0r;
  input inp_0a;
  input [7:0] inp_0d;
  output out_0r;
  input out_0a;
  output [7:0] out_0d;
  assign activate_0a = out_0a;
  assign out_0r = inp_0a;
  assign inp_0r = activate_0r;
  assign out_0d[0] = inp_0d[0];
  assign out_0d[1] = inp_0d[1];
  assign out_0d[2] = inp_0d[2];
  assign out_0d[3] = inp_0d[3];
  assign out_0d[4] = inp_0d[4];
  assign out_0d[5] = inp_0d[5];
  assign out_0d[6] = inp_0d[6];
  assign out_0d[7] = inp_0d[7];
endmodule

module BrzLoop (
  activate_0r, activate_0a,
  activateOut_0r, activateOut_0a
);
  input activate_0r;
  output activate_0a;
  output activateOut_0r;
  input activateOut_0a;
  wire nReq_0n;
  supply0 gnd;
  INV I0 (nReq_0n, activate_0r);
  NOR2 I1 (activateOut_0r, nReq_0n, activateOut_0a);
  assign activate_0a = gnd;
endmodule

module BALSA_TELEMR (
  Ar,
  Aa,
  Br,
  Ba,
  initialise
);
  input Ar;
  output Aa;
  output Br;
  input Ba;
  input initialise;
  wire s;
  AND2 I0 (Br, Ar, s);
  INV I1 (s, Aa);
  C2R I2 (Aa, Ba, Ar, initialise);
endmodule

module BrzSequence_3_s2_TT (
  activate_0r, activate_0a,
  activateOut_0r, activateOut_0a,
  activateOut_1r, activateOut_1a,
  activateOut_2r, activateOut_2a,
  initialise
);
  input activate_0r;
  output activate_0a;
  output activateOut_0r;
  input activateOut_0a;
  output activateOut_1r;
  input activateOut_1a;
  output activateOut_2r;
  input activateOut_2a;
  input initialise;
  wire [2:0] sreq_0n;
  assign activate_0a = activateOut_2a;
  assign activateOut_2r = sreq_0n[2];
  assign sreq_0n[0] = activate_0r;
  BALSA_TELEMR I3 (sreq_0n[1], sreq_0n[2], activateOut_1r, activateOut_1a, initialise);
  BALSA_TELEMR I4 (sreq_0n[0], sreq_0n[1], activateOut_0r, activateOut_0a, initialise);
endmodule

module BrzVariable_8_1_s0_ (
  write_0r, write_0a, write_0d,
  read_0r, read_0a, read_0d
);
  input write_0r;
  output write_0a;
  input [7:0] write_0d;
  input read_0r;
  output read_0a;
  output [7:0] read_0d;
  wire [7:0] data_0n;
  wire nWriteReq_0n;
  wire bWriteReq_0n;
  wire nbWriteReq_0n;
  assign read_0a = read_0r;
  assign read_0d[0] = data_0n[0];
  assign read_0d[1] = data_0n[1];
  assign read_0d[2] = data_0n[2];
  assign read_0d[3] = data_0n[3];
  assign read_0d[4] = data_0n[4];
  assign read_0d[5] = data_0n[5];
  assign read_0d[6] = data_0n[6];
  assign read_0d[7] = data_0n[7];
  LATCH I9 (bWriteReq_0n, write_0d[0], data_0n[0]);
  LATCH I10 (bWriteReq_0n, write_0d[1], data_0n[1]);
  LATCH I11 (bWriteReq_0n, write_0d[2], data_0n[2]);
  LATCH I12 (bWriteReq_0n, write_0d[3], data_0n[3]);
  LATCH I13 (bWriteReq_0n, write_0d[4], data_0n[4]);
  LATCH I14 (bWriteReq_0n, write_0d[5], data_0n[5]);
  LATCH I15 (bWriteReq_0n, write_0d[6], data_0n[6]);
  LATCH I16 (bWriteReq_0n, write_0d[7], data_0n[7]);
  INV I17 (write_0a, nbWriteReq_0n);
  INV I18 (nbWriteReq_0n, bWriteReq_0n);
  INV I19 (bWriteReq_0n, nWriteReq_0n);
  INV I20 (nWriteReq_0n, write_0r);
endmodule

module Balsa_buffer1 (
  activate_0r, activate_0a,
  i_0r, i_0a, i_0d,
  j_0r, j_0a, j_0d,
  o_0r, o_0a, o_0d,
  initialise
);
  input activate_0r;
  output activate_0a;
  output i_0r;
  input i_0a;
  input [7:0] i_0d;
  output j_0r;
  input j_0a;
  input [7:0] j_0d;
  output o_0r;
  input o_0a;
  output [7:0] o_0d;
  input initialise;
  wire c13_r;
  wire c13_a;
  wire c12_r;
  wire c12_a;
  wire c11_r;
  wire c11_a;
  wire [7:0] c11_d;
  wire c10_r;
  wire c10_a;
  wire c9_r;
  wire c9_a;
  wire [7:0] c9_d;
  wire c8_r;
  wire c8_a;
  wire c7_r;
  wire c7_a;
  wire [7:0] c7_d;
  wire c6_r;
  wire c6_a;
  wire [7:0] c6_d;
  wire c5_r;
  wire c5_a;
  wire [7:0] c5_d;
  BrzVariable_8_1_s0_ I0 (c11_r, c11_a, c11_d, c6_r, c6_a, c6_d);
  BrzVariable_8_1_s0_ I1 (c9_r, c9_a, c9_d, c5_r, c5_a, c5_d);
  BrzLoop I2 (activate_0r, activate_0a, c13_r, c13_a);
  BrzSequence_3_s2_TT I3 (c13_r, c13_a, c12_r, c12_a, c10_r, c10_a, c8_r, c8_a, initialise);
  BrzFetch_8_s5_false I4 (c12_r, c12_a, i_0r, i_0a, i_0d, c11_r, c11_a, c11_d);
  BrzFetch_8_s5_false I5 (c10_r, c10_a, j_0r, j_0a, j_0d, c9_r, c9_a, c9_d);
  BrzFetch_8_s5_false I6 (c8_r, c8_a, c7_r, c7_a, c7_d, o_0r, o_0a, o_0d);
  BrzBinaryFunc_8_8_8_s3_And_s5_false_s5_fal_m2m I7 (c7_r, c7_a, c7_d, c6_r, c6_a, c6_d, c5_r, c5_a, c5_d);
endmodule

