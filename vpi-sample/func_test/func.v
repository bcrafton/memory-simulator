module main;
  reg [31:0] val;
initial begin
  val = $func_test(32'hdeadbeef);
  $display("val=%h", val);
end
endmodule
