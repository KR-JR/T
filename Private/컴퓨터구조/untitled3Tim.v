// Auto-generated test bench created by VeriLogger Pro at Mon Nov 06 15:07:04 2023

// Timing model is min/max timing. 

`timescale 1ns / 1ps

module syncad_top;


wire [1:0] tb_status;
reg [1:0] tb_status_driver;
assign tb_status = tb_status_driver;
reg [15:0] a;
reg [15:0] b;
wire [15:0] sum;
wire  cout;

initial tb_status_driver = 1'b1; //diagram always running in auto-generated file

ripple_adder ripple_adder(  .a ( a ),
  .b ( b ),
  .sum ( sum ),
  .cout ( cout ));

  initial
    begin
    end
endmodule

