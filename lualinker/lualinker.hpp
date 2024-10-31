#ifndef lua_linker_hpp
#define lua_linker_hpp

#include <iostream>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
#include <filesystem>
#include <string>

#include "../shaders/shader.hpp"
//#include "../textures/textures.hpp"
#include "../signalemitter/signalemitter.hpp"
#include "../scene/scene.hpp"
#include "../light/light.hpp"
#include "../physics/physics.hpp"
#include "../declarations.hpp"

#include <vector>

class LuaLinker
{
public:
    lua_State* L;
    void initialize();
    void loadscripts(std::string directory);
    void close();
    void pushscript(std::string script);
    void runscripts();
    void registerfunctions();
private:
    std::vector<std::string> scripts;
};

#endif /* lua_linker_hpp */