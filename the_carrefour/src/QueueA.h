#ifndef __THE_CARREFOUR_QUEUEA_H
#define __THE_CARREFOUR_QUEUEA_H

#include <omnetpp.h>
#include "till2queue_m.h"

#include <string>
#include <bits/stdc++.h>

using namespace omnetpp;

namespace the_carrefour {

#define N_TILLS     3
#define QUEUE_CONTROL_SIZE  20000 // maximum size for time entering control


class QueueA : public cSimpleModule
{
    public:
        QueueA();
        virtual ~QueueA();

    private:
    cMessage *qtimerMessage;

    simtime_t lastArrival;
    simtime_t entryQueueTime[QUEUE_CONTROL_SIZE]; // vector to keep track of clients entry time in the queue
    simtime_t sent_to_tillTime[N_TILLS]; // vector to keep track of clients entry time in the till

    cHistogram iaTimeHistogram; // interarrival time histogram
    cHistogram procTimeHistogram; // processing time histogram
    cOutVector time_in_queueVector; // time each client spent on the queue (order follows client number)

    cOutVector client_proc_orderVector; // order in which clients were processed (data is the client number)
    cOutVector proc_timeVector; // processing time, stored in the order that clients were processed (to match previous vector client order)
    cOutVector till_proc_orderVector; // identifies the till number that processed the client

    cOutVector client_till_timeVector; // time that the client spent during the processing plus the time spent to reach the till
    cOutVector queue_sizeVector; // show the queue size in every minute
    cOutVector queue_progressionVector; // show the queue size whenever a client enters it or exits it (accounted after the action)

    int queue_control_position [N_TILLS]; // array used to track the Client number assigned at each Till number
    int tot_n_clients = 0; // total number of clients that entered the system
    int head_queue_client_n = 0; // Client number at the first position of the queue
    int empty_till_array[N_TILLS] = {0}; // identify if a till is in idle or processing: 0 is empty(idle), 1 is processing
    int n_clients_in_queue = 0; // current queue size

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    int find_empty_till(void);
    void collect_new_client_entry_data(void);
    void collect_client_dispatch_data (int till_to_send);
    void dispatch_client(int till_to_send);
    void collect_processing_data(int rec_till_n, simtime_t procTime);
};


}; // namespace

#endif
