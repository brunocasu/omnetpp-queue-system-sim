#ifndef __THE_CARREFOUR_QUEUEB_H
#define __THE_CARREFOUR_QUEUEB_H

#include <omnetpp.h>
#include "till2queue_m.h"

#include <string>
#include <bits/stdc++.h>

using namespace omnetpp;

namespace the_carrefour {

#define N_TILLS     10
#define QUEUE_CONTROL_SIZE  2000 // maximum size for time entering control


class QueueB : public cSimpleModule
{
    public:
        QueueB();
        virtual ~QueueB();


    private:
    cMessage *qtimerMessage;

    simtime_t lastArrival;
    simtime_t entryQueueTime;
    simtime_t sent_to_tillTime;

    cHistogram iaLocalTimeHistogram; // inter arrival time histogram for this queue
    cHistogram procTimeHistogram; // all queues processing time histogram
    cOutVector time_in_queueVector; // time each client spent on the queue (sequential client number)
    cOutVector proc_timeVector; // processing time, stored in the order that clients were processed (to match previous vector client order)

    cOutVector queue_sizeVector; // show the queue size in every minute
    cOutVector queue_progressionVector; // show the queue size whenever a client enters it or exits it (accounted after the action)

    int queue_control_position [N_TILLS];
    int tot_n_clients = 0;
    int head_queue_client_n = 0;
    int empty_till_ctrl = 0; // identify if the local till is in idle or processing: 0 is empty(idle), 1 is processing
    int n_clients_in_queue = 0;
    int queue_number;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    void collect_new_client_entry_data(void);
    void dispatch_client(void);
    void collect_processing_data(simtime_t procTime);
};


}; // namespace

#endif
