#include "signalemitter.hpp"

SignalEmitter::SignalEmitter() : signals() {}

void SignalEmitter::emit(int signal)
{
    for (auto& callback : signals[signal])
    {
        callback();
    }
}

void SignalEmitter::connect(int signal, std::function<void()> callback)
{
    std::cout << "Connecting signal " << signal << std::endl;
    std::cout << "Before connection, signals size: " << signals.size() << std::endl;
    signals[signal].push_back(callback);
    std::cout << "After connection, signals size: " << signals.size() << std::endl;
}

void SignalEmitter::disconnect(int signal)
{   
    std::cout << "Disconnecting signal " << signal << std::endl;
    std::cout << "Before disconnection, signals size: " << signals.size() << std::endl;
    signals.erase(signal);
    std::cout << "After disconnection, signals size: " << signals.size() << std::endl;
}