#include <iostream>
// #include "include/editor.h"
// #include "../../runtime/resource/ConfigManager.h"

using namespace std;

// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html
#define GET_ZEUS_STR(s) GET_STR(s)
#define GET_STR(s) #s

int main() {
  // std::filesystem::path zeus_root_folder =
  // std::filesystem::path(GET_ZEUS_STR(ZEUS_ROOT_DIR)); zeus::EngineInitParams
  // zeus_init_paras; zeus_init_paras.m_root_folder = zeus_root_folder;
  // zeus_init_paras.m_config_file_path = zeus_root_folder / "ZeusEditor.ini";
  // zeus::ConfigManager::instance().initialize(zeus_init_paras);

  cout << "zeus editor start." << endl;
  // zeus::editor zeus_editor;
  // zeus_editor.run();

  return 0;
}
