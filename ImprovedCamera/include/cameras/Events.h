/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace ImprovedCamera {

	struct CameraEvent {

		enum Event : std::uint8_t
		{
			kFirstPerson = 0,
			kFirstPersonCombat,
			kFurniture,
			kCrafting,
			kKillmove,
			kRagdoll,
			kDeath,
			kCannibal,
			kHorse,
			kHorseCombat,
			kHorseTransition,
			kDragon,
			kDragonCombat,
			kDragonTransition,
			kVampireLord,
			kWerewolf,
			kTransform,
			kNecroLich,
			kScripted,
			kThirdPerson,

			kTotal
		};
	};

}
