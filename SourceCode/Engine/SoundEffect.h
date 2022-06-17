#pragma once
#include "Audio.h"
#include <assert.h>

// Used in TerrainAudio_Component
// The only difference between this and Audio is that this does not use a AudioStateMachine
class SoundEffect : public Audio
{
public:
    SoundEffect(std::wstring path) : Audio(path)
    {
        
    }
    void Play() override
    {
        if (isPlaying)
            return;
        HRESULT hr = sourceVoice->SubmitSourceBuffer(&buffer);
        assert(hr == S_OK);
        sourceVoice->Start();
        isPlaying = true;
    }
};