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

#include "Spawner.h"

namespace ctrltwr {

Define_Module(Spawner);

Spawner::Spawner()
    : newSpawn_(nullptr)
{
}

Spawner::~Spawner()
{
    cancelAndDelete(newSpawn_);
}

void Spawner::initialize()
{
    newSpawn_ = new cMessage("newSpawn");
    planesBalance_ = 0;
	planeExitedSig_ = registerSignal("exited");

    WATCH(planesBalance_);

    // start
    simtime_t tn = par("tn");
    scheduleAt(simTime() + tn, newSpawn_);
}

void Spawner::handleMessage(cMessage* msg)
{
    // Message from self
    if (msg->isSelfMessage())
        handleNewSpawn();
    else
        handleIncomingAirplane(check_and_cast<Airplane*>(msg));
}

void Spawner::handleNewSpawn()
{
    Airplane* a = new Airplane("Airplane", ASK_TO_LAND);
    a->setTl(par("tl"));
    a->setTp(par("tp"));
    a->setTo(par("to"));
    a->setSchedulingPriority(0);

    ++planesBalance_;

    send(a, "toAirport");

    simtime_t tn = par("tn");
    scheduleAt(simTime() + tn, newSpawn_);
}

void Spawner::handleIncomingAirplane(Airplane* a)
{
    --planesBalance_;
    ASSERT(planesBalance_ >= 0);

	emit(planeExitedSig_, 1);

    delete a;
}

} // namespace