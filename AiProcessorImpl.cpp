#include "AiProcessorImpl.h"

void AiProcessorImpl::AddFaceInfo(std::shared_ptr<FaceRecogInfo> info)
{
    std::lock_guard<std::mutex> lock(face_info_mtx_);
    face_infos_.push_back(info);
    ::printf("std::shared_ptr<FaceRecogInfo> count %d\n", (int)face_infos_.size());
}

void AiProcessorImpl::ClearFaceInfo()
{
    std::lock_guard<std::mutex> lock(face_info_mtx_);
    face_infos_.clear();
}