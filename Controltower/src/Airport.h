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

#ifndef __CONTROLTOWER_AIRPORT_H_
#define __CONTROLTOWER_AIRPORT_H_

#include "Airplane_m.h"
#include "common.h"
#include <omnetpp.h>
#include <queue>

using namespace omnetpp;

namespace ctrltwr {

/**
 * TODO - Generated class
 */
class Airport : public cSimpleModule {
	// Queues for planes waiting to land and to takeoff, respectively
    cQueue landQueue_, toffQueue_;

	// Number of planes in the parking zone at any moment
    int numParked_;

	// State of the runway
    bool isRunwayEmpty_;

	simsignal_t landQueueTimeSig_, landQueueLenSig_, toffQueueTimeSig_, toffQueueLenSig_, numParkedSig_;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage* msg);

    /**
     * Handler for incoming planes asking to land.
     *
     * If the runway is empty puts the plane in the landing state and marks the
	 * runway as busy, otherwise puts the plane in the `landQueue_`.
     */
    void handleAskToLand(Airplane*);

	/**
	 * Handler for planes that have finished the landing.
	 * 
	 * Puts the plane in the parking state and schedules a new plane in the runway,
	 * if there are any.
	 */
    void handleLanding(Airplane*);

	/**
	 * Handler for planes that have finished the parking.
	 * 
	 * It the runway is empty puts the airplane in the landing state and marks the
	 * runway as busy, otherwise puts the plane in the `toffQueue_`.
	 */
    void handleParking(Airplane*);

	/**
	 * Handler for planes that have finished the takeoff.
	 * 
	 * Sends a plane back to the spawner and schedules a new plane in the runway,
	 * if there are any. 
	 */
    void handleTakeoff(Airplane*);

	/**
	 * If there are planes waiting to land takes the first from the queue and puts 
	 * it in the runway and returns true, otherwise returns false.
	 */
    bool tryServeOneFromLandQueue();

	/**
	 * If there are planes waiting to takeoff takes the first from the queue and
	 * puts it in the runway and returns true, otherwise returns false.
	 */
    bool tryServeOneFromToffQueue();
};

#endif

} // namespace