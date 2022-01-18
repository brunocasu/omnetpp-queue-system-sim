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

#ifndef __THE_CARREFOUR_SOURCE_H
#define __THE_CARREFOUR_SOURCE_H

#include <omnetpp.h>

using namespace omnetpp;

namespace the_carrefour {

/**
 * Generates messages; see NED file for more info.
 */
class Source : public cSimpleModule
{
  private:
    cMessage *newClientMessage;
    cMessage *timerMessage;
    cOutVector partialClientsVector; // number of new clients in the timer interval
    int n_clients_sent = 0;
    int prev_count = 0;
    int partial_n = 0;
    int intital_set = 0;

  public:
     Source();
     virtual ~Source();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

}; // namespace

#endif
