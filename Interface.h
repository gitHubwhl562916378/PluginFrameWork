#pragma once
#include <dlfcn.h>
#include <string>
#include <memory>
#include <mutex>
#include "opencv2/opencv.hpp"

struct FaceRecogInfo
{
    int trace_id;
    std::string face_id;
    cv::Rect face_rect;
    cv::Mat org;
};

class AiProcessor
{
public:
    virtual void AddFaceInfo(std::shared_ptr<FaceRecogInfo> info) = 0;
    virtual void ClearFaceInfo() = 0;
};

struct PluginInterface
{
    PluginInterface(AiProcessor *pro) : processor_(pro) {}
    virtual ~PluginInterface(){::printf("~PluginInterface\n");};
    virtual bool Init(const std::string &params) = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

protected:
    AiProcessor *processor_ = nullptr;
};

typedef std::shared_ptr<PluginInterface> (*CretePluginFunc)(AiProcessor *pro);
class PluginManager
{
public:
    PluginManager(AiProcessor *pro) : processor_(pro) {}
    ~PluginManager()
    {
        std::lock_guard<std::mutex> lock(handle_mtx_);
        for(auto iter = handles_map_.begin(); iter != handles_map_.end(); ++iter)
        {
            dlclose(iter->second.first);
            ::printf("plugin unload\n");
        }
    }

    std::shared_ptr<PluginInterface> MakePlugin(const std::string &name)
    {
        CretePluginFunc func;
        {
            std::lock_guard<std::mutex> lock(handle_mtx_);
            auto iter = handles_map_.find(name);
            if (iter == handles_map_.end())
            {
                ::printf("plugin not load\n");
                return nullptr;
            }
            func = iter->second.second;
        }
        return func(processor_);
    }

    //加载插件
    bool LoadPlugin(const std::string &name)
    {
        {
            std::lock_guard<std::mutex> lock(handle_mtx_);
            if (handles_map_.find(name) != handles_map_.end())
            {
                ::printf("plugin already loaded\n");
                return true;
            }
        }

        void *handle = dlopen(name.c_str(), RTLD_NOW | RTLD_DEEPBIND); //RTLD_NOW | RTLD_DEEPBIND
        if (!handle)
        {
            ::printf("!!!!!!!!!!dlopen %s fail!!!!!!!!!!!!!!!!!!\n", name.data());
            return false;
        }

        CretePluginFunc reg = (CretePluginFunc)dlsym(handle, "CreatePlugin");
        if (!reg)
        {
            ::printf("!!!!!!!!!!!plugin[%s] dlsym RegisterAiService  fail!!!!!!!!!!!!!!\n", name.data());
            dlclose(handle);
            return false;
        }

        {
            std::lock_guard<std::mutex> lock(handle_mtx_);
            handles_map_.insert(std::make_pair(name, std::make_pair(handle, reg)));
        }
        ::printf("plugin %s loaded\n", name.data());
        return true;
    }

    //卸载插件
    bool UnLoadPlugin(const std::string &name)
    {
        {
            std::lock_guard<std::mutex> lock(handle_mtx_);
            auto iter = handles_map_.find(name);
            if (iter == handles_map_.end())
            {
                ::printf("plugin not find\n");
                return true;
            }
            ::printf("plugin prepare close ptr: %d\n", (unsigned long long)iter->second.first);
            dlclose(iter->second.first);
            handles_map_.erase(iter);
            ::printf("plugin unload\n");
        }
    }

private:
    std::mutex handle_mtx_;
    std::map<std::string, std::pair<void *, CretePluginFunc>> handles_map_;
    AiProcessor *processor_;
};