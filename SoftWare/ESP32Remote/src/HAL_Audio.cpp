#include "HAL.h"
#include "TonePlayer/TonePlayer.h"


static TonePlayer player;

#include "TonePlayer/MusicCode.h"

static void Tone_Callback(uint32_t freq, uint16_t volume)
{
    Buzz_Tone(freq);
}

void Audio_Init()
{
    player.SetCallback(Tone_Callback);
}

void Audio_Update()
{
    player.Update(millis());
}

bool Audio_PlayMusic(const char* name)
{
    bool retval = false;
    for (int i = 0; i < sizeof(MusicList) / sizeof(MusicList[0]); i++)
    {
        if (strcmp(name, MusicList[i].name) == 0)
        {
            player.Play(MusicList[i].mc, MusicList[i].length);
            retval = true;
            break;
        }
    }
    return retval;
}
