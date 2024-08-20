#include "audio.hpp"

Audio::Audio()
{}

Audio::~Audio()
{
    if (deviceinit == true)
    {
        ma_device_uninit(&device);
    }
    if (decoderinit == true)
    {
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
    if (result != MA_SUCCESS) 
    {
        std::cout << "Failed to load audio file" << std::endl;
        return;
    }
    decoderinit = true;

    ma_device_config deviceConfig = ma_device_config_init(ma_device_type_playback);;
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) 
    {
        std::cout << "Failed to initialize playback device" << std::endl;
        ma_decoder_uninit(&decoder);
        decoderinit = false;
        return;
    }

    deviceinit = true;

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) 
    {
        std::cout << "Failed to start playback device" << std::endl;
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        deviceinit = false;
        decoderinit = false;
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
        //ma_engine_set_volume_3d(&device, volume);
        //ma_engine_set_pitch_3d(&device, pitch);
        //ma_engine_set_looping_3d(&device, loop);
        //ma_engine_set_position_3d(&device, position.x, position.y, position.z);
        //ma_engine_set_velocity_3d(&device, velocity.x, velocity.y, velocity.z);
        //ma_engine_set_distance_model_3d(&device, MA_DISTANCE_MODEL_LINEAR_ROLLOFF);
        //ma_engine_set_rolloff_3d(&device, rolloff);
        //ma_engine_set_spread_3d(&device, spread);
        ma_device_start(&device);
    }
    else
    {
        ma_device_stop(&device);
        ma_decoder_seek_to_pcm_frame(&decoder, 0);
    }
    queued = false;
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