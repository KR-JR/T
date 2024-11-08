module fulladder(a, b, cin, sum, cout);
  input a, b, cin;
  output sum, cout;

  assign sum = a ^ b ^ cin;
  assign cout = a & b | a & cin | b & cin;
endmodule

module ripple_adder(a, b, sum, cout);
  input [15:0] a, b;     // Extended to 16 bits
  output [15:0] sum;     // Extended to 16 bits
  output cout;

  wire [15:0] c;  // Extended to 16 bits for carry connections

  assign c[0] = 0;  // No initial carry input

  // Instantiate 16 full adders for the 16-bit ripple adder
  fulladder f0(a[0], b[0], c[0], sum[0], c[1]);
  fulladder f1(a[1], b[1], c[1], sum[1], c[2]);
  fulladder f2(a[2], b[2], c[2], sum[2], c[3]);
  fulladder f3(a[3], b[3], c[3], sum[3], c[4]);
  fulladder f4(a[4], b[4], c[4], sum[4], c[5]);
  fulladder f5(a[5], b[5], c[5],   sum[5], c[6]);
  fulladder f6(a[6], b[6], c[6], sum[6], c[7]);
  fulladder f7(a[7], b[7], c[7], sum[7], c[8]);
  fulladder f8(a[8], b[8], c[8], sum[8], c[9]);
  fulladder f9(a[9], b[9], c[9], sum[9], c[10]);
  fulladder f10(a[10], b[10], c[10], sum[10], c[11]);
  fulladder f11(a[11], b[11], c[11], sum[11], c[12]);
  fulladder f12(a[12], b[12], c[12], sum[12], c[13]);
  fulladder f13(a[13], b[13], c[13], sum[13], c[14]);
  fulladder f14(a[14], b[14], c[14], sum[14], c[15]);
  fulladder f15(a[15], b[15], c[15], sum[15], cout); // Last carry out is the output carry
endmodule

