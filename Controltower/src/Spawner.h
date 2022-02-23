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

#ifndef __CONTROLTOWER_SPAWNER_H_
#define __CONTROLTOWER_SPAWNER_H_

#include "Airplane_m.h"
#include "common.h"
#include <omnetpp.h>

using namespace omnetpp;

namespace ctrltwr {

/**
 * TODO - Generated class
 */
class Spawner : public cSimpleModule {
    cMessage* newSpawn_;
    int planesBalance_;
	simsignal_t planeExitedSig_;

public:
    Spawner();
    ~Spawner();

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage* msg);

    void handleNewSpawn();
    void handleIncomingAirplane(Airplane*);
};

} // namespace

#endif
