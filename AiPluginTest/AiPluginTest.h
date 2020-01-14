#pragma once
#include <thread>
#include "../Interface.h"

class AiPluginTest : public PluginInterface
{
public:
    AiPluginTest(AiProcessor *pro):PluginInterface(pro){}
    ~AiPluginTest();
    bool Init(const std::string &params) override;
    void Start() override;
    void Stop() override;

private:
    bool running_ = false;
    std::thread work_thr_;
    int id_idex_ = 0;
};