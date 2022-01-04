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

//
// Sample network, consisting of a source and a sink.
//
network TheCarrefour
{
    submodules:
        source: Source;
        queue: Queue;
        till0: Till;
        till1: Till;
        till2: Till;
    connections:
        source.out --> queue.in;
        queue.t_out0 --> till0.in;
        till0.out --> queue.t_in0;
        queue.t_out1 --> till1.in;
        till1.out --> queue.t_in1;
        queue.t_out2 --> till2.in;
        till2.out --> queue.t_in2;
}
