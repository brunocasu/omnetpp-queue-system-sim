#ifndef __THE_CARREFOUR_TILLA_H
#define __THE_CARREFOUR_TILLA_H

#include <omnetpp.h>
#include "till2queue_m.h"

using namespace omnetpp;

namespace the_carrefour {


class TillA : public cSimpleModule
{
  private:
    simtime_t procTimeVal;
    simtime_t constProcVal;
    simtime_t beginProcTime;
    int is_proc = 0;
    int is_cfg_msg = 0;
    int till_number = 0;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

}; // namespace

#endif
