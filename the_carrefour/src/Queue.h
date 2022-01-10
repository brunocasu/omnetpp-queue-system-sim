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

#define N_TILLS     1

/**
 * Message queue; see NED file for more info.
 */
class Queue : public cSimpleModule
{
  private:
    // state
    simtime_t lastArrival;

    // statistics
    cHistogram iaTimeHistogram;
    cOutVector arrivalsVector;

    int empty_till_array[N_TILLS] = {0}; // 0 is empty, 1 is processing
    int n_clients_in_queue = 0;
    //int n_tills = 1;


  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};

}; // namespace

#endif
