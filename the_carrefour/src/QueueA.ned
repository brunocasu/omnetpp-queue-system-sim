package the_carrefour;

simple QueueA
{
    parameters:
        volatile double deltaInterval @unit(s) = 2s;
        volatile double timerInterval @unit(s) = 60s;
        @display("i=block/queue");
    gates:
        input in;
        output out;
        
        input t_in0;
        output t_out0;
        input t_in1;
        output t_out1;
        input t_in2;
        output t_out2;
        input t_in3;
        output t_out3;
        input t_in4;
        output t_out4;
        input t_in5;
        output t_out5;
        input t_in6;
        output t_out6;
        input t_in7;
        output t_out7;
        input t_in8;
        output t_out8;
        input t_in9;
        output t_out9;
                
}
