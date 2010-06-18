// Copyright (C) 2003 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#include "Common.h"
#include "Thread.h"

#include "PulseAudioStream.h"

#define BUFFER_SIZE 4096
#define BUFFER_SIZE_BYTES (BUFFER_SIZE*2*2)

PulseAudio::PulseAudio(CMixer *mixer) : SoundStream(mixer), thread_data(0), handle(NULL)
{
	mix_buffer = new u8[BUFFER_SIZE_BYTES];
}

PulseAudio::~PulseAudio()
{
	delete [] mix_buffer;
}

static void *ThreadTrampoline(void *args)
{
	reinterpret_cast<PulseAudio *>(args)->SoundLoop();
	return NULL;
}

bool PulseAudio::Start()
{
	thread = new Common::Thread(&ThreadTrampoline, this);
	thread_data = 0;
	return true;
}

void PulseAudio::Stop()
{
	thread_data = 1;
	delete thread;
	thread = NULL;
}

void PulseAudio::Update()
{
	// don't need to do anything here.
}

// Called on audio thread.
void PulseAudio::SoundLoop()
{
	if (!PulseInit()) {
		thread_data = 2;
		return;
	}
	while (!thread_data)
	{
		int err;
		int frames_to_deliver = 512;
		m_mixer->Mix(reinterpret_cast<short *>(mix_buffer), frames_to_deliver);
		if (pa_simple_write(handle, mix_buffer, frames_to_deliver * 2 * 2, &err) < 0)
		{
			ERROR_LOG(AUDIO, "pa_simple_write fail: %s", pa_strerror(err));
		}
	}
	PulseShutdown();
	thread_data = 2;
}

bool PulseAudio::PulseInit()
{
	// The Sample format to use
	static const pa_sample_spec ss = {
		PA_SAMPLE_S16LE,
		m_mixer->GetSampleRate(),
		2
	};

	int err;

	if (!(handle = pa_simple_new(NULL, "dolphin-emu", PA_STREAM_PLAYBACK, NULL,
					"emulator", &ss, NULL, NULL, &err)))
	{
		ERROR_LOG(AUDIO, "PulseAudio open error: %s\n", pa_strerror(err));
		return false;
	}

	NOTICE_LOG(AUDIO, "Pulse successfully initialized.\n");
	return true;
}

void PulseAudio::PulseShutdown()
{
	if (handle != NULL)
	{
		pa_simple_free(handle);
		handle = NULL;
	}
}

