/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once


namespace Systems {

	class Logging {

	public:
		Logging() = default;
		~Logging();

		void Initialize();

	private:
		bool m_Initialized = false;
	};

}
