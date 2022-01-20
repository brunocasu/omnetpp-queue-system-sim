/*
 * TillA.cc
 *
 *  Created on: 28 de dez. de 2021
 *      Author: bruno casu
 */

#include "TillA.h"

namespace the_carrefour {

Define_Module(TillA);

void TillA::initialize()
{

}

void TillA::handleMessage(cMessage *msg)
{
    std::string message_name = msg->getName();

    Till2queue *tempMsg;
    tempMsg = (Till2queue*)msg;

    if (message_name.compare("client")==0){ // received a client
        if (is_cfg_msg == 0){ // first message configures the Till
            till_number = tempMsg->getTill_n();
            is_cfg_msg = 1;
        }
        EV << "RECEIVED CLIENT AT TILL " << till_number << endl;

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
        job->setTill_n(till_number);
        job->setProcTime(simTime() - beginProcTime);
        send(job, "out");
        EV << "TILL " << till_number << " PROCESSED CLIENT" << endl;

    }
    delete msg;
}

void TillA::finish()
{

}

}; // namespace


