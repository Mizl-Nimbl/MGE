#include "lualinker.hpp"

void LuaLinker::initialize()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    loadscripts("/home/mizl/Documents/MGE/assets/scripts");
    registerfunctions();
}

void LuaLinker::loadscripts(std::string directory)
{
    for (auto entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".lua")
        {
            std::string scriptPath = entry.path().string();
            if (luaL_dofile(L, scriptPath.c_str()) != LUA_OK)
            {
                std::cerr << "Error loading script: " << scriptPath << "\n";
                std::cerr << lua_tostring(L, -1) << "\n";
                lua_pop(L, 1);
            }
            pushscript(scriptPath);
        }
    }
}

void LuaLinker::pushscript(std::string script)
{
    scripts.push_back(script);
}

void LuaLinker::runscripts()
{
    for (auto script : scripts)
    {
        if (luaL_dostring(L, script.c_str()) != LUA_OK)
        {
            std::cerr << "Error running script: " << script << "\n";
            std::cerr << lua_tostring(L, -1) << "\n";
            lua_pop(L, 1);
        }
    }
}

void LuaLinker::registerfunctions()
{
    luabridge::getGlobalNamespace(L)
        .beginClass<Shader>("Shader")
        .addConstructor<void (*)()>()
        .addFunction("readShader", &Shader::readShader)
        .addFunction("use", &Shader::use)
        .addFunction("setBool", &Shader::setBool)
        .addFunction("setInt", &Shader::setInt)
        .addFunction("setFloat", &Shader::setFloat)
        .addFunction("setVec2", &Shader::setVec2)
        .addFunction("setVec3", &Shader::setVec3)
        .addFunction("setVec4", &Shader::setVec4)
        .addFunction("setMat2", &Shader::setMat2)
        .addFunction("setMat3", &Shader::setMat3)
        .addFunction("setMat4", &Shader::setMat4)
        .endClass()
        /*
        .beginClass<Textures>("Textures")
        .addConstructor<void (*)()>()
        .addFunction("loadTexture", &Textures::initTexture)
        .addFunction("loadCubemap", &Textures::initCubemap)
        .endClass()
        */
        .beginClass<SignalEmitter>("SignalEmitter")
        .addConstructor<void (*)()>()
        .addFunction("emit", &SignalEmitter::emit)
        .addFunction("connect", &SignalEmitter::connect)
        .addFunction("disconnect", &SignalEmitter::disconnect)
        .endClass()
        .beginClass<Scene>("Scene")
        .addConstructor<void (*)()>()
        .addFunction("getModels", &Scene::getModels)
        .addFunction("getTexts", &Scene::getTexts)
        .addFunction("getLights", &Scene::getLights)
        .addFunction("getPhysicsType", &Scene::getPhysicsType)
        .addFunction("getModel", &Scene::getModel)
        .addFunction("getLight", &Scene::getLight)
        .addFunction("getGlobalLocation", &Scene::getGlobalLocation)
        .addFunction("getGlobalRotation", &Scene::getGlobalRotation)
        .addFunction("getGlobalScale", &Scene::getGlobalScale)
        .addFunction("getModelLocation", &Scene::getModelLocation)
        .addFunction("getModelRotation", &Scene::getModelRotation)
        .addFunction("getModelScale", &Scene::getModelScale)
        .addFunction("getTextLocation", &Scene::getTextLocation)
        .addFunction("getTextRotation", &Scene::getTextRotation)
        .addFunction("getTextScale", &Scene::getTextScale)
        .addFunction("getTextColor", &Scene::getTextColor)
        .addFunction("getLightLocation", &Scene::getLightLocation)
        .addFunction("getLightDirection", &Scene::getLightDirection)
        .addFunction("getLightColor", &Scene::getLightColor)
        .addFunction("getLightType", &Scene::getLightType)
        .addFunction("setModel", &Scene::setModel)
        .addFunction("setPhysicsType", &Scene::setPhysicsType)
        .addFunction("setText", &Scene::setText)
        .addFunction("setGlobalLocation", &Scene::setGlobalLocation)
        .addFunction("setGlobalRotation", &Scene::setGlobalRotation)
        .addFunction("setGlobalScale", &Scene::setGlobalScale)
        .addFunction("setModelLocation", &Scene::setModelLocation)
        .addFunction("setModelRotation", &Scene::setModelRotation)
        .addFunction("setModelScale", &Scene::setModelScale)
        .addFunction("setTextLocation", &Scene::setTextLocation)
        .addFunction("setTextRotation", &Scene::setTextRotation)
        .addFunction("setTextScale", &Scene::setTextScale)
        .addFunction("setTextColor", &Scene::setTextColor)
        .addFunction("setLightLocation", &Scene::setLightLocation)
        .addFunction("setLightDirection", &Scene::setLightDirection)
        .addFunction("setLightColor", &Scene::setLightColor)
        .addFunction("setLightType", &Scene::setLightType)
        .endClass()
        .beginClass<Light>("Light")
        .addConstructor<void (*)()>()
        .addFunction("setDirection", &Light::setDirection)
        .addFunction("setPosition", &Light::setPosition)
        .addFunction("setConstant", &Light::setConstant)
        .addFunction("setLinear", &Light::setLinear)
        .addFunction("setQuadratic", &Light::setQuadratic)
        .addFunction("setCutOff", &Light::setCutOff)
        .addFunction("setOuterCutOff", &Light::setOuterCutOff)
        .addFunction("setAlpha", &Light::setAlpha)
        .addFunction("setRed", &Light::setRed)
        .addFunction("setGreen", &Light::setGreen)
        .addFunction("setBlue", &Light::setBlue)
        .addFunction("setType", &Light::setType)
        .addFunction("getDirection", &Light::getDirection)
        .addFunction("getPosition", &Light::getPosition)
        .addFunction("getConstant", &Light::getConstant)
        .addFunction("getLinear", &Light::getLinear)
        .addFunction("getQuadratic", &Light::getQuadratic)
        .addFunction("getCutOff", &Light::getCutOff)
        .addFunction("getOuterCutOff", &Light::getOuterCutOff)
        .addFunction("getAlpha", &Light::getAlpha)
        .addFunction("getRed", &Light::getRed)
        .addFunction("getGreen", &Light::getGreen)
        .addFunction("getBlue", &Light::getBlue)
        .addFunction("getType", &Light::getType)
        .endClass()
        /*
        .beginClass<Audio*>("Audio")
        .addConstructor<void (*)()>()
        .addFunction("data_callback", &Audio::data_callback)
        .addFunction("queue", &Audio::queue)
        .addFunction("dequeue", &Audio::dequeue)
        .addFunction("play", &Audio::play)
        .addFunction("setvolume", &Audio::setvolume)
        .addFunction("setpitch", &Audio::setpitch)
        .addFunction("setloop", &Audio::setloop)
        .addFunction("set3dposition", &Audio::set3dposition)
        .addFunction("set3dvelocity", &Audio::set3dvelocity)
        .addFunction("set3ddistance", &Audio::set3ddistance)
        .addFunction("set3droloff", &Audio::set3droloff)
        .addFunction("set3dspread", &Audio::set3dspread)
        .addFunction("setqueuesignal", &Audio::setqueuesignal)
        .addFunction("getvolume", &Audio::getvolume)
        .addFunction("getpitch", &Audio::getpitch)
        .addFunction("getloop", &Audio::getloop)
        .addFunction("get3dposition", &Audio::get3dposition)
        .addFunction("get3dvelocity", &Audio::get3dvelocity)
        .addFunction("get3ddistance", &Audio::get3ddistance)
        .addFunction("get3droloff", &Audio::get3droloff)
        .addFunction("get3dspread", &Audio::get3dspread)
        .addFunction("getqueuesignal", &Audio::getqueuesignal)
        .addFunction("getqueued", &Audio::getqueued)
        .endClass()
        */
        .beginClass<Physics>("Physics")
        .addConstructor<void (*)()>()
        .addFunction("getObjPos", &Physics::getObjPos)
        .addFunction("getObjVel", &Physics::getObjVel)
        .addFunction("getObjAcc", &Physics::getObjAcc)
        .addFunction("setObjPos", &Physics::setObjPos)
        .addFunction("setObjVel", &Physics::setObjVel)
        .addFunction("setObjAcc", &Physics::setObjAcc)
        .addFunction("nudge", &Physics::nudge)
        .endClass();
}

void LuaLinker::close()
{
    lua_close(L);
}


