#include "SourceB.h"

namespace the_carrefour {

Define_Module(SourceB);

SourceB::SourceB()
{
    newClientMessage = NULL;
    timerMessage = NULL;
}

SourceB::~SourceB()
{
    cancelAndDelete(newClientMessage);
    cancelAndDelete(timerMessage);
}

void SourceB::initialize()
{
    partialClientsVector.setName("number of clients entering the queue per timer interval");
    partialClientsVector.setInterpolationMode(cOutVector::NONE);

    newClientMessage = new cMessage("new_client");
    timerMessage = new cMessage("timer");

    scheduleAt(simTime(), newClientMessage);
    scheduleAt(simTime()+par("timerInterval").doubleValue(), timerMessage);

}

void SourceB::handleMessage(cMessage *msg)
{
    ASSERT(msg==newClientMessage);
    std::string rec_name = msg->getName();

    if(rec_name.compare("new_client")==0) {
        cMessage *job = new cMessage("client");
        int queue_to_send = find_empty_till(); // if more than one till is idle, return one of those at random
        if (queue_to_send >= 0){ // found and idle till, send to it (priority over queues with size zero, but not idle)
            std::string tmp = std::to_string(queue_to_send);
            const char *num_char = tmp.c_str();
            char out_port[] = "q_out";
            strcat(out_port, num_char); // output port name
            send(job, out_port); // send client to the queue
        }
        else { // no idle tills, proceed to find the queue with smaller size, and send the client to it

        }





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

int SourceB::find_empty_till(void){
    int empty_tills_found[N_TILLS] = {0};
    int amount_of_empty_tills = 0;

    for (int i=0; i<N_TILLS; i++){ // find an empty till
        if (empty_till_array[i]==0){
            empty_tills_found[amount_of_empty_tills] = i;
            amount_of_empty_tills++;
            empty_till_array[i] = 1; // allocate till
        }
    }

    if (amount_of_empty_tills==0){ // no idle tills found
        return -1;
    }
    else if (amount_of_empty_tills==1){ // one idle till found, send message to it
        empty_till_array[empty_tills_found[0]];
        return empty_tills_found[0];
    }
    else if ((amount_of_empty_tills>1) && (amount_of_empty_tills<=N_TILLS)){ //found multiple idle tills, randomly chooses one
        int rand_pos = intuniform(0, amount_of_empty_tills-1);
        empty_till_array[empty_tills_found[rand_pos]];
        return empty_tills_found[rand_pos];
    }

}
int SourceB::find_smalest_queue(void){

}

}; // namespace
