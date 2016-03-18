#include "application.h"
#include <algorithm>

static const int VIDEO_DEVICE_ID = 1;

static std::string get_command_line_option(char** begin, char** end, const std::string & option)
{
    char** iter = std::find(begin, end, option);
    if (iter != end && ++iter != end)
    {
        return std::string(*iter);
    }
    return 0;
}

static bool command_line_option_exists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

static void print_help(const char* program_name)
{
    std::cout << "Usage: " << std::string(program_name) << " [options]" << std::endl;
    std::cout << " -h,--help Print help and exit" << std::endl;
    std::cout << " -v,--verbose Sets output to verbose" << std::endl;
    std::cout << " --video-device-id [value] Sets the video device identifier" << std::endl;
    std::cout << " -o,--option [name] [value] Sets an option value (TODO)" << std::endl;
}

int main(int argc, char** argv)
{
    blobdetective::Configuration configuration = blobdetective::Configuration();
    configuration.add_option_int("osc_send_port", 31340);
    configuration.add_option_string("osc_send_host", "127.0.0.1");
    configuration.add_option_string("identifier", "default");
    configuration.add_option_boolean("verbose", false);
    configuration.add_option_string("video_device_id", "/dev/video0");
    configuration.add_option_int("video_width", 640);
    configuration.add_option_int("video_height", 480);

    configuration.add_option_float("minThreshold", 0.0);
    configuration.add_option_float("maxThreshold", 50.0);
    configuration.add_option_float("minDistBetweenBlobs", 10.0);
    configuration.add_option_boolean("filterByArea", true);
    configuration.add_option_int("minArea", 1500);
    configuration.add_option_int("maxArea", 10000);
    configuration.add_option_boolean("filterByCircularity", false);
    configuration.add_option_boolean("filterByConvexity", false);

    if (command_line_option_exists(argv, argv + argc, "-h") ||
        command_line_option_exists(argv, argv + argc, "--help"))
    {
        print_help("blobdetective");
        return 0;
    }
    if (command_line_option_exists(argv, argv + argc, "-v") ||
        command_line_option_exists(argv, argv + argc, "--verbose"))
    {
        configuration.get_option("verbose")->set_boolean(true);
    }
    if (command_line_option_exists(argv, argv + argc, "--video-device-id"))
    {
        std::string value = get_command_line_option(argv, argv + argc, "--video-device-id");
        configuration.get_option("video_device_id")->set_string(value.c_str());
    }

    //configuration.add_option_float("float_test", 3.14159f);
    //blobdetective::Option* float_test_option = configuration.get_option("float_test");
    //float_test_option->set_float(123.123);
    if (configuration.get_option("verbose")->get_boolean())
    {
        std::cout << configuration << std::endl;
    }

    configuration.video_device_id = VIDEO_DEVICE_ID;

    blobdetective::Application app(configuration);
    return app.run();
}
