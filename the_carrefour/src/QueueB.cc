#include "QueueB.h"

namespace the_carrefour {

Define_Module(QueueB);

QueueB::QueueB()
{
    //qtimerMessage = NULL;
}

QueueB::~QueueB()
{
    //cancelAndDelete(qtimerMessage);
}

void QueueB::initialize()
{

    lastArrival = simTime();
    iaLocalTimeHistogram.setName("local inter arrival times");
    iaLocalTimeHistogram.setBinSizeHint(5);

    procTimeHistogram.setName("processing times");
    procTimeHistogram.setBinSizeHint(10);

    time_in_queueVector.setName("time spent on the queue");
    time_in_queueVector.setInterpolationMode(cOutVector::NONE);

    proc_timeVector.setName("processing time with client order");
    proc_timeVector.setInterpolationMode(cOutVector::NONE);

    queue_sizeVector.setName("queue size per minute");
    queue_sizeVector.setInterpolationMode(cOutVector::NONE);

    queue_progressionVector.setName("queue size after modification");
    queue_progressionVector.setInterpolationMode(cOutVector::NONE);

    //qtimerMessage = new cMessage("timer");
    //scheduleAt(simTime()+par("timerInterval").doubleValue(), qtimerMessage);

}

void QueueB::handleMessage(cMessage *msg)
{
    //ASSERT(msg==qtimerMessage);
    //EV << "Received " << msg->getName() << endl;
    std::string rec_name = msg->getName();
    Till2queue *tempMsg;
    tempMsg = (Till2queue*)msg;

    //int till_to_send = -1; // assigned till number is undefined at the start
    //test_fun(2);
    if (rec_name.compare("client")==0){ //received a new client from the source
        queue_number = tempMsg->getTill_n();
        EV << "QUEUE " << queue_number << " RECEIVED CLIENT = "<< tot_n_clients << endl;

        n_clients_in_queue++; // increase the number of clients currently in the queue
        tot_n_clients++; // increase total number of clients that entered the queue

        collect_new_client_entry_data();

        if (empty_till_ctrl == 0){ // if the till is empty, send the client to it
            time_in_queueVector.record(simTime() - entryQueueTime); // always zero

            dispatch_client();

            queue_progressionVector.record(n_clients_in_queue); // mark number of clients in the queue
        }
        else if (empty_till_ctrl == 1) { // the till is already processing a client
            EV << "TILL IS PROCESSING" << endl;
            queue_progressionVector.record(n_clients_in_queue); // mark number of clients in the queue
        }
        else {
            EV << "QUEUE CONTROL ERROR" << endl;
        }
        EV << "CURRENT QUEUE: " << n_clients_in_queue << " CLIENT(S)" << endl;
        delete msg;
    }
    else if (rec_name.compare("empty")==0){ // till processed the client
        Till2queue *tempMsg;
        tempMsg = (Till2queue*)msg;
        simtime_t procTime = tempMsg->getProcTime();
        collect_processing_data(procTime);

        if (n_clients_in_queue > 0 ){ // if clients are waiting in the queue, dispatch the next one
            time_in_queueVector.record(simTime() - entryQueueTime); // calculate client queue time
            dispatch_client();

            Till2queue *update_to_source = new Till2queue("update"); // queue size has decreased, inform the source
            update_to_source->setTill_n(queue_number); // add the queue/till number to the message
            send(update_to_source, "s_out");

            queue_progressionVector.record(n_clients_in_queue);

            EV << "UPDATE IN QUEUE" << queue_number << ": " << n_clients_in_queue << " CLIENT(S)" << endl;
        }
        else{ // no clients in the queue
            Till2queue *update_to_source = new Till2queue("empty"); // inform the source that the till is empty
            update_to_source->setTill_n(queue_number); // add the queue/till number to the message
            send(update_to_source, "s_out");
            empty_till_ctrl = 0; // free the till
            EV << "UPDATE IN QUEUE" << queue_number << ": TILL IS EMPTY" << endl;
        }
        delete msg;
    }
    else if (rec_name.compare("timer")==0){
        //queue_sizeVector.record(n_clients_in_queue); // queue size every timer interval
        //scheduleAt(simTime()+par("timerInterval").doubleValue(), qtimerMessage);
    }
}

void QueueB::finish()
{
    EV << "TOTAL NUMBER OF CLIENTS " << tot_n_clients << endl;
    recordStatistic(&iaLocalTimeHistogram);
    recordStatistic(&procTimeHistogram);
}

// functions


/**
 * collect inter arrival time for each new client
 * mark the client entry time in the queue
 */
void QueueB::collect_new_client_entry_data(void){
    simtime_t iaTime = simTime() - lastArrival;
    iaLocalTimeHistogram.collect(iaTime); // collect inter arrival times
    lastArrival = simTime();

    entryQueueTime = simTime();
}

/**
 * send client to designated till
 * add a delay value, corresponding to the distance from the till
 * for higher till numbers, the time to reach increases
 * delay  = (till_n + 1)*delta
 * the target till remains in idle while the message does not reach it
 */
void QueueB::dispatch_client(void){
    Till2queue *job = new Till2queue("client");
    send(job, "t_out");
    //sendDelayed(job, (1+till_to_send)*(par("deltaInterval").doubleValue()), out_port);

    n_clients_in_queue--; // remove client from the queue
    empty_till_ctrl = 1; // allocate the till
}

/**
 * save the processing time of the client (histogram)
 * mark the client number in the client order vector (used to identify each client to its processing time)
 * mark the client processing time (same order as the client order vector)
 * mark which till was used by the client
 * mark the total time the client spent in the till, adding the time to reach it (delay)
 */
void QueueB::collect_processing_data(simtime_t procTime){
    procTimeHistogram.collect(procTime);
    proc_timeVector.record(procTime);

}

}; // namespace
