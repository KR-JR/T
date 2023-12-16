module ALU16 (
  input [15:0] a,
  input [15:0] b,
  input cin,
  input [2:0] opcode,
  output [15:0] result,
  output cout,
  output overflow
  // set 출력 포트 제거됨
);

wire [14:0] c; // Internal carries between the slices

// Instantiate 15 regular ALU slices for bits 0 to 14
genvar i;
generate
  for (i = 0; i < 15; i = i + 1) begin : alu_slices
    ALU_slice slice (
      .a(a[i]),
      .b(b[i]),
      .cin(i == 0 ? cin : c[i - 1]),
      .opcode(opcode),
      .result(result[i]),
      .cout(c[i])
    );
  end
endgenerate

// Instantiate the MSB ALU slice with overflow detection
ALU_slice_MSB msb_slice (
  .a(a[15]),
  .b(b[15]),
  .cin(c[14]),
  .opcode(opcode),
  .result(result[15]),
  .cout(cout),
  .overflow(overflow)
  // .set(set) 포트 연결 제거됨
);

endmodule

