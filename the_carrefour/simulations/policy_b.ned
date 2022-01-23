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

import the_carrefour.SourceB;
import the_carrefour.QueueB;
import the_carrefour.TillB;

network PolicyB
{
    submodules:
        source: SourceB {
            @display("p=650,400");
        }
        queue0: QueueB {
            @display("p=200,200");
        }
        queue1: QueueB {
            @display("p=300,200");
        }
        queue2: QueueB {
            @display("p=400,200");
        }
        queue3: QueueB {
            @display("p=500,200");
        }
        queue4: QueueB {
            @display("p=600,200");
        }
        queue5: QueueB {
            @display("p=700,200");
        }
        queue6: QueueB {
            @display("p=800,200");
        }
        queue7: QueueB {
            @display("p=900,200");
        }
        queue8: QueueB {
            @display("p=1000,200");
        }
        queue9: QueueB {
            @display("p=1100,200");
        }
        till0: TillB {
            @display("p=200,100");
        }
        till1: TillB {
            @display("p=300,100");
        }
        till2: TillB {
            @display("p=400,100");
        }
        till3: TillB {
            @display("p=500,100");
        }
        till4: TillB {
            @display("p=600,100");
        }
        till5: TillB {
            @display("p=700,100");
        }
        till6: TillB {
            @display("p=800,100");
        }
        till7: TillB {
            @display("p=900,100");
        }
        till8: TillB {
            @display("p=1000,100");
        }
        till9: TillB {
            @display("p=1100,100");
        }
    connections:
        source.q_out0 --> queue0.s_in;
        queue0.s_out --> source.q_in0;

        source.q_out1 --> queue1.s_in;
        queue1.s_out --> source.q_in1;

        source.q_out2 --> queue2.s_in;
        queue2.s_out --> source.q_in2;

        source.q_out3 --> queue3.s_in;
        queue3.s_out --> source.q_in3;

        source.q_out4 --> queue4.s_in;
        queue4.s_out --> source.q_in4;

        source.q_out5 --> queue5.s_in;
        queue5.s_out --> source.q_in5;

        source.q_out6 --> queue6.s_in;
        queue6.s_out --> source.q_in6;

        source.q_out7 --> queue7.s_in;
        queue7.s_out --> source.q_in7;

        source.q_out8 --> queue8.s_in;
        queue8.s_out --> source.q_in8;

        source.q_out9 --> queue9.s_in;
        queue9.s_out --> source.q_in9;

        queue0.t_out --> till0.in;
        till0.out --> queue0.t_in;
        queue1.t_out --> till1.in;
        till1.out --> queue1.t_in;
        queue2.t_out --> till2.in;
        till2.out --> queue2.t_in;
        queue3.t_out --> till3.in;
        till3.out --> queue3.t_in;
        queue4.t_out --> till4.in;
        till4.out --> queue4.t_in;
        queue5.t_out --> till5.in;
        till5.out --> queue5.t_in;
        queue6.t_out --> till6.in;
        till6.out --> queue6.t_in;
        queue7.t_out --> till7.in;
        till7.out --> queue7.t_in;
        queue8.t_out --> till8.in;
        till8.out --> queue8.t_in;
        queue9.t_out --> till9.in;
        till9.out --> queue9.t_in;
}
