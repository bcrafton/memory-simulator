module main;
    reg [63:0] val;
    reg [15:0] a;
    reg [15:0] b;

    initial begin
        val = 0;
        val = $func_test(32'hdeadbeef);
        $display("%h", val);
    end
endmodule
