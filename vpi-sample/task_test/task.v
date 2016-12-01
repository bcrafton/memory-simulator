module main;
  integer val;
  reg [31:0] val2;
  reg [31:0] val3;
initial begin
  val = 1;
  val2 = 2;
  val3 = 3;
  $task_test(99, val, val2, val3);
  $display("val=%d", val);
  $display("val2=%d", val2);
  $display("val3=%d", val3);
end
endmodule
