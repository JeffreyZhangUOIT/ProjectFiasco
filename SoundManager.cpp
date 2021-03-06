/*
Copyright 2017, 2018 Jeffrey Zhang

This file is part of ProjectFiasco.

ProjectFiasco is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

ProjectFiasco is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ProjectFiasco.  If not, see <http://www.gnu.org/licenses/>.

The MIT License (MIT)

Copyright (c) 2018 Microsoft Corp

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "SoundManager.h"
using namespace DirectX;


SoundManager::SoundManager(float x) {
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
	#ifdef _DEBUG
		eflags = eflags | AudioEngine_Debug;
	#endif
	m_audEngine = std::make_unique<AudioEngine>(eflags);
	m_retryAudio = false;
	m_Solitude = std::make_unique<SoundEffect>(m_audEngine.get(), L"modSolitude.wav");
	std::random_device rd;
	m_random = std::make_unique<std::mt19937>(rd());

	m_solitudeLoop = m_Solitude->CreateInstance();
	masterVolume = x;
}

SoundManager::~SoundManager() {
	if (m_audEngine)
	{
		m_audEngine->Suspend();
	}

	m_solitudeLoop.reset();
}

void SoundManager::Update(int gamemode) {
	if (m_retryAudio)
	{
		m_retryAudio = false;

		if (m_audEngine->Reset())
		{
			// TODO: restart any looped sounds here
			if (m_solitudeLoop)
				m_solitudeLoop->Play(true);
		}
	}
	else if (!m_audEngine->Update())
	{
		if (m_audEngine->IsCriticalError())
		{
			m_retryAudio = true;
		}
	}
	m_solitudeLoop->SetVolume(masterVolume);
	if (gamemode == 1) {
		m_solitudeLoop->Play();
	}
	if (gamemode == 0) {
		m_solitudeLoop->Stop();
	}
	if (gamemode == 2) {
		m_solitudeLoop->Play();
	}
	
}

void SoundManager::Suspend() {
	m_audEngine->Suspend();
}

void SoundManager::Resume() {
	m_audEngine->Resume();
}

void SoundManager::Reset() {
	m_retryAudio = true;
}

float SoundManager::setVolume(float vol) {
	if (vol > 1) {
		vol = 1;
	}
	if (vol < 0) {
		vol = 0;
	}
	masterVolume = vol;
	return masterVolume;
}

float SoundManager::changeVolume(float delta) {
	masterVolume += delta;
	if (masterVolume > 1) {
		masterVolume = 1.0f;
	}
	if (masterVolume < 0) {
		masterVolume = 0.0f;
	}
	return masterVolume;
}
