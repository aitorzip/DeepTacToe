#include "abServerConfig.h"
#include "yaml-cpp/yaml.h"
#include <fstream>

ServerConfig::ServerConfig()
    : _loaded(false), _clientPerThread(3)
{
}

void ServerConfig::loadFromFile(const std::string& file)
{
    std::ifstream     ifs(file);
    std::stringstream buffer;

    buffer << ifs.rdbuf();

    loadFromBuffer(buffer.str());
}

void ServerConfig::loadFromBuffer(const std::string& yaml)
{
    YAML::Node root = YAML::Load(yaml);

    if(root.IsNull() )
        throw std::runtime_error("Failed to load yaml configuration");

    if(!root["iFaceAndPort"])
        throw std::runtime_error("Invalid yaml. Missing iFaceAndPort");

    _iFaceAndPort = root["iFaceAndPort"].as<std::string>();

    if(!root["clientPerThread"])
        throw std::runtime_error("Invalid yaml. Missing clientPerThread");

    _clientPerThread = root["clientPerThread"].as<uint32_t>();

    _loaded = true;
}

const std::string ServerConfig::genTemplate(void)
{
    YAML::Emitter yaml;

    yaml << YAML::BeginMap;

    yaml << YAML::Key << "iFaceAndPort";
    yaml << YAML::Value << "localhost:6666";
    yaml << YAML::Comment("It could be any:6666");

    yaml << YAML::Key << "clientPerThread";
    yaml << YAML::Value << 3;

    yaml << YAML::EndMap;

    return yaml.c_str();
}
