#include <opencv2/opencv.hpp>
#include <iterator>
#include <future>
#include <chrono>
#include <numeric>
#include <mutex>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "AiProcessorImpl.h"

int main(int argc, char *argv[])
{
    const std::string plug_name = "/home/whl/Documents/git/AiFrameWork/cpptest/build/libAiPluginTest.so";

    AiProcessor *processor = new AiProcessorImpl;
    PluginManager plugin_manager(processor);
    plugin_manager.LoadPlugin(plug_name);

    std::shared_ptr<PluginInterface> test_inter_face = plugin_manager.MakePlugin(plug_name);
    if (!test_inter_face)
    {
        return;
    }

    volatile bool quit = false;
    while (!quit)
    {
        char c = ::getchar();
        switch (c)
        {
        case 'u':
            test_inter_face = nullptr;
            plugin_manager.UnLoadPlugin(plug_name);
            break;

        case 'g':
            test_inter_face->Init("test args");
            test_inter_face->Start();
            break;

        case 'c':
            processor->ClearFaceInfo();
            break;

        case 's':
            test_inter_face->Stop();
            break;

        case 'r':
            if (plugin_manager.LoadPlugin(plug_name)) {
                test_inter_face = plugin_manager.MakePlugin(plug_name);
            }
            break;

        case 'n':
            test_inter_face = nullptr;
            plugin_manager.UnLoadPlugin(plug_name);
            break;

        case 'q':
            quit = true;
            break;
        }
    }

    std::cout << "-----------main quit------------" << std::endl;
    return 0;
}