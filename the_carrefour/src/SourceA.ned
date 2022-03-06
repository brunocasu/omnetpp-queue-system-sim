package the_carrefour;


simple SourceA
{
    parameters:
        volatile double sendInterval @unit(s) = default(exponential(60s));
        volatile double timerInterval @unit(s) = 600s;
        @display("i=block/source");
    gates:
        output out;
}
