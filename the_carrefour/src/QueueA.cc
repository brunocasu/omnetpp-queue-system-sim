#include "QueueA.h"

namespace the_carrefour {

Define_Module(QueueA);

QueueA::QueueA()
{
    qtimerMessage = NULL;
}

QueueA::~QueueA()
{
    cancelAndDelete(qtimerMessage);
}

void QueueA::initialize()
{
    lastArrival = simTime();
    iaTimeHistogram.setName("inter arrival times");
    iaTimeHistogram.setBinSizeHint(10);

    procTimeHistogram.setName("processing times");
    procTimeHistogram.setBinSizeHint(10);

    time_in_queueVector.setName("time spent on the queue");
    time_in_queueVector.setInterpolationMode(cOutVector::NONE);

    client_proc_orderVector.setName("client processing order");
    client_proc_orderVector.setInterpolationMode(cOutVector::NONE);

    proc_timeVector.setName("processing time with client order");
    proc_timeVector.setInterpolationMode(cOutVector::NONE);

    till_proc_orderVector.setName("till processing order");
    till_proc_orderVector.setInterpolationMode(cOutVector::NONE);

    client_till_timeVector.setName("client total time in the till");
    client_till_timeVector.setInterpolationMode(cOutVector::NONE);

    queue_sizeVector.setName("queue size per minute");
    queue_sizeVector.setInterpolationMode(cOutVector::NONE);

    queue_progressionVector.setName("queue size after modification");
    queue_progressionVector.setInterpolationMode(cOutVector::NONE);

    qtimerMessage = new cMessage("timer");
    scheduleAt(simTime()+par("timerInterval").doubleValue(), qtimerMessage);

}

void QueueA::handleMessage(cMessage *msg)
{
    std::string rec_name = msg->getName();
    int till_to_send = -1; // assigned till number is undefined at the start

    if (rec_name.compare("client")==0){ // received a new client from the source

        EV << "RECEIVED CLIENT N = "<< tot_n_clients << endl;
        EV << "CURRENT QUEUE: " << n_clients_in_queue << " CLIENT(S)" << endl;

        collect_new_client_entry_data();

        till_to_send = find_empty_till();

        if ((till_to_send >= 0) && (till_to_send < N_TILLS)){ // if an empty till is found, send client to it
            collect_client_dispatch_data(till_to_send);

            dispatch_client(till_to_send);

            EV << "CLIENT DISPATCHED TO TILL " << till_to_send <<  endl;

            queue_progressionVector.record(n_clients_in_queue); // mark number of clients in the queue
        }
        else { // No Tills available
            EV << "ALL TILLS OCCUPIED" << endl;
            queue_progressionVector.record(n_clients_in_queue); // mark number of clients in the queue
        }
        delete msg;
    }
    else if (rec_name.compare("empty")==0){
        Till2queue *tempMsg;
        tempMsg = (Till2queue*)msg;
        int rec_till_n = tempMsg->getTill_n(); // read which till sent the message
        simtime_t procTime = tempMsg->getProcTime();
        collect_processing_data(rec_till_n, procTime);

        EV << "TILL " << rec_till_n << " IS NOW FREE" << endl;

        if (n_clients_in_queue > 0 ){ // if clients are waiting in the queue, dispatch the next one to the now free till
            collect_client_dispatch_data (rec_till_n);

            dispatch_client(rec_till_n);

            EV << "CLIENT DISPATCHED TO " << rec_till_n << endl;

            queue_progressionVector.record(n_clients_in_queue);

            EV << "CURRENT QUEUE: " << n_clients_in_queue << " CLIENT(S)" << endl;

            empty_till_array[rec_till_n] = 1; // take the till again
        }
        else{ // no clients in the queue
            empty_till_array[rec_till_n] = 0; // only free the till
        }
        delete msg;
    }
    else if (rec_name.compare("timer")==0){
        queue_sizeVector.record(n_clients_in_queue); // queue size every timer interval
        scheduleAt(simTime()+par("timerInterval").doubleValue(), qtimerMessage);
    }

}

void QueueA::finish()
{
    EV << "TOTAL NUMBER OF CLIENTS " << tot_n_clients << endl;
    recordStatistic(&iaTimeHistogram);
    recordStatistic(&procTimeHistogram);
}

// functions

/**
 * search all tills for an idle
 * priority for the till with the smallest Number (distance from the queue increases with the till Number)
 * if an empty (idle) till is found, return Till Number and mark till as in use
 * if no tills are available, return -1
 */
int QueueA::find_empty_till(void){
    int till_n = -1;
    for (int i=0; i<N_TILLS; i++){ // find an empty till
        if (empty_till_array[i]==0){
            till_n = i;
            empty_till_array[i] = 1; // allocate till
            break;
        }
    }
    return till_n;
}

/**
 * collect interarrival time for each new client:
 * mark the client entry time in the queue
 */
void QueueA::collect_new_client_entry_data(void){
    simtime_t iaTime = simTime() - lastArrival;
    iaTimeHistogram.collect(iaTime); // collect interarrival times
    lastArrival = simTime();

    if (n_clients_in_queue < QUEUE_CONTROL_SIZE){ // prevents buffer overflow
        entryQueueTime[n_clients_in_queue] = simTime(); // mark client time entering the queue, he/she is placed in the last position (FIFO queue)
    }

    n_clients_in_queue++; // increase the number of clients currently in the queue
    tot_n_clients++; // increase total number of client that entered the queue
}

/**
 * collect data for a client that is being sent to a till:
 * record total time in the queue
 * mark the till Number where the client was sent
 * mark the time the client left the queue
 *
 */
void QueueA::collect_client_dispatch_data (int till_to_send){
    time_in_queueVector.record(simTime() - entryQueueTime[0]); // calculate client queue time
    for (int k=1; k<QUEUE_CONTROL_SIZE-1; k++){ // move the queue recorded times
        entryQueueTime[k-1] = entryQueueTime[k];
    }
    entryQueueTime[QUEUE_CONTROL_SIZE-1] = 0;

    // find the number of the client at the head of the queue, and assign it to till N (till_to_send)
    queue_control_position[till_to_send] = tot_n_clients - n_clients_in_queue;

    sent_to_tillTime[till_to_send] = simTime(); // mark exit time from the queue for the client in till N

}

/**
 * send client to designated till
 * add a delay value, corresponding to the distance from the till
 * for higher till numbers, the time to reach the till increases
 * delay  = (till_n + 1)*delta
 * the target till remains in idle while the message does not reach it
 */
void QueueA::dispatch_client(int till_to_send){
    std::string tmp = std::to_string(till_to_send);
    const char *num_char = tmp.c_str();
    char out_port[] = "t_out";
    strcat(out_port, num_char); // output port name

    Till2queue *job = new Till2queue("client");
    job->setTill_n(till_to_send);
    sendDelayed(job, (1+till_to_send)*(par("deltaInterval").doubleValue()), out_port);

    n_clients_in_queue--; // remove client from the queue

}

/**
 * save the processing time of the client (histogram)
 * mark the client number in the client order vector (used to identify each client to its processing time)
 * mark the client processing time (same order as the client order vector)
 * mark which till was used by the client
 * mark the total time the client spent in the till, adding the time to reach it (delay)
 */
void QueueA::collect_processing_data(int rec_till_n, simtime_t procTime){
    procTimeHistogram.collect(procTime);
    client_proc_orderVector.record(queue_control_position[rec_till_n]);
    proc_timeVector.record(procTime);
    till_proc_orderVector.record(rec_till_n);
    client_till_timeVector.record(simTime() - sent_to_tillTime[rec_till_n] );
}

}; // namespace
