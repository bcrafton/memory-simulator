
module top;
    reg  [15:0]  wr_address;
    reg          wr_en;
    reg  [15:0]  wr_data;
    wire [15:0]  wr_ret_address;
    wire         wr_ret_ack;

    reg  [15:0]  rd_address;
    reg          rd_en;
    wire [15:0]  rd_ret_data;
    wire [15:0]  rd_ret_address;
    wire         rd_ret_ack;

    reg clk;

    reg dump_valid;

    memory_controller mc(
    .clk(clk),
    .wr_address(wr_address),
    .wr_en(wr_en),
    .wr_data(wr_data),
    .wr_ret_address(wr_ret_address),
    .wr_ret_ack(wr_ret_ack),

    .rd_address(rd_address),
    .rd_en(rd_en),
    .rd_ret_data(rd_ret_data),
    .rd_ret_address(rd_ret_address),
    .rd_ret_ack(rd_ret_ack)
    );

    initial begin
        // it does not allow you to have multiple drivers
        // actually awesome
        wr_address = 0;
        wr_en = 0;
        wr_data = 0;
        //wr_ret_address = 0;
        //wr_ret_ack = 0;
        
        rd_address = 0;
        rd_en = 0;
        //rd_ret_data = 0;
        //rd_ret_address = 0;
        //rd_ret_ack = 0;
        
        wr_address = 0;
        wr_data = 0;
        rd_en = 1;
        wr_en = 1;

        clk = 0;
        dump_valid = 0;
    end

    always #1 clk = ~clk;

    always @(posedge clk) begin

        if ($time > 250) begin
            rd_en <= 0;
            wr_en <= 0;
        end else begin
            //$display("%h %h\n", rd_address, $time);
        end

        rd_address <= rd_address + 1;
        wr_address <= wr_address + 1;
        wr_data <= wr_data + 1;

        if(rd_ret_ack == 1) begin
            //$display("%h %h", rd_ret_address, rd_ret_data);
        end
    end

    always @(posedge clk) begin

        if($time > 175) begin
            dump_valid <= $dump($time);
            $finish;
        end
    end

endmodule
