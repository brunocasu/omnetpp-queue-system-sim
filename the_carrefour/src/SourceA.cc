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
        scheduleAt(simTime()+discourage_multiplyer*(par("sendInterval").doubleValue()), newClientMessage);
    }
    else if (rec_name.compare("timer")==0){

        partial_n = n_clients_sent - prev_count;
        prev_count = n_clients_sent;

        partialClientsVector.record(partial_n);
        scheduleAt(simTime()+par("timerInterval").doubleValue(), timerMessage);
    }
    else if (rec_name.compare("discourage")==0){
        Till2queue *tempMsg;
        tempMsg = (Till2queue*)msg;
        int rec_queue_size = tempMsg->getTill_n(); // method to transmit the queue size information for the source module
        EV << "DISCOURAGED MESSAGE RECEIVED - CURRENT QUEUE SIZE = "<< rec_queue_size << endl;
        if (rec_queue_size < 5) {discourage_multiplyer = 1;}
        else if (rec_queue_size >= 5 && rec_queue_size < 10) {discourage_multiplyer = 1.1;}
        else if (rec_queue_size >= 10 && rec_queue_size < 15) {discourage_multiplyer = 1.2;}
        else if (rec_queue_size >= 15 && rec_queue_size < 20) {discourage_multiplyer = 1.3;}
        else if (rec_queue_size >= 20 && rec_queue_size < 25) {discourage_multiplyer = 1.4;}
        else if (rec_queue_size >= 25 && rec_queue_size < 30) {discourage_multiplyer = 1.5;}
    }
}

}; // namespace
