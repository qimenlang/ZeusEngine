#include "facade.h"

#include <iostream>
#include <memory>
#include <string>

namespace design {

// ================= 子系统类 =================
class LightingSystem {
   public:
    void setBrightness(int level) {
        std::cout << "灯光设置亮度: " << level << "%\n";
    }

    void turnOff() { std::cout << "所有灯光已关闭\n"; }
};

class AirConditionSystem {
   public:
    void setTemperature(double temp) {
        std::cout << "空调设置温度: " << temp << "°C\n";
    }

    void setMode(const std::string& mode) {
        std::cout << "空调模式: " << mode << "\n";
    }
};

class SecuritySystem {
   public:
    void arm() { std::cout << "安全系统已布防\n"; }

    void disarm() { std::cout << "安全系统已撤防\n"; }

    void lockDoors() { std::cout << "所有门已上锁\n"; }
};

// ================= Facade类 =================
class SmartHomeFacade {
   private:
    static SmartHomeFacade facade;
    std::weak_ptr<LightingSystem> lighting;
    std::weak_ptr<AirConditionSystem> ac;
    std::weak_ptr<SecuritySystem> security;
    SmartHomeFacade() = default;

   public:
    void init(std::shared_ptr<LightingSystem> lights,
              std::shared_ptr<AirConditionSystem> ac,
              std::shared_ptr<SecuritySystem> secu) {
        lighting = lights;
        ac = ac;
        security = secu;
    }

    static SmartHomeFacade& instance() {
        static SmartHomeFacade facade;
        return facade;
    }
    // 离家模式：关闭所有设备并布防
    void leaveHome() {
        std::cout << "\n=== 离家模式 ===\n";
        lighting.lock()->turnOff();
        ac.lock()->setTemperature(26.0);  // 设置节能温度
        security.lock()->lockDoors();
        security.lock()->arm();
    }

    // 回家模式：开启舒适环境
    void arriveHome() {
        std::cout << "\n=== 回家模式 ===\n";
        security.lock()->disarm();
        lighting.lock()->setBrightness(70);
        ac.lock()->setTemperature(22.0);
        ac.lock()->setMode("comfort");
    }

    // 影院模式：优化环境设置
    void theaterMode() {
        std::cout << "\n=== 影院模式 ===\n";
        lighting.lock()->setBrightness(20);
        ac.lock()->setTemperature(21.0);
        ac.lock()->setMode("静音");
        security.lock()->lockDoors();
    }
};

// ================= 客户端代码 =================
void facadeTest::run() {
    auto lightSys = std::make_shared<LightingSystem>();
    auto acsys = std::make_shared<AirConditionSystem>();
    auto secusys = std::make_shared<SecuritySystem>();
    auto& facade = SmartHomeFacade::instance();
    facade.init(lightSys, acsys, secusys);

    // 客户端通过简单接口控制系统
    facade.arriveHome();
    facade.theaterMode();
    facade.leaveHome();
}
}  // namespace design