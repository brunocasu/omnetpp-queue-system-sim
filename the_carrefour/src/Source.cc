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

#include "Source.h"

namespace the_carrefour {

Define_Module(Source);

Source::Source()
{
    newClientMessage = NULL;
    timerMessage = NULL;
}

Source::~Source()
{
    cancelAndDelete(newClientMessage);
    cancelAndDelete(timerMessage);
}

void Source::initialize()
{
    partialClientsVector.setName("number of clients entering the queue per timer interval");
    partialClientsVector.setInterpolationMode(cOutVector::NONE);

    newClientMessage = new cMessage("new_client");
    timerMessage = new cMessage("timer");

    scheduleAt(simTime(), newClientMessage);
    scheduleAt(simTime()+par("timerInterval").doubleValue(), timerMessage);

}

void Source::handleMessage(cMessage *msg)
{
    ASSERT(msg==newClientMessage);

    std::string rec_name = msg->getName();
    if(rec_name.compare("new_client")==0) {
        cMessage *job = new cMessage("client");
        send(job, "out"); // send client to the queue
        n_clients_sent++;
        scheduleAt(simTime()+par("sendInterval").doubleValue(), newClientMessage);
    }
    else if (rec_name.compare("timer")==0){

        partial_n = n_clients_sent - prev_count;
        prev_count = n_clients_sent;

        partialClientsVector.record(partial_n);
        scheduleAt(simTime()+par("timerInterval").doubleValue(), timerMessage);
    }
}

}; // namespace
