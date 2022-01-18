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

#include "Queue.h"

namespace the_carrefour {

Define_Module(Queue);

Queue::Queue()
{
    qtimerMessage = NULL;
}

Queue::~Queue()
{
    cancelAndDelete(qtimerMessage);
}

void Queue::initialize()
{
    lastArrival = simTime();
    iaTimeHistogram.setName("inter arrival times");
    iaTimeHistogram.setBinSizeHint(10);

    procTimeHistogram.setName("processing times");
    procTimeHistogram.setBinSizeHint(10);

    time_in_queueVector.setName("time spent on the queue");
    time_in_queueVector.setInterpolationMode(cOutVector::NONE);

    client_proc_orderVector.setName("client processing order");
    client_proc_orderVector.setInterpolationMode(cOutVector::NONE);

    proc_timeVector.setName("processing time with client order");
    proc_timeVector.setInterpolationMode(cOutVector::NONE);

    till_proc_orderVector.setName("till processing order");
    till_proc_orderVector.setInterpolationMode(cOutVector::NONE);

    client_till_timeVector.setName("client total time in the till");
    client_till_timeVector.setInterpolationMode(cOutVector::NONE);

    queue_sizeVector.setName("queue size per minute");
    queue_sizeVector.setInterpolationMode(cOutVector::NONE);

    queue_progressionVector.setName("queue size after modification");
    queue_progressionVector.setInterpolationMode(cOutVector::NONE);

    qtimerMessage = new cMessage("timer");
    scheduleAt(simTime()+par("timerInterval").doubleValue(), qtimerMessage);

}

void Queue::handleMessage(cMessage *msg)
{
    //ASSERT(msg==qtimerMessage);
    EV << "Received " << msg->getName() << endl;
    std::string rec_name = msg->getName();
    int till_to_send = -1; // assigned till number is undefined at the start

    if (rec_name.compare("client")==0){
        simtime_t d = simTime() - lastArrival;
        iaTimeHistogram.collect(d); // collect inter arrival times
        lastArrival = simTime();

        EV << "RECEIVED CLIENT N = "<< tot_n_clients << endl;
        EV << "CURRENT QUEUE: " << n_clients_in_queue << " CLIENT(S)" << endl;

        if (n_clients_in_queue < QUEUE_CONTROL_SIZE){ // prevents buffer overflow
            entryQueueTime[n_clients_in_queue] = simTime(); // mark client time entering the queue, he/she is placed in the last position (FIFO queue)
        }

        n_clients_in_queue++; // increase the number of clients in the queue
        tot_n_clients++;

        for (int i=0; i<N_TILLS; i++){ // find an empty till
            if (empty_till_array[i]==0){
                till_to_send = i;
                empty_till_array[i] = 1; // allocate till
                break;
            }
        }
        if ((till_to_send >= 0) && (till_to_send < N_TILLS)){
            time_in_queueVector.record(simTime() - entryQueueTime[0]); // calculate client queue time
            for (int k=1; k<QUEUE_CONTROL_SIZE-1; k++){ // move the queue recorded times
                entryQueueTime[k-1] = entryQueueTime[k];
            }
            entryQueueTime[QUEUE_CONTROL_SIZE-1] = 0;

            // find the number of the client at the head of the queue, and assign it to till N (till_to_send)
            queue_control_position[till_to_send] = tot_n_clients - n_clients_in_queue;

            sent_to_tillTime[till_to_send] = simTime();

            std::string tmp = std::to_string(till_to_send);
            const char *num_char = tmp.c_str();
            char out_port[] = "t_out";
            strcat(out_port, num_char); // output port name

            Till2queue *job = new Till2queue("client");
            job->setTill_n(till_to_send);
            //send(job, out_port);
            sendDelayed(job, (1+till_to_send)*(par("deltaInterval").doubleValue()), out_port);
            EV << "CLIENT TO TILL " << till_to_send << " ("<< out_port << ")" <<  endl;

            n_clients_in_queue--; // remove client from the queue
            queue_progressionVector.record(n_clients_in_queue);
            // mark exit time
            // calculate total time in the queue
        }
        else { // No Tills available
            EV << "ALL TILLS FULL" << endl;
            queue_progressionVector.record(n_clients_in_queue);
        }
        delete msg;
    }
    else if (rec_name.compare("empty")==0){
        Till2queue *tempMsg;
        tempMsg = (Till2queue*)msg;
        //string rec_msg = tempMsg->getMsg;
        int rec_till_n = tempMsg->getTill_n();
        simtime_t procTime = tempMsg->getProcTime();
        procTimeHistogram.collect(procTime);
        client_proc_orderVector.record(queue_control_position[rec_till_n]);
        proc_timeVector.record(procTime);
        till_proc_orderVector.record(rec_till_n);
        client_till_timeVector.record(simTime() - sent_to_tillTime[rec_till_n] );

        EV << "TILL " << rec_till_n << " IS FREE" << endl;

        if (n_clients_in_queue > 0 ){
            time_in_queueVector.record(simTime() - entryQueueTime[0]); // calculate client queue time
            for (int k=1; k<QUEUE_CONTROL_SIZE-1; k++){ // move the queue recorded times
                entryQueueTime[k-1] = entryQueueTime[k];
            }
            entryQueueTime[QUEUE_CONTROL_SIZE-1] = 0;

            // find the number of the client at the head of the queue, and assign it to the free till (till_to_send)
            queue_control_position[rec_till_n] = tot_n_clients - n_clients_in_queue;

            sent_to_tillTime[rec_till_n] = simTime();

            std::string tmp = std::to_string(rec_till_n);
            const char *num_char = tmp.c_str();
            char out_port[] = "t_out";
            strcat(out_port, num_char); // output port name

            Till2queue *job = new Till2queue("client");
            //send(job, out_port);
            sendDelayed (job, (1+rec_till_n)*(par("deltaInterval").doubleValue()), out_port);
            // client is sent with a delay (depending on the till number) = delta*j (2s to 20s)
            // delta = time to reach the till 0 (2s)
            // j = till number (0 to 9)
            EV << "CLIENT SENT TO " << out_port << endl;

            n_clients_in_queue--; // remove client from the queue
            queue_progressionVector.record(n_clients_in_queue);
            // mark exit time
            // calculate total time in the queue
            std::string Q;
            Q = std::to_string(n_clients_in_queue);
            EV << "CLIENT DISPATCHED - CURRENT QUEUE: " << Q << " CLIENT(S)" << endl;

            empty_till_array[rec_till_n] = 1; // take the till again
        }
        else{ // no clients in the queue
            empty_till_array[rec_till_n] = 0; // free the till
        }
        delete msg;
    }
    else if (rec_name.compare("timer")==0){
        queue_sizeVector.record(n_clients_in_queue); // queue size every timer interval
        scheduleAt(simTime()+par("timerInterval").doubleValue(), qtimerMessage);
    }

}

void Queue::finish()
{
    EV << "TOTAL NUMBER OF CLIENTS " << tot_n_clients << endl;
    recordStatistic(&iaTimeHistogram);
    recordStatistic(&procTimeHistogram);
}

}; // namespace
