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
    //lastArrival = simTime();
    //iaTimeHistogram.setName("interarrival times");
    //arrivalsVector.setName("arrivals");
    //arrivalsVector.setInterpolationMode(cOutVector::NONE);
}

void Till::handleMessage(cMessage *msg)
{
    //simtime_t d = simTime() - lastArrival;

    //iaTimeHistogram.collect(d);
    //arrivalsVector.record(1);

    //lastArrival = simTime();

    std::string message_name = msg->getName();

    Till2queue *tempMsg;
    tempMsg = (Till2queue*)msg;
    //string rec_msg = tempMsg->getMsg;

    if (message_name.compare("client")==0){ // received a client
        if (is_cfg_msg == 0){ // first message configures the Till
            till_number = tempMsg->getTill_n();
            is_cfg_msg = 1;
        }
        //is_proc = 1;
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
        //is_proc = 0;
        EV << "TILL " << till_number << " PROCESSED CLIENT" << endl;

    }

    delete msg;
    // delete tempMsg;
}

void Till::finish()
{
    //recordStatistic(&iaTimeHistogram);
}

}; // namespace


