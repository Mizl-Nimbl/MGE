#include "scene.hpp"

Scene::Scene()
{
    location = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
    physicsType = 0;
}

Scene::Scene(glm::vec3 loc, glm::vec3 rot, glm::vec3 sca)
{
    location = loc;
    rotation = rot;
    scale = sca;
    physicsType = 0;
}

Scene::Scene(std::string path)
{
    int modelCount = 0;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) 
    {
        std::cerr << "Failed to load XML file: " << path << std::endl;
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    if (!root) 
    {
        std::cerr << "No root element in XML file: " << path << std::endl;
    }

    glm::vec3 alocation;
    tinyxml2::XMLElement* locationElement = root->FirstChildElement("Location");
    if (locationElement) {
        locationElement->QueryFloatAttribute("x", &alocation.x);
        locationElement->QueryFloatAttribute("y", &alocation.y);
        locationElement->QueryFloatAttribute("z", &alocation.z);
    }
    location = alocation;

    glm::vec3 arotation;
    tinyxml2::XMLElement* rotationElement = root->FirstChildElement("Rotation");
    if (rotationElement) {
        rotationElement->QueryFloatAttribute("x", &arotation.x);
        rotationElement->QueryFloatAttribute("y", &arotation.y);
        rotationElement->QueryFloatAttribute("z", &arotation.z);
    }
    rotation = arotation;

    glm::vec3 ascale;
    tinyxml2::XMLElement* scaleElement = root->FirstChildElement("Scale");
    if (scaleElement) {
        scaleElement->QueryFloatAttribute("x", &ascale.x);
        scaleElement->QueryFloatAttribute("y", &ascale.y);
        scaleElement->QueryFloatAttribute("z", &ascale.z);
    }
    scale = ascale;

    for (tinyxml2::XMLElement* modelElement = root->FirstChildElement("Model"); modelElement; modelElement = modelElement->NextSiblingElement("Model")) 
    {
        const char* modelPath = modelElement->FirstChildElement("Path")->GetText();
        if (!modelPath)
        {
            std::cerr << "No model path in XML file: " << path << std::endl;
        }
        Model model(modelPath);
        models.push_back(model);

        glm::vec3 blocation;
        tinyxml2::XMLElement* modelLocationElement = modelElement->FirstChildElement("Location");
        if (modelLocationElement) {
            modelLocationElement->QueryFloatAttribute("x", &blocation.x);
            modelLocationElement->QueryFloatAttribute("y", &blocation.y);
            modelLocationElement->QueryFloatAttribute("z", &blocation.z);
            setModelLocation(models.size() - 1, blocation);
        }

        glm::vec3 brotation;
        tinyxml2::XMLElement* modelRotationElement = modelElement->FirstChildElement("Rotation");
        if (modelRotationElement) {
            modelRotationElement->QueryFloatAttribute("x", &brotation.x);
            modelRotationElement->QueryFloatAttribute("y", &brotation.y);
            modelRotationElement->QueryFloatAttribute("z", &brotation.z);
            setModelRotation(models.size() - 1, brotation);
        }

        glm::vec3 bscale;
        tinyxml2::XMLElement* modelScaleElement = modelElement->FirstChildElement("Scale");
        if (modelScaleElement) {
            modelScaleElement->QueryFloatAttribute("x", &bscale.x);
            modelScaleElement->QueryFloatAttribute("y", &bscale.y);
            modelScaleElement->QueryFloatAttribute("z", &bscale.z);
            setModelScale(models.size() - 1, bscale);
        }

        modelCount++;
    }

    for (tinyxml2::XMLElement* lightElement = root->FirstChildElement("Audio"); lightElement; lightElement = lightElement->NextSiblingElement("Audio")) 
    {
        const char* audioPath = lightElement->FirstChildElement("Path")->GetText();
        if (!audioPath)
        {
            std::cerr << "No audio path in XML file: " << path << std::endl;
        }
        Audio* audio = new Audio(audioPath);

        float vol;
        tinyxml2::XMLElement* audioVolumeElement = lightElement->FirstChildElement("Volume");
        if (audioVolumeElement) 
        {
            audioVolumeElement->QueryFloatText(&vol);
            audio->setvolume(vol);
        }
        int qsig;
        tinyxml2::XMLElement* audioQueueSignalElement = lightElement->FirstChildElement("QueueSignal");
        if (audioQueueSignalElement) 
        {
            audioQueueSignalElement->QueryIntText(&qsig);
            audio->setqueuesignal(qsig);
        }
        bool loop;
        tinyxml2::XMLElement* audioLoopElement = lightElement->FirstChildElement("Loop");
        if (audioLoopElement) 
        {
            audioLoopElement->QueryBoolText(&loop);
            audio->setloop(loop);
        }
        glm::vec3 pos;
        tinyxml2::XMLElement* audioPositionElement = lightElement->FirstChildElement("Position");
        if (audioPositionElement) 
        {
            audioPositionElement->QueryFloatAttribute("x", &pos.x);
            audioPositionElement->QueryFloatAttribute("y", &pos.y);
            audioPositionElement->QueryFloatAttribute("z", &pos.z);
            audio->set3dposition(pos.x, pos.y, pos.z);
        }
        glm::vec3 vel;
        tinyxml2::XMLElement* audioVelocityElement = lightElement->FirstChildElement("Velocity");
        if (audioVelocityElement) 
        {
            audioVelocityElement->QueryFloatAttribute("x", &vel.x);
            audioVelocityElement->QueryFloatAttribute("y", &vel.y);
            audioVelocityElement->QueryFloatAttribute("z", &vel.z);
            audio->set3dvelocity(vel.x, vel.y, vel.z);
        }
        std::cout << "Connecting signal for audio: " << audio->getqueuesignal() << std::endl;
        e.connect(audio->getqueuesignal(), [this, audio]() { b.queueAudio(audio->getqueuesignal()); });
        std::cout << "Connected signal for audio: " << audio->getqueuesignal() << std::endl;
        audios.push_back(audio);
    }
    b.audiobank.insert(b.audiobank.end(), audios.begin(), audios.end());
    for (tinyxml2::XMLElement* signalElement = root->FirstChildElement("PhysicsType"); signalElement; signalElement = signalElement->NextSiblingElement("PhysicsType")) 
    {
        signalElement->QueryIntText(&physicsType);
    }
    for (tinyxml2::XMLElement* textElement = root->FirstChildElement("Text"); textElement; textElement = textElement->NextSiblingElement("Text")) 
    {
        const char* fontPath = textElement->FirstChildElement("Font")->GetText();
        if (!fontPath)
        {
            std::cerr << "No font path in XML file: " << path << std::endl;
        }
        Font font(fontPath);
        glm::vec2 textPos;
        std::string text = textElement->FirstChildElement("Data")->GetText();
        if (text != "")
        {
            setText(font, text);
        }
        tinyxml2::XMLElement* textLocationElement = textElement->FirstChildElement("Location");
        if (textLocationElement)
        {
            textLocationElement->QueryFloatAttribute("x", &textPos.x);
            textLocationElement->QueryFloatAttribute("y", &textPos.y);
            setTextLocation(font, textPos);
            std::cout << "Text location: " << textPos.x << ", " << textPos.y << std::endl;
        }
        float textRot;
        if (textElement->FirstChildElement("Rotation")) 
        {
            textElement->FirstChildElement("Rotation")->QueryFloatText(&textRot);
            setTextRotation(font, textRot);
            std::cout << "Text rotation: " << textRot << std::endl;
        }
        float textScale;
        if (textElement->FirstChildElement("Scale")) 
        {
            textElement->FirstChildElement("Scale")->QueryFloatText(&textScale);
            setTextScale(font, textScale);
            std::cout << "Text scale: " << textScale << std::endl;
        }
        glm::vec3 textColor;
        tinyxml2::XMLElement* textColorElement = textElement->FirstChildElement("Color");
        if (textColorElement) 
        {
            textColorElement->QueryFloatAttribute("r", &textColor.x);
            textColorElement->QueryFloatAttribute("g", &textColor.y);
            textColorElement->QueryFloatAttribute("b", &textColor.z);
            textColor = glm::vec3(textColor.x / 255.0f, textColor.y / 255.0f, textColor.z / 255.0f);
            setTextColor(font, textColor);
            std::cout << "Text color: " << textColor.x << ", " << textColor.y << ", " << textColor.z << std::endl;
        }
        texts.push_back(font);
        std::cout << "Text object loaded." << std::endl;
    }

    for (tinyxml2::XMLElement* lightElement = root->FirstChildElement("Light"); lightElement; lightElement = lightElement->NextSiblingElement("Light")) 
    {
        int red; 
        int green;
        int blue;
        int alpha;
        glm::vec3 position;
        glm::vec3 direction;
        float attenuation;
        int type;
        tinyxml2::XMLElement* lightPositionElement = lightElement->FirstChildElement("Location");
        glm::vec3 pos;
        if (lightPositionElement) 
        {
            lightPositionElement->QueryFloatAttribute("x", &pos.x);
            lightPositionElement->QueryFloatAttribute("y", &pos.y);
            lightPositionElement->QueryFloatAttribute("z", &pos.z);
            position = pos;
        }
        glm::vec3 dir;
        tinyxml2::XMLElement* lightDirectionElement = lightElement->FirstChildElement("Direction");
        if (lightDirectionElement) 
        {
            lightDirectionElement->QueryFloatAttribute("x", &dir.x);
            lightDirectionElement->QueryFloatAttribute("y", &dir.y);
            lightDirectionElement->QueryFloatAttribute("z", &dir.z);
            direction = dir;
        }
        glm::vec4 col;
        tinyxml2::XMLElement* lightColorElement = lightElement->FirstChildElement("Color");
        if (lightColorElement) 
        {
            lightColorElement->QueryFloatAttribute("r", &col.x);
            lightColorElement->QueryFloatAttribute("g", &col.y);
            lightColorElement->QueryFloatAttribute("b", &col.z);
            lightColorElement->QueryFloatAttribute("a", &col.w);
            red = col.x;
            green = col.y;
            blue = col.z;
            alpha = col.w;
        }
        int t;
        tinyxml2::XMLElement* lightTypeElement = lightElement->FirstChildElement("Type");
        if (lightTypeElement) 
        {
            lightTypeElement->QueryIntText(&t);
            type = t;
        }
        float att;
        tinyxml2::XMLElement* lightAttenElement = lightElement->FirstChildElement("AttenuationMod");
        if (lightAttenElement) 
        {
            lightAttenElement->QueryFloatText(&att);
            attenuation = att;
            std::cout << "Attenuation: " << att << std::endl;
        }
        if (type == 0)
        {
            Light light(red, green, blue, alpha, dir);
            lights.push_back(light);
            std::cout << "DirectionalLight loaded." << std::endl;
        }
        if (type == 1)
        {
            Light light(red, green, blue, alpha, pos, attenuation);
            lights.push_back(light);
            std::cout << "PointLight loaded." << std::endl;
        }
        if (type == 2)
        {
            Light light(red, green, blue, alpha, pos, dir, attenuation);
            lights.push_back(light);
            std::cout << "SpotLight loaded." << std::endl;
        }
        
    }

    std::cout << "Loaded scene with " << modelCount << " models and " << audios.size() << " audio files and " << texts.size() << " text objects and " << lights.size() << " lights." << std::endl;
}

