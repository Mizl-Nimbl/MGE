#include "audiobank.hpp"

Audiobank::Audiobank() 
{
    std::cout << "Audiobank constructor called" << std::endl;
}

void Audiobank::queueAudio(int signal)
{
    for (int i = 0; i < audiobank.size(); i++)
    {
        if (audiobank[i]->getqueuesignal() == signal)
        {
            audiobank[i]->queue();
            break;
        }
    }
}

void Audiobank::playaudiobank(std::vector<Audio*> audiobank)
{
    if (audiobank.size() == 0)
    {
        std::cout << "No audio files to play" << std::endl;
        return;
    }
    for (int i = 0; i < audiobank.size(); i++)
    {
        if (audiobank[i]->getqueued())
        {
            audiobank[i]->play();
        }
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