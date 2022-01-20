//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package the_carrefour;

//
// Generates messages with a configurable interarrival time.
//
simple SourceB
{
    parameters:
        volatile double sendInterval @unit(s) = default(exponential(20s));
        volatile double timerInterval @unit(s) = 600s;
        @display("i=block/source");
    gates:
        input q_in0;
        output q_out0;
        input q_in1;
        output q_out1;
        input q_in2;
        output q_out2;
        input q_in3;
        output q_out3;
        input q_in4;
        output q_out4;
        input q_in5;
        output q_out5;
        input q_in6;
        output q_out6;
        input q_in7;
        output q_out7;
        input q_in8;
        output q_out8;
        input q_in9;
        output q_out9;
        
}