Model Scene::getModel(int index)
{
    return models[index];
}

std::vector<Model> Scene::getModels()
{
    return models;
}

std::vector<Light> Scene::getLights()
{
    return lights;
}

int Scene::getPhysicsType()
{
    return physicsType;
}

glm::vec3 Scene::getGlobalLocation()
{
    return location;
}

glm::vec3 Scene::getGlobalRotation()
{
    return rotation;
}

glm::vec3 Scene::getGlobalScale()
{
    return scale;
}

glm::vec3 Scene::getModelLocation(int index)
{
    return models[index].getLocation();
}

glm::vec3 Scene::getModelRotation(int index)
{
    return models[index].getRotation();
}

glm::vec3 Scene::getModelScale(int index)
{
    return models[index].getScale();
}

Light Scene::getLight(int index)
{
    return lights[index];
}

glm::vec3 Scene::getLightLocation(int index)
{
    return lights[index].getPosition();
}

glm::vec3 Scene::getLightDirection(int index)
{
    return lights[index].getDirection();
}

glm::vec4 Scene::getLightColor(int index)
{
    return glm::vec4(lights[index].getRed(), lights[index].getGreen(), lights[index].getBlue(), lights[index].getAlpha());
}

int Scene::getLightType(int index)
{
    return lights[index].getType();
}

