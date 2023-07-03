/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "plugin.h"
#include "cameras/Events.h"
#include "skyrimse/Helper.h"

#if _DEBUG
	#include <iostream>
	#include <iomanip>
#endif


namespace Interface {

	class ICamera {

	protected:
		struct Data {
			const bool* EventActive;
			const float* FOV;
			const float* NearDistance;
		} m_Data{};

		std::string m_Name{};
		std::int32_t m_ID = -1;

		RE::PlayerCharacter* Player = nullptr;
		Systems::Config* m_pluginConfig = nullptr;

	public:
		ICamera(const std::string& name = "ICamera", std::int32_t idx = -1);
		virtual ~ICamera() = default;

		virtual bool OnUpdate(std::uint8_t currentCamera, std::uint8_t previousCamera) = 0;
		virtual bool HasControl() = 0;
		virtual std::uint8_t GetEventID() = 0;
		virtual std::uint8_t GetStateID() = 0;
		virtual const Data& GetData() const = 0;
		virtual void OnShutdown() = 0;

		const std::string& GetName() const { return m_Name; };
		const std::int32_t GetID() const { return m_ID; };
	};

	class Camera {

	public:
		Camera() = default;
		~Camera();

		void Register(ICamera* camera);
		void Unregister(ICamera* camera);

		std::vector<ICamera*>::iterator begin() { return m_Cameras.begin(); }
		std::vector<ICamera*>::iterator end() { return m_Cameras.end(); }
		std::vector<ICamera*>::reverse_iterator rbegin() { return m_Cameras.rbegin(); }
		std::vector<ICamera*>::reverse_iterator rend() { return m_Cameras.rend(); }

		std::vector<ICamera*>::const_iterator begin() const { return m_Cameras.begin(); }
		std::vector<ICamera*>::const_iterator end() const { return m_Cameras.end(); }
		std::vector<ICamera*>::const_reverse_iterator rbegin() const { return m_Cameras.rbegin(); }
		std::vector<ICamera*>::const_reverse_iterator rend() const { return m_Cameras.rend(); }

	private:
		std::vector<ICamera*> m_Cameras{};
		std::uint32_t m_CameraInsertIndex = 0;
	};

}
