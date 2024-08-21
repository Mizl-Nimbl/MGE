#include "audio.hpp"

Audio::Audio()
{}

Audio::~Audio()
{
    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
    if (deviceinit) {
        ma_device_uninit(&device);
    }
    if (decoderinit) {
        ma_decoder_uninit(&decoder);
    }
    std::cout << "Audio uninitialized and stopped successfully" << std::endl;
}

Audio::Audio(const char* filename)
{
    deviceinit = false;
    decoderinit = false;
    volume = 1.0f;
    pitch = 1.0f;
    loop = false;
    queued = false;
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    distance = 1.0f;
    rolloff = 1.0f;
    spread = 1.0f;

    ma_result result = ma_decoder_init_file(filename, NULL, &decoder);
    // Initialize the engine
    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to initialize audio engine" << std::endl;
        return;
    }

    // Initialize the sound
    result = ma_sound_init_from_file(&engine, filename, MA_SOUND_FLAG_STREAM, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to load audio file" << std::endl;
        ma_engine_uninit(&engine);
        return;
    }
}

void Audio::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL) {
        return;
    }

    ma_result result = ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);
    if (result != MA_SUCCESS) {
        return;
    }

    (void)pInput;
}

void Audio::queue()
{
    queued = true;
}

void Audio::dequeue()
{
    queued = false;
}

void Audio::play()
{
    if (queued == true)
    {
        ma_sound_set_volume(&sound, volume);
        ma_sound_set_pitch(&sound, pitch);
        ma_sound_set_looping(&sound, loop);
        ma_sound_set_position(&sound, position.x, position.y, position.z);
        ma_sound_set_velocity(&sound, velocity.x, velocity.y, velocity.z);
        //ma_sound_set_distance_model(&sound, MA_DISTANCE_MODEL_LINEAR_ROLLOFF);
        //ma_sound_set_rolloff(&sound, rolloff);
        //ma_sound_set_spread(&sound, spread);
        ma_sound_start(&sound);
        queued = false;
    }
    else
    {
        ma_sound_stop(&sound);
        ma_decoder_seek_to_pcm_frame(&decoder, 0);
    }
}

void Audio::setqueuesignal(int signal)
{
    queueSignal = signal;
}

void Audio::setvolume(float volume)
{
    this->volume = volume;
}

void Audio::setpitch(float pitch)
{
    this->pitch = pitch;
}

void Audio::setloop(bool loop)
{
    this->loop = loop;
}

void Audio::set3dposition(float x, float y, float z)
{
    this->position = glm::vec3(x, y, z);
}

void Audio::set3dvelocity(float x, float y, float z)
{
    this->velocity = glm::vec3(x, y, z);
}

void Audio::set3ddistance(float distance)
{
    this->distance = distance;
}

void Audio::set3droloff(float rolloff)
{
    this->rolloff = rolloff;
}

void Audio::set3dspread(float spread)
{
    this->spread = spread;
}

float Audio::getvolume()
{
    return volume;
}

float Audio::getpitch()
{
    return pitch;
}

bool Audio::getloop()
{
    return loop;
}

glm::vec3 Audio::get3dposition()
{
    return position;
}

glm::vec3 Audio::get3dvelocity()
{
    return velocity;
}

float Audio::get3ddistance()
{
    return distance;
}

float Audio::get3droloff()
{
    return rolloff;
}

float Audio::get3dspread()
{
    return spread;
}

int Audio::getqueuesignal()
{
    return queueSignal;
}

bool Audio::getqueued()
{
    return queued;
}