void Scene::setPhysicsType(int newType)
{
    physicsType = newType;
}

void Scene::setModel(std::string const &modelpath)
{
    Model newModel(modelpath);
    models.push_back(newModel);
}

void Scene::setGlobalLocation(glm::vec3 newLocation)
{
    location = newLocation;
}

void Scene::setGlobalRotation(glm::vec3 newRotation)
{
    rotation = newRotation;
}

void Scene::setGlobalScale(glm::vec3 newScale)
{
    scale = newScale;
}

void Scene::setModelLocation(int index, glm::vec3 newLocation)
{
    models[index].setLocation(newLocation + location);
}

void Scene::setModelRotation(int index, glm::vec3 newRotation)
{
    models[index].setRotation(newRotation + rotation);
}

void Scene::setModelScale(int index, glm::vec3 newScale)
{
    models[index].setScale(newScale + scale);
}

void Scene::setText(Font& f, std::string text)
{
    f.setText(text);
    std::cout << "(SCENE) Text set to: " << text << std::endl;
}

void Scene::setTextLocation(Font& f, glm::vec2 newLocation)
{
    f.setPosition(newLocation);
}

void Scene::setTextRotation(Font& f, float newRotation)
{
    f.setRotation(newRotation);
}

void Scene::setTextScale(Font& f, float newScale)
{
    f.setScale(newScale);
}

void Scene::setTextColor(Font& f, glm::vec3 newColor)
{
    f.setColor(newColor);
}

std::vector<Font> Scene::getTexts()
{
    return texts;
}

glm::vec2 Scene::getTextLocation(int index)
{
    return texts[index].getPosition();
}

float Scene::getTextRotation(int index)
{
    return texts[index].getRotation();
}

float Scene::getTextScale(int index)
{
    return texts[index].getScale();
}

glm::vec3 Scene::getTextColor(int index)
{
    return texts[index].getColor();
}

void Scene::setLightLocation(int index, glm::vec3 newLocation)
{
    lights[index].setPosition(newLocation);
}

void Scene::setLightDirection(int index, glm::vec3 newRotation)
{
    lights[index].setDirection(newRotation);
}

void Scene::setLightColor(int index, glm::vec4 newColor)
{
    lights[index].setRed(newColor.r);
    lights[index].setGreen(newColor.g);
    lights[index].setBlue(newColor.b);
    lights[index].setAlpha(newColor.a);
}

void Scene::setLightType(int index, int type)
{
    lights[index].setType(type);
}
