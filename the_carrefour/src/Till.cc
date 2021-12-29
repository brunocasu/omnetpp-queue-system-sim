/*
 * Tills.cc
 *
 *  Created on: 28 de dez. de 2021
 *      Author: bruno casu
 */

#include "Till.h"

namespace the_carrefour {

Define_Module(Till);

void Till::initialize()
{
    lastArrival = simTime();
    iaTimeHistogram.setName("interarrival times");
    arrivalsVector.setName("arrivals");
    arrivalsVector.setInterpolationMode(cOutVector::NONE);
}

void Till::handleMessage(cMessage *msg)
{
    simtime_t d = simTime() - lastArrival;
    EV << "Received " << msg->getName() << endl;
    delete msg;

    iaTimeHistogram.collect(d);
    arrivalsVector.record(1);

    lastArrival = simTime();

    ASSERT(msg==timerMessage);

    cMessage *job = new cMessage("empty");
    send(job, "out");

}

void Till::finish()
{
    recordStatistic(&iaTimeHistogram);
}

}; // namespace


