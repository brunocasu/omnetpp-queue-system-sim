#ifndef __THE_CARREFOUR_SOURCEB_H
#define __THE_CARREFOUR_SOURCEB_H

#include <omnetpp.h>
#include "till2queue_m.h"

using namespace omnetpp;

#define N_TILLS     10

namespace the_carrefour {

/**
 * Generates messages; see NED file for more info.
 */
class SourceB : public cSimpleModule
{
  private:
    cMessage *newClientMessage;
    cMessage *timerMessage;
    //cMessage *initMessage;
    simtime_t lastArrival;

    cHistogram randomTillSelectionHist;
    cHistogram randomQueueSelectionHist;
    cHistogram iaGenerationHist;
    cOutVector partialClientsVector; // number of new clients in the timer interval
    int n_clients_sent = 0;
    int prev_count = 0;
    int partial_n = 0;
    int intital_set = 0;

    int queue_size_array[N_TILLS] = {0}; // tracks the size of each queue
    int empty_till_array[N_TILLS] = {0}; // identify if a till is in idle or processing: 0 is empty(idle), 1 is processing

  public:
     SourceB();
     virtual ~SourceB();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();

    int find_empty_till(void);
    int find_smallest_queue(void);
    void send_client_to_queue(int queue_n);
};

}; // namespace

#endif
