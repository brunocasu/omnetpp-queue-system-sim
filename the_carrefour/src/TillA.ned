package the_carrefour;


simple TillA
{
    parameters:
        volatile double procInterval @unit(s) = default(exponential(240s));
        //volatile double procInterval @unit(s) = lognormal(0, 0.10)*240s;
        volatile double minProcInterval @unit(s) = 30s;
        @display("i=block/sink");
    gates:
        input in;
        output out;
}
