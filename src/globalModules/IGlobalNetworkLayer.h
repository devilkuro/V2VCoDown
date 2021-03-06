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

#ifndef IGLOBALNETWORKLAYER_H_
#define IGLOBALNETWORKLAYER_H_
#include "IGlobalControlUnit.h"
#include "cmessage.h"

class IGlobalNetworkLayer {
public:
    IGlobalNetworkLayer() {};
    virtual ~IGlobalNetworkLayer(){};

public:
    virtual void registerGCU(IGlobalControlUnit* gcu) = 0;
    virtual void unregisterGCU(IGlobalControlUnit* gcu) = 0;
    virtual void refreshGCU(IGlobalControlUnit* gcu) = 0;
    virtual void sendMsg(cMessage* msg) = 0;
    virtual void sendMsgToAP(int apid,cMessage* msg) = 0;
};

#endif /* IGLOBALNETWORKLAYER_H_ */
