#ifndef AUDIO_HPP
#define AUDIO_HPP

#define MINIAUDIO_IMPLEMENTATION
#include "../include/miniaudio/miniaudio.h"

#include <vector>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../systems/systems.hpp"
#include "../declarations.hpp"

class Audio
{
public:
    Audio();
    Audio(const char* filename);
    ~Audio();
    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    void queue();
    void dequeue();
    void play();
    void setvolume(float volume);
    void setpitch(float pitch);
    void setloop(bool loop);
    void set3dposition(float x, float y, float z);
    void set3dvelocity(float x, float y, float z);
    void set3ddistance(float distance);
    void set3droloff(float rolloff);
    void set3dspread(float spread);

    void setqueuesignal(int signal);

    float getvolume();
    float getpitch();
    bool getloop();
    glm::vec3 get3dposition();
    glm::vec3 get3dvelocity();
    float get3ddistance();
    float get3droloff();
    float get3dspread();

    int getqueuesignal();
    bool getqueued();
private:
    int queueSignal;
    bool deviceinit;
    bool decoderinit;
    float volume;
    float pitch;
    bool loop;
    bool queued;
    glm::vec3 position;
    glm::vec3 velocity;
    float distance;
    float rolloff;
    float spread;
    ma_decoder decoder;
    ma_device device;
    ma_engine engine;
    ma_sound sound;
};

#endif /* AUDIO_HPP */