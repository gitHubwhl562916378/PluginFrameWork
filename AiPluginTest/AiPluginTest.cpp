#include "AiPluginTest.h"

AiPluginTest::~AiPluginTest()
{
    Stop();
    if (work_thr_.joinable())
    {
        work_thr_.join();
    }
    processor_->ClearFaceInfo();
    ::printf("AiPluginTest::~AiPluginTest---------------\n");
}

bool AiPluginTest::Init(const std::string &params)
{
    ::printf("----------AiPluginTest::Init with param: %s-----\n", params.data());
    return true;
}

void AiPluginTest::Start()
{
    running_ = true;
    work_thr_ = std::thread([&] {
        ::printf("--------AiPluginTest::Start-------\n");
        while (running_)
        {

            cv::Mat org = cv::Mat::ones(3, 3, CV_8UC3);
            std::shared_ptr<FaceRecogInfo> info = std::make_shared<FaceRecogInfo>();
            info->face_id = "test_id ";
            info->face_id += std::to_string(id_idex_);
            info->face_rect = cv::Rect(200, 100, 50, 50);
            info->org = org;
            info->trace_id = id_idex_;
            id_idex_++;

            processor_->AddFaceInfo(info);
            // std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });
}

void AiPluginTest::Stop()
{
    running_ = false;
}

PLUGIN_API_EXPORT std::shared_ptr<PluginInterface> CreatePlugin(AiProcessor *pro)
{
    return std::make_shared<AiPluginTest>(pro);
}