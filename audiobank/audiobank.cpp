#include "audiobank.hpp"

void Audiobank::playaudiobank(std::vector<Audio*> audiobank)
{
    for (int i = 0; i < audiobank.size(); i++)
    {
        audiobank[i]->play();
    }
}

void Audiobank::loadaudio()
{
    //iterate through audio folder and push to audiobank
    std::string audioFolderPath = "/home/mizl/Documents/MGE/assets/audio";
    for (const auto& entry : std::filesystem::directory_iterator(audioFolderPath))
    {
        if (entry.path().extension() == ".mp3")
        {
            Audio* audio = new Audio(entry.path().string().c_str());
            audiobank.push_back(audio);
        }
    }
}