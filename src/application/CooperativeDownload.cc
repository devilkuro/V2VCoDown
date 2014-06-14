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

#include "CooperativeDownload.h"

Define_Module(CooperativeDownload);

CooperativeDownload::CooperativeDownload() {
    // TODO
}

CooperativeDownload::~CooperativeDownload() {
    clearContentMap();
    clearTimeMap();
    cancelAndDelete(frameTimer);
}

void CooperativeDownload::finish() {
}
void CooperativeDownload::initialize(int stage)
{
    BaseGlobalAppLayer::initialize(stage);
    if(stage == 0){
        // initialize the gcu module
        gcu->setAddr(getParentModule()->getId());
        gcu->isAp(false);
        gcu->apid(-1);
        // initialize other parameters
        coreDebug = par("coreDebug").boolValue();
        debug = par("debug").boolValue();
        isTargetCar = par("isTargetCar").boolValue();
        taskSize = par("taskSize").doubleValue();
        frameSize = par("frameSize").doubleValue();
        frameInterval = par("frameInterval").doubleValue();
        frameTimer = new cMessage("frameTimer");

        car_Status = CAR_IDEL;
    }else if(stage == 1){

    }
}

void CooperativeDownload::handleSelfMsg(cMessage* msg) {
    if(msg == frameTimer){
        handleFrameTimer();
        delete msg;
    }else{
        debugEV<<"Error: Unknown self message."<<endl;
        delete msg;
    }
}

void CooperativeDownload::handleLowerControl(cMessage* msg) {
    CoDownCtrlMsg* cdmsg =check_and_cast<CoDownCtrlMsg*>(msg);
    if(cdmsg == NULL){
        debugEV <<"Error: CooperativeDownload::get a wrong type message."
                << endl;
        delete msg;
    } else {
        switch (cdmsg->getMsgType()) {
        case CDCMT_ConnectToAP:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_ConnectToAP: "
                    << cdmsg->getTargetId() << endl;
            connectToAP(cdmsg->getTargetId());
            break;
        case CDCMT_ConnectToGCU:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_ConnectToGCU: "
                    << cdmsg->getTargetId() << endl;
            connectToCar(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectFromAP:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromAP: "
                    << cdmsg->getTargetId() << endl;
            sayByeToAp(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectFromGCU:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_DisconnectFromGCU: "
                    << cdmsg->getTargetId() << endl;
            disconnectFromCar(cdmsg->getTargetId());
            break;
        case CDCMT_DisconnectAll:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_DisconnectAll"
                    << endl;
            selfReset();
            break;
        case CDCMT_UpdatePostion:
            debugEV
                    << "CooperativeDownload::handleLowerMsg::CDCMT_UpdatePostion"
                    << endl;
            break;
        default:
            break;
        }
        delete cdmsg;
    }
}

void CooperativeDownload::sayHelloToAp(int apid) {
    // TODO
}

void CooperativeDownload::sayByeToAp(int apid) {
    // TODO
}

void CooperativeDownload::disconnectFromCurrentCar() {
    targetID = LAddress::L3NULL;
    car_Status = CAR_IDEL;
    if (!isTargetCar) {
        cancelEvent(frameTimer);
    }else{
        if(!untappedCarList.empty()){

        }
    }
}

void CooperativeDownload::connectToCar(int carid) {
    // TODO

}

void CooperativeDownload::disconnectFromCar(int carid) {
    untappedCarList.remove(carid);
    if (carid == targetID) {
        disconnectFromCurrentCar();
    }
}


void CooperativeDownload::askForDownload(int apid) {
}

void CooperativeDownload::selfReset() {
    clearContentMap();
    clearTimeMap();
    untappedCarList.clear();
    disconnectFromCurrentCar();
}

void CooperativeDownload::handleLowerMsg(cMessage* msg) {
    // TODO
}

void CooperativeDownload::clearContentMap() {
    for(CD_SQUEUE_MAP::iterator it = contentQueueMap.begin();
            it!=contentQueueMap.end();it++){
        it->second->clear();
    }
    contentQueueMap.clear();
}

void CooperativeDownload::connectToAP(int apid) {
    if(isTargetCar){
        askForDownload(apid);
    }else{
        sayHelloToAp(apid);
    }
}

void CooperativeDownload::disconnectFromAP(int apid) {
    if(isTargetCar){

    }else{

    }
}

void CooperativeDownload::handleFrameTimer() {
    if (!isTargetCar) {
        if (car_Status == CAR_SENDING||car_Status == CAR_PRESENDING) {
            // start sending data to target car
            car_Status = CAR_SENDING;
            SegmentQueue* content = contentQueueMap[targetID]->getFirstSegment(frameSize);
            contentQueueMap[targetID]->remove(*content);
            {
                CoDownContentMsg* cdmsg = new CoDownContentMsg();
                cdmsg->setMsgType(CDMT_Send);
                cdmsg->setSrcAddr(gcu->getAddr());
                cdmsg->setDestAddr(targetID);
                cdmsg->setStartPos(content->getFirstStart());
                cdmsg->setEndPos(content->getFirstEnd());
                cdmsg->setLastMsg(contentQueueMap[targetID]->empty());
                sendDown(cdmsg);
            }
            delete content;
            if(!contentQueueMap[targetID]->empty()){
                scheduleAt(simTime()+frameInterval,frameTimer);
            }
        }else{
            debugEV<<"Error: Start sending process from an unknown status."<<endl;
        }
    }else{

    }
}

void CooperativeDownload::clearTimeMap() {
    for(CD_SQUEUE_MAP::iterator it = timeQueueMap.begin();
            it!=timeQueueMap.end();it++){
        it->second->clear();
    }
    timeQueueMap.clear();
}
