#pragma once
#include "Audio.h"
#include <assert.h>

class SoundEffect : public AUDIO
{
public:
    SoundEffect(std::wstring path) : AUDIO(path)
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