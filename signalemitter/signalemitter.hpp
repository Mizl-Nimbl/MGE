#ifndef signalemitter_hpp
#define signalemitter_hpp

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>

class SignalEmitter
{
public:
    SignalEmitter();

    void emit(int signal);
    void connect(int signal, std::function<void()> callback);
    void disconnect(int signal);

private:
    std::unordered_map<int, std::vector<std::function<void()>>> signals;
};


#endif /* signalemitter_hpp */