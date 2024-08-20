#ifndef audiobank_hpp
#define audiobank_hpp

#include <vector>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>

#include "../audio/audio.hpp"
#include "../declarations.hpp"

class Audiobank
{
public:
    void playaudiobank(std::vector<Audio*> audiobank); //play all audio in bank & dequeue
    void loadaudio(); //push to bank
    std::vector<Audio*> audiobank;
private:
};

#endif /* audiobank_hpp */