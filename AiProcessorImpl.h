#pragma once
#include "Interface.h"

class AiProcessorImpl : public AiProcessor
{
public:
    void AddFaceInfo(std::shared_ptr<FaceRecogInfo> info) override;
    void ClearFaceInfo() override;

private:
    std::mutex face_info_mtx_;
    std::vector<std::shared_ptr<FaceRecogInfo>> face_infos_;
};