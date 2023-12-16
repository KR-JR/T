module ALU_slice_MSB (
  input a,
  input b,
  input cin,
  input [2:0] opcode,
  output reg result,
  output reg cout,
  output reg overflow
);

  always @(a, b, cin, opcode) begin
    // 기본 값을 초기화
    result = 0;
    cout = 0;
    overflow = 0;
  
    // 여기에 ALU 연산 로직을 구현합니다.
    // opcode 값에 따라 다양한 연산을 선택
    case (opcode)
      // 예: opcode가 3'b000 일 때 덧셈
      3'b000: begin
        {cout, result} = a + b + cin; // 덧셈 연산 결과
        overflow = cout ^ (a[MSB] ^ b[MSB]); // 오버플로우 검출
      end
      // 다른 연산들에 대한 case 구문 추가...
    endcase
  end
endmodule

