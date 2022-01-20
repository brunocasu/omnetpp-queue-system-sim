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
simple QueueB
{
    parameters:
        volatile double timerInterval @unit(s) = 60s;
        @display("i=block/queue");
    gates:
        input s_in; // source input
        output s_out;
        
        input t_in; // till input
        output t_out;
                
}
