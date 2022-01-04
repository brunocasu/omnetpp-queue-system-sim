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

void Queue::initialize()
{
    lastArrival = simTime();
    iaTimeHistogram.setName("inter arrival times");
    arrivalsVector.setName("arrivals");
    arrivalsVector.setInterpolationMode(cOutVector::NONE);

}

void Queue::handleMessage(cMessage *msg)
{
    simtime_t d = simTime() - lastArrival;
    EV << "Received " << msg->getName() << endl;
    std::string Q;
    std::string rec_name = msg->getName();
    int till_to_send = -1;
    if (rec_name.compare("client")==0){
        iaTimeHistogram.collect(d);
        arrivalsVector.record(1);
        lastArrival = simTime();

        n_clients_in_queue++; // add client to the queue
        // mark arrival time at queue

        Q = std::to_string(n_clients_in_queue);
        EV << "CLIENT RECEIVED - CURRENT QUEUE: " << Q << " CLIENT(S)" << endl;

        for (int i=0; i<N_TILLS; i++){ // find an empty till
            if (empty_till_array[i]==0){
                till_to_send = i;
                empty_till_array[i] = 1; // allocate till
                break;
            }
        }
        if ((till_to_send >= 0) && (till_to_send < N_TILLS)){
            //EV << "CLIENT DISPATCHED TO TILL: " << till_to_send << endl;
            std::string tmp = std::to_string(till_to_send);
            const char *num_char = tmp.c_str();
            char out_port[] = "t_out";
            strcat(out_port, num_char); // output port name

            Till2queue *job = new Till2queue("client");
            job->setTill_n(till_to_send);
            send(job, out_port);
            EV << "SENT TO TILL " << till_to_send << " ("<< out_port << ")" <<  endl;

            n_clients_in_queue--; // remove client from the queue
            // mark exit time
            // calculate total time in the queue

            Q = std::to_string(n_clients_in_queue);
            EV << "CLIENT DISPATCHED - CURRENT QUEUE: " << Q << " CLIENT(S)" << endl;
        }
        else { // No Tills available
            EV << "ALL TILLS FULL" << endl;
        }
    }
    else if (rec_name.compare("empty")==0){
        Till2queue *tempMsg;
        tempMsg = (Till2queue*)msg;
        //string rec_msg = tempMsg->getMsg;
        int rec_till_n = tempMsg->getTill_n();

        EV << "TILL " << rec_till_n << " IS FREE" << endl;

        if (n_clients_in_queue > 0 ){
            std::string tmp = std::to_string(rec_till_n);
            const char *num_char = tmp.c_str();
            char out_port[] = "t_out";
            strcat(out_port, num_char); // output port name

            Till2queue *job = new Till2queue("client");
            send(job, out_port);
            EV << "CLIENT SENT TO " << out_port << endl;

            n_clients_in_queue--; // remove client from the queue
            // mark exit time
            // calculate total time in the queue

            Q = std::to_string(n_clients_in_queue);
            EV << "CLIENT DISPATCHED - CURRENT QUEUE: " << Q << " CLIENT(S)" << endl;

            empty_till_array[rec_till_n] = 1; // take the till again
        }
        else{ // no clients in the queue
            empty_till_array[rec_till_n] = 0; // free the till
        }
    }

    delete msg;
}

void Queue::finish()
{
    recordStatistic(&iaTimeHistogram);
}

}; // namespace
