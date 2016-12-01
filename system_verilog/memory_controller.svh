// header file for the memory controller

typedef bit [15:0] WORD;

class mrqst_t;
    WORD address;
    WORD data;
    // letting this be randomized to whatever
    // will say anything that matches the time or is ahead of it ships
    int response_time;
    function new(WORD address, WORD data);
    begin
        this.address = address;
        this.data = data;
        this.response_time = $time + $urandom_range(25, 100);
    end
    endfunction
endclass

// ideally would use a heap... but dont have that.
function add_to_rqst_q(ref mrqst_t rqst_q[$], ref mrqst_t rqst);
    int size, i;
    size = rqst_q.size();
    if(size == 0) begin
        rqst_q.push_back(rqst);
    end 
    else begin
        for(i=0; i<size; i++) begin
            if(rqst.response_time < rqst_q[i].response_time) begin
                rqst_q.insert(i, rqst);
                return;
            end
        end
        rqst_q.push_back(rqst);
    end
endfunction