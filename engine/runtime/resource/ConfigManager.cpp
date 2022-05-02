#include <fstream>
#include <string>

#include "ConfigManager.h"
namespace zeus
{
    void zeus::ConfigManager::initialize(const EngineInitParams& init_param)
    {
        m_root_folder = init_param.m_root_folder;
        // read configs
        std::ifstream config_file(init_param.m_config_file_path);
        std::string   config_line;
        while (std::getline(config_file, config_line))
        {
            size_t seperate_pos = config_line.find_first_of('=');
            if (seperate_pos > 0 && seperate_pos < (config_line.length() - 1))
            {
                std::string name = config_line.substr(0, seperate_pos);
                std::string value = config_line.substr(seperate_pos + 1, config_line.length() - seperate_pos - 1);
                if (name.compare("ShaderFolder") == 0)
                {
                    m_shader_folder = m_root_folder / value;
                }
            }
        }
    }

    void zeus::ConfigManager::clear()
    {
        m_root_folder.clear();
        m_shader_folder.clear();
    }

    const std::filesystem::path& zeus::ConfigManager::getRootFolder() const
    {
        return m_root_folder;
    }

    const std::filesystem::path& zeus::ConfigManager::getShaderFolder() const
    {
        return m_shader_folder;
    }
    
}