#pragma once
#include "Audio.h"
#include <assert.h>

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