#include "Sounds.hpp"
void florestabackground(){
    PlaySoundA((LPCSTR) "resources/florestasons.wav", NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
}
void cavaloandando(){
    PlaySoundA((LPCSTR) "resources/horse_trot.wav", NULL, SND_ASYNC|SND_FILENAME);
}
void roar(){
    PlaySoundA((LPCSTR) "resources/roar.wav", NULL, SND_ASYNC|SND_FILENAME);
}


