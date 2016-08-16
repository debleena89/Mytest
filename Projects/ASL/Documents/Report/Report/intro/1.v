module top;
reg clk,r1,r2;
wire g1,g2;

arbiter A(.r1(r1), .r2(r2), .g1(g1), .g2(g2), .clk(clk));

integer i=0;

always #1 clk = ~clk;

always @(posedge clk)
begin
	r2 = ~r2;
	if (i < 4) 
	begin 
		i = i + 1;
		r1 = 0;
	end
	else 
	begin 
		i = 0;
		r1 = 1;
	end
end

always @ (negedge clk)
begin
        r1 = 0;
end

initial 
begin
    clk = 0;
    r1 = 0;
    r2 = 0;
    $monitor($time, "Clock = %b , r1 = %b r2 = %b",clk,r1,r2);
    #50 $finish;
end
endmodule

module arbiter (r1,r2, g1, g2, clk);
input clk,r1,r2;
output g1,g2;
reg g1,g2;

always @(posedge clk)
begin
	g2 <= r2 & ~r1 & g1;	
	g1 <= r1;
end
endmodule
