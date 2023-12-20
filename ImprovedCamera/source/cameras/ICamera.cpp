/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

// Precompiled Header
#include "PCH.h"

#include "cameras/ICamera.h"

namespace Interface {

	ICamera::ICamera(const std::string& name, std::int32_t idx) :
		m_Name(name), m_ID(idx)
	{
		m_pluginConfig = DLLMain::Plugin::Get()->Config();
	}

	Camera::~Camera()
	{
		for (ICamera* camera : m_Cameras)
		{
			camera->OnShutdown();
			delete camera;
		}
	}

	void Camera::Register(ICamera* camera)
	{
		m_Cameras.emplace(m_Cameras.begin() + m_CameraInsertIndex, camera);
		m_CameraInsertIndex++;
	}

	void Camera::Unregister(ICamera* camera)
	{
		auto it = std::find(m_Cameras.begin(), m_Cameras.begin() + m_CameraInsertIndex, camera);

		if (it != m_Cameras.begin() + m_CameraInsertIndex)
		{
			camera->OnShutdown();
			m_Cameras.erase(it);
			m_CameraInsertIndex--;
		}
	}

}
