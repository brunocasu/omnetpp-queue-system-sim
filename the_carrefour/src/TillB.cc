/*
 * TillB.cc
 *
 *  Created on: 28 de dez. de 2021
 *      Author: bruno casu
 */

#include "TillB.h"

namespace the_carrefour {

Define_Module(TillB);

void TillB::initialize()
{

}

void TillB::handleMessage(cMessage *msg)
{
    std::string message_name = msg->getName();

    Till2queue *tempMsg;
    tempMsg = (Till2queue*)msg;

    if (message_name.compare("client")==0){ // received a client
        // EV << "RECEIVED CLIENT AT TILL " << endl;

        beginProcTime = simTime();
        Till2queue *fix_proc = new Till2queue("fixed_proc");
        constProcVal = par("minProcInterval").doubleValue();
        scheduleAt(simTime()+constProcVal, fix_proc); // add minimum processing time
    }
    else if (message_name.compare("fixed_proc")==0){
        Till2queue *proc_job = new Till2queue("end_proc");
        procTimeVal = par("procInterval").doubleValue();
        scheduleAt(simTime()+procTimeVal, proc_job); // add random time to processing
    }
    else if (message_name.compare("end_proc")==0){
        Till2queue *job = new Till2queue("empty");
        job->setProcTime(simTime() - beginProcTime);
        send(job, "out");
        // EV << "TILL " << " PROCESSED CLIENT" << endl;
    }
    delete msg;
}

void TillB::finish()
{

}

}; // namespace


