#include "SourceB.h"

namespace the_carrefour {

Define_Module(SourceB);

SourceB::SourceB()
{
    newClientMessage = NULL;
    //timerMessage = NULL;
}

SourceB::~SourceB()
{
    cancelAndDelete(newClientMessage);
    //cancelAndDelete(timerMessage);
}

void SourceB::initialize()
{
    lastArrival = simTime();

    partialClientsVector.setName("number of clients entering the queue per timer interval");
    partialClientsVector.setInterpolationMode(cOutVector::NONE);

    randomQueueSelectionHist.setName("Queue selection distribution");
    randomQueueSelectionHist.setBinSizeHint(1);

    randomTillSelectionHist.setName("Till selection distribution");
    randomTillSelectionHist.setBinSizeHint(1);

    iaGenerationHist.setName("IA client generation times");
    iaGenerationHist.setBinSizeHint(10);

    newClientMessage = new cMessage("new_client");
    //timerMessage = new cMessage("timer");

    scheduleAt(simTime(), newClientMessage);
    //scheduleAt(simTime()+par("timerInterval").doubleValue(), timerMessage);

}

void SourceB::handleMessage(cMessage *msg)
{
    ASSERT(msg==newClientMessage);
    std::string rec_name = msg->getName();
    int queue_to_send;

    if(rec_name.compare("new_client")==0) {
        simtime_t iaTime = simTime() - lastArrival;
        iaGenerationHist.collect(iaTime); // collect inter arrival times
        lastArrival = simTime();
        n_clients_sent++;
        queue_to_send = find_empty_till(); // if more than one till is idle, return one of those at random
        if (queue_to_send >= 0){ // found and idle till, send to it (priority over queues with size zero, but not idle)
            EV << "SEND TO EMTPTY TILL N:" << queue_to_send << endl;
            send_client_to_queue(queue_to_send);
        }
        else { // no idle tills, proceed to find the queue with the smallest size, and send the client to it
            queue_to_send = find_smallest_queue();
            if (queue_to_send>=0 && queue_to_send<N_TILLS){
                EV << "SEND TO SMALLEST QUEUE N:" << queue_to_send << endl;
                send_client_to_queue(queue_to_send);
                queue_size_array[queue_to_send]++;
            }
            else {EV << "DISPATCH ERROR " << queue_to_send << endl;}
        }
        scheduleAt(simTime()+par("sendInterval").doubleValue(), newClientMessage);
    }
    else if (rec_name.compare("update")==0){
        Till2queue *tempMsg;
        tempMsg = (Till2queue*)msg;
        int rec_queue_n = tempMsg->getTill_n(); // read which queue sent the message
        queue_size_array[rec_queue_n]--;
    }
    else if (rec_name.compare("empty")==0){
        Till2queue *tempMsg;
        tempMsg = (Till2queue*)msg;
        int rec_queue_n = tempMsg->getTill_n(); // read which queue sent the message
        empty_till_array[rec_queue_n] = 0; // update that the till is idle
    }
    else if (rec_name.compare("timer")==0){

        partial_n = n_clients_sent - prev_count;
        prev_count = n_clients_sent;

        partialClientsVector.record(partial_n);
        //scheduleAt(simTime()+par("timerInterval").doubleValue(), timerMessage);
    }
}

void SourceB::finish(){
    recordStatistic(&randomQueueSelectionHist);
    recordStatistic(&randomTillSelectionHist);
}

/**
 *
 *
 */
int SourceB::find_empty_till(void){
    int empty_tills_found[N_TILLS] = {0};
    int amount_of_empty_tills = 0;

    for (int i=0; i<N_TILLS; i++){ // find an empty till
        if (empty_till_array[i]==0){
            empty_tills_found[amount_of_empty_tills] = i;
            amount_of_empty_tills++;
        }
    }

    EV << "FOUND " << amount_of_empty_tills << " EMPTY TILLS" << endl;
    if (amount_of_empty_tills==0){ // no idle tills found
        return -1;
    }
    else if (amount_of_empty_tills==1){ // one idle till found, send message to it
        empty_till_array[empty_tills_found[0]] = 1;

        EV << "CHOSEN QUEUE" << empty_tills_found[0] << " (one Empty Till found)" << endl;
        randomTillSelectionHist.collect(empty_tills_found[0]);
        return empty_tills_found[0];
    }
    else if ((amount_of_empty_tills>1) && (amount_of_empty_tills<=N_TILLS)){ //found multiple idle tills, randomly chooses one
        int rand_pos = intuniform(0, amount_of_empty_tills-1);
        empty_till_array[empty_tills_found[rand_pos]] = 1;

        EV << "CHOSEN QUEUE" << empty_tills_found[rand_pos] << " (Empty Tills rand decision)" << endl;
        randomTillSelectionHist.collect(empty_tills_found[rand_pos]);
        return empty_tills_found[rand_pos];
    }
    else {
        EV << "FIND EMPTY TILL ERROR " << endl;
        return -1;
    }

}

/**
 *
 *
 */
int SourceB::find_smallest_queue(void){
    int curr_smallest_queue = 0; // index (queue number)
    int smallest_queue_array[N_TILLS] = {0}; // indexes
    int n_queues_found = 0; // number of queues tied with the lowest value

    for (int i=0; i<N_TILLS; i++){ // find the first smallest queue
        if (queue_size_array[i] < queue_size_array[curr_smallest_queue]){
            curr_smallest_queue = i;
        }
        EV << "Queue size array pos (" << i << ") value:" << queue_size_array[i] << endl;
    }
    int n=0;
    for (int i=0; i<N_TILLS; i++){
        if (queue_size_array[i] == queue_size_array[curr_smallest_queue]){
            smallest_queue_array[n] = i;
            n++;
            EV << "Smallest queue array pos (" << n << ") value:" << i << endl;
            n_queues_found++;
        }
    }
    EV << "FOUND " << n_queues_found << " SMALLEST QUEUE(S) WITH SIZE" << smallest_queue_array[0] << endl;
    if (n_queues_found > 1){ // decide to which queue to send the client
        int rand_pos = intuniform(0, n_queues_found-1);
        EV << "CHOSEN QUEUE" << smallest_queue_array[rand_pos] << " (random choice)" << endl;
        randomQueueSelectionHist.collect(smallest_queue_array[rand_pos]);
        return smallest_queue_array[rand_pos];
    }
    else {
        EV << "CHOSEN QUEUE" << curr_smallest_queue << " (one found)" << endl;
        randomQueueSelectionHist.collect(curr_smallest_queue);
        return curr_smallest_queue;
    }

}

void SourceB::send_client_to_queue(int queue_n){
    Till2queue *job = new Till2queue("client");
    job->setTill_n(queue_n);
    std::string tmp = std::to_string(queue_n);
    const char *num_char = tmp.c_str();
    char out_port[] = "q_out";
    strcat(out_port, num_char); // output port name
    send(job, out_port); // send client to the queue
}

}; // namespace
