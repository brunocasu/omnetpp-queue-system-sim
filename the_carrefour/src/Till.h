#ifndef __THE_CARREFOUR_TILL_H
#define __THE_CARREFOUR_TILL_H

#include <omnetpp.h>

using namespace omnetpp;

namespace the_carrefour {


class Till : public cSimpleModule
{
  private:
    // state
    simtime_t lastArrival;

    // statistics
    cDoubleHistogram iaTimeHistogram;
    cOutVector arrivalsVector;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

}; // namespace

#endif
