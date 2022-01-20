#include "SourceA.h"

namespace the_carrefour {

Define_Module(SourceA);

SourceA::SourceA()
{
    newClientMessage = NULL;
    timerMessage = NULL;
}

SourceA::~SourceA()
{
    cancelAndDelete(newClientMessage);
    cancelAndDelete(timerMessage);
}

void SourceA::initialize()
{
    partialClientsVector.setName("number of clients entering the queue per timer interval");
    partialClientsVector.setInterpolationMode(cOutVector::NONE);

    newClientMessage = new cMessage("new_client");
    timerMessage = new cMessage("timer");

    scheduleAt(simTime(), newClientMessage);
    scheduleAt(simTime()+par("timerInterval").doubleValue(), timerMessage);

}

void SourceA::handleMessage(cMessage *msg)
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
