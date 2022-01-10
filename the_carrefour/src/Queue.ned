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
// Consumes received messages and collects statistics
//
simple Queue
{
    parameters:
        volatile double deltaInterval @unit(s) = 2s;
        @display("i=block/queue");
    gates:
        input in;
        
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
