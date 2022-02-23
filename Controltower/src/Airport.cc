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

#include "Airport.h"

namespace ctrltwr {

Define_Module(Airport);

void Airport::initialize()
{
    landQueue_ = cQueue("landQueue");
    toffQueue_ = cQueue("toffQueue");
    numParked_ = 0;
    isRunwayEmpty_ = true;

	landQueueTimeSig_ = registerSignal("lqtime");
	toffQueueTimeSig_ = registerSignal("tqtime");
	numParkedSig_ = registerSignal("npark");
	landQueueLenSig_ = registerSignal("lqlen");
	toffQueueLenSig_ = registerSignal("tqlen");

    WATCH(numParked_);
    WATCH(isRunwayEmpty_);
}

void Airport::handleMessage(cMessage* msg)
{
    Airplane* a = check_and_cast<Airplane*>(msg);

    switch (a->getKind()) {
    case ASK_TO_LAND:
        handleAskToLand(a);
        break;
    case LANDING:
        handleLanding(a);
        break;
    case PARKING:
        handleParking(a);
        break;
    case TAKEOFF:
        handleTakeoff(a);
        break;
    default:
        throw cRuntimeError("Airport::handleMessage - Unexpected message: %s", msg->str().c_str());
    }
}

void Airport::handleAskToLand(Airplane* a)
{
    a->setTimestamp();
    landQueue_.insert(a);
	emit(landQueueLenSig_, landQueue_.getLength());

    if (isRunwayEmpty_)
        tryServeOneFromLandQueue();
}

void Airport::handleLanding(Airplane* a)
{
    ASSERT(!isRunwayEmpty_);

    // send airplane to parking area
    numParked_ += 1;
	emit(numParkedSig_, numParked_);

    a->setKind(PARKING);
	a->setSchedulingPriority(1);
    scheduleAt(simTime() + a->getTp(), a);

    isRunwayEmpty_ = true;

    tryServeOneFromLandQueue() || tryServeOneFromToffQueue();
}

void Airport::handleParking(Airplane* a)
{
    numParked_ -= 1;
	emit(numParkedSig_, numParked_);

    a->setTimestamp();
    toffQueue_.insert(a);
	emit(toffQueueLenSig_, toffQueue_.getLength());

    if (isRunwayEmpty_)
        tryServeOneFromToffQueue();
}

void Airport::handleTakeoff(Airplane* a)
{
    ASSERT(!isRunwayEmpty_);

    isRunwayEmpty_ = true;

	send(a, "toSpawner");

    tryServeOneFromLandQueue() || tryServeOneFromToffQueue();
}

bool Airport::tryServeOneFromLandQueue()
{
    ASSERT(isRunwayEmpty_);

    if (!landQueue_.getLength())
        return false;

    isRunwayEmpty_ = false;

    Airplane* a = check_and_cast<Airplane*>(landQueue_.pop());
	emit(landQueueLenSig_, landQueue_.getLength());

    simtime_t queue_time = simTime() - a->getTimestamp();
    emit(landQueueTimeSig_, queue_time);

    a->setKind(LANDING);
    scheduleAt(simTime() + a->getTl(), a);

    return true;
}

bool Airport::tryServeOneFromToffQueue()
{
    ASSERT(isRunwayEmpty_);

    if (!toffQueue_.getLength())
        return false;

    isRunwayEmpty_ = false;

    Airplane* a = check_and_cast<Airplane*>(toffQueue_.pop());
	emit(toffQueueLenSig_, toffQueue_.getLength());

    simtime_t queue_time = simTime() - a->getTimestamp();
    emit(toffQueueTimeSig_, queue_time);

    a->setKind(TAKEOFF);
    scheduleAt(simTime() + a->getTo(), a);

    return true;
}

} // namespace