

module memory_controller(
    clk,
    
    wr_address,
    wr_en,
    wr_data,
    wr_ret_address,
    wr_ret_ack,
    
    rd_address,
    rd_en,
    rd_ret_data,
    rd_ret_address,
    rd_ret_ack
    );
    
    input clk;

    input      [15:0] wr_address;
    input             wr_en;
    input      [15:0] wr_data;
    output reg [15:0] wr_ret_address; // tag = address
    output reg        wr_ret_ack;

    input      [15:0] rd_address;
    input             rd_en;
    output reg [15:0] rd_ret_data;
    output reg [15:0] rd_ret_address; // tag = address
    output reg        rd_ret_ack;

    reg vld_init;

    reg vld_wr_rqst;
    reg vld_rd_rqst;

    reg vld_update;

    initial begin
        vld_init = $init($time);
    end

    always @(posedge clk) begin
        vld_update = $update($time);

        if(wr_en) begin
            vld_wr_rqst = $wr_rqst(wr_address, wr_data, $time);
        end
        if (rd_en) begin
            vld_rd_rqst = $rd_rqst(rd_address, $time);
        end
        
        {rd_ret_address, rd_ret_data, rd_ret_ack} = $rd_ret($time);
        {wr_ret_address, wr_ret_ack} = $wr_ret($time);
    end

endmodule
