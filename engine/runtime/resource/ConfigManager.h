#include <filesystem>
#include "../core/base/singleton.h"

namespace zeus {
    struct EngineInitParams
    {
        std::filesystem::path m_root_folder;
        std::filesystem::path m_config_file_path;
    };

    class ConfigManager final : public singleton<ConfigManager>
    {
        friend class singleton<ConfigManager>;

    public:
        ConfigManager(const ConfigManager&) = delete;
        ConfigManager& operator=(const ConfigManager&) = delete;
        ConfigManager() = default;

    private:
        std::filesystem::path m_root_folder;
        std::filesystem::path m_shader_folder;


    public:
        void initialize(const EngineInitParams& init_param);
        void clear();
        const std::filesystem::path& getRootFolder() const;
        const std::filesystem::path& getShaderFolder() const;
    };
}