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

#include "Queue.h"

namespace the_carrefour {

Define_Module(Queue);

void Queue::initialize()
{
    lastArrival = simTime();
    iaTimeHistogram.setName("interarrival times");
    arrivalsVector.setName("arrivals");
    arrivalsVector.setInterpolationMode(cOutVector::NONE);
}

void Queue::handleMessage(cMessage *msg)
{
    simtime_t d = simTime() - lastArrival;
    EV << "Received " << msg->getName() << endl;

    std::string str1 = msg->getName();
    if (str1.compare("client")==0){
        iaTimeHistogram.collect(d);
        arrivalsVector.record(1);

        lastArrival = simTime();

        ASSERT(msg==timerMessage);

        cMessage *job = new cMessage("client");
        send(job, "t_out");
    }
    delete msg;
}

void Queue::finish()
{
    recordStatistic(&iaTimeHistogram);
}

}; // namespace
