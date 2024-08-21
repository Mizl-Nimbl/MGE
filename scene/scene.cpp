#include "scene.hpp"

Scene::Scene()
{
    location = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

Scene::Scene(glm::vec3 loc, glm::vec3 rot, glm::vec3 sca)
{
    location = loc;
    rotation = rot;
    scale = sca;
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

    for (tinyxml2::XMLElement* audioElement = root->FirstChildElement("Audio"); audioElement; audioElement = audioElement->NextSiblingElement("Audio")) 
    {
        const char* audioPath = audioElement->FirstChildElement("Path")->GetText();
        if (!audioPath)
        {
            std::cerr << "No audio path in XML file: " << path << std::endl;
        }
        Audio* audio = new Audio(audioPath);

        float vol;
        tinyxml2::XMLElement* audioVolumeElement = audioElement->FirstChildElement("Volume");
        if (audioVolumeElement) 
        {
            audioVolumeElement->QueryFloatText(&vol);
            audio->setvolume(vol);
        }
        int qsig;
        tinyxml2::XMLElement* audioQueueSignalElement = audioElement->FirstChildElement("QueueSignal");
        if (audioQueueSignalElement) 
        {
            audioQueueSignalElement->QueryIntText(&qsig);
            audio->setqueuesignal(qsig);
        }
        bool loop;
        tinyxml2::XMLElement* audioLoopElement = audioElement->FirstChildElement("Loop");
        if (audioLoopElement) 
        {
            audioLoopElement->QueryBoolText(&loop);
            audio->setloop(loop);
        }
        glm::vec3 pos;
        tinyxml2::XMLElement* audioPositionElement = audioElement->FirstChildElement("Position");
        if (audioPositionElement) 
        {
            audioPositionElement->QueryFloatAttribute("x", &pos.x);
            audioPositionElement->QueryFloatAttribute("y", &pos.y);
            audioPositionElement->QueryFloatAttribute("z", &pos.z);
            audio->set3dposition(pos.x, pos.y, pos.z);
        }
        glm::vec3 vel;
        tinyxml2::XMLElement* audioVelocityElement = audioElement->FirstChildElement("Velocity");
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
    std::cout << "Loaded scene with " << modelCount << " models and " << audios.size() << " audio files." << std::endl;
}

Model Scene::getModel(int index)
{
    return models[index];
}

std::vector<Model> Scene::getModels()
{
    return models;
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

