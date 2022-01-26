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
    iaLocalTimeHistogram.setName("local interarrival times");
    iaLocalTimeHistogram.setBinSizeHint(10);

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

}

void QueueB::handleMessage(cMessage *msg)
{
    std::string rec_name = msg->getName();
    Till2queue *tempMsg;
    tempMsg = (Till2queue*)msg;

    if (rec_name.compare("client")==0){ //received a new client from the source
        queue_number = tempMsg->getTill_n();
        EV << "QUEUE " << queue_number << " RECEIVED CLIENT = "<< tot_n_clients << endl;

        collect_new_client_entry_data();

        if (empty_till_ctrl == 0){ // if the till is empty, send the client to it
            collect_client_dispatch_data();

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
            collect_client_dispatch_data();
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

}

void QueueB::finish()
{
    EV << "TOTAL NUMBER OF CLIENTS " << tot_n_clients << endl;
    recordStatistic(&iaLocalTimeHistogram);
    recordStatistic(&procTimeHistogram);
}

// functions


/**
 * collect interarrival time for each new client
 * mark the client entry time in the queue
 */
void QueueB::collect_new_client_entry_data(void){
    simtime_t iaTime = simTime() - lastArrival;
    iaLocalTimeHistogram.collect(iaTime); // collect inter arrival times
    lastArrival = simTime();

    if (n_clients_in_queue < QUEUE_CONTROL_SIZE){ // prevents buffer overflow
        entryQueueTime[n_clients_in_queue] = simTime(); // mark client time entering the queue, he/she is placed in the last position (FIFO queue)
    }

    n_clients_in_queue++; // increase the number of clients currently in the queue
    tot_n_clients++; // increase total number of clients that entered the queue
}

/**
 * send client to designated till
 * allocate the till
 */
void QueueB::dispatch_client(void){
    Till2queue *job = new Till2queue("client");
    job->setTill_n(queue_number);
    send(job, "t_out");

    n_clients_in_queue--; // remove client from the queue
    empty_till_ctrl = 1; // allocate the till
}

/**
 * calculate the client time on the queue
 * shift the collected times in the control vector
 * position zero of the control vector is always used to calculate the queue time
 * the queue is in FIFO mode
 *
 */
void QueueB::collect_client_dispatch_data(void){
    time_in_queueVector.record(simTime() - entryQueueTime[0]); // calculate client queue time
    for (int k=1; k<QUEUE_CONTROL_SIZE-1; k++){ // move the queue recorded times
        entryQueueTime[k-1] = entryQueueTime[k];
    }
    entryQueueTime[QUEUE_CONTROL_SIZE-1] = 0;
}


/**
 * save the processing time of the client
 * in the scenario b, the clients at a single queue/till system are always processed in order
 * no need to collect vectors for client processing synchronization
 */
void QueueB::collect_processing_data(simtime_t procTime){
    procTimeHistogram.collect(procTime);
    proc_timeVector.record(procTime);

}

}; // namespace
