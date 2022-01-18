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

#ifndef __THE_CARREFOUR_QUEUE_H
#define __THE_CARREFOUR_QUEUE_H

#include <omnetpp.h>
#include "till2queue_m.h"

#include <string>
#include <bits/stdc++.h>

using namespace omnetpp;

namespace the_carrefour {

#define N_TILLS     5
#define TIMER_INTERVAL      60
#define QUEUE_CONTROL_SIZE  2000 // maximum size for time entering control

/**
 * Message queue; see NED file for more info.
 */
class Queue : public cSimpleModule
{
    public:
        Queue();
        virtual ~Queue();

    private:
    // state
    simtime_t lastArrival;
    //simtime_t sentToTillN[N_TILLS];
    simtime_t entryQueueTime[QUEUE_CONTROL_SIZE];
    simtime_t sent_to_tillTime[N_TILLS];

    // statistics
    cHistogram iaTimeHistogram; // inter arrival time histogram
    cHistogram procTimeHistogram; // all queues processing time histogram
    cOutVector time_in_queueVector; // time each client spent on the queue (sequential client number)

    // next three vectors are correlated
    cOutVector client_proc_orderVector; // order in witch clients were processed (data is the client number)
    cOutVector proc_timeVector; // processing time, stored in the order that clients were processed (to match previous vector client order)
    cOutVector till_proc_orderVector; // identifies the till number that processed the client

    cOutVector client_till_timeVector; // time that the client spent during the processing plus the time spent to reach the till
    cOutVector queue_sizeVector; // show the queue size in every minute
    cOutVector queue_progressionVector; // show the queue size whenever a client enters it or exits it (accounted after the action)


    cMessage *qtimerMessage;

    int queue_control_position [N_TILLS];
    int tot_n_clients = 0;
    int head_queue_client_n = 0;
    int empty_till_array[N_TILLS] = {0}; // identify if a till is in idle or processing: 0 is empty(idle), 1 is processing
    int n_clients_in_queue = 0;
    //int n_tills = 1;


  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

}; // namespace

#endif
