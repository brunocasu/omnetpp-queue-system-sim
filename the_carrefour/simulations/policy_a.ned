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

package the_carrefour.simulations;

import the_carrefour.Source;
import the_carrefour.Queue;
import the_carrefour.Till;

network PolicyA
{
    submodules:
        source: Source;
        queue: Queue;
        till0: Till;
        till1: Till;
        till2: Till;
        till3: Till;
        till4: Till;
        till5: Till;
        till6: Till;
        till7: Till;
        till8: Till;
        till9: Till;
    connections:
        source.out --> queue.in;
        queue.t_out0 --> till0.in;
        till0.out --> queue.t_in0;
        
        queue.t_out1 --> till1.in;
        till1.out --> queue.t_in1;
        
        queue.t_out2 --> till2.in;
        till2.out --> queue.t_in2;
        
        queue.t_out3 --> till3.in;
        till3.out --> queue.t_in3;
        
        queue.t_out4 --> till4.in;
        till4.out --> queue.t_in4;
        
        queue.t_out5 --> till5.in;
        till5.out --> queue.t_in5;
        
        queue.t_out6 --> till6.in;
        till6.out --> queue.t_in6;
        
        queue.t_out7 --> till7.in;
        till7.out --> queue.t_in7;
        
        queue.t_out8 --> till8.in;
        till8.out --> queue.t_in8;
        
        queue.t_out9 --> till9.in;
        till9.out --> queue.t_in9;
}
