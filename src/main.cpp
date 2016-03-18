#include "application.h"
#include <algorithm>
#include <sstream>

template<typename T>
T from_string(const std::string& str)
{
    std::istringstream ss(str);
    T ret;
    ss >> ret;
    return ret;
}

static char* get_command_line_option_argument_1(char** begin, char** end, const std::string& option)
{
    char** iter = std::find(begin, end, option);
    if (iter != end && ++iter != end)
    {
        return (*iter);
    }
    return 0;
}

static char* get_command_line_option_argument_2(char** begin, char** end, const std::string& option)
{
    char** iter = std::find(begin, end, option);
    if (iter != end && ++iter != end)
    {
        if (iter != end && ++iter != end)
        {
            return (*iter);
        }
        else
        {
            return 0;
        }
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
    std::cout << " -v,--verbose Set output to verbose" << std::endl;
    std::cout << " -l List options and exit" << std::endl;
    std::cout << " -d [value] Set the video device identifier. /dev/video0" << std::endl;
    std::cout << " -o [name] [value] Sets an option value" << std::endl;
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
    if (command_line_option_exists(argv, argv + argc, "-d"))
    {
        char* value = get_command_line_option_argument_1(argv, argv + argc, "-d");
        configuration.get_option("video_device_id")->set_string(value);
    }
    if (command_line_option_exists(argv, argv + argc, "-l"))
    {
        std::cout << configuration << std::endl;
        return 0;
    }
    if (command_line_option_exists(argv, argv + argc, "-o"))
    {
        char* name = get_command_line_option_argument_1(argv, argv + argc, "-o");
        char* value = get_command_line_option_argument_2(argv, argv + argc, "-o");
        if (value == 0)
        {
            std::cerr << "Error: option should have a value" << std::endl;
            print_help("blobdetective");
            return 1;
        }
        if (configuration.has_option(name))
        {
            blobdetective::Option* option = configuration.get_option(name);
            if (option->is_string())
            {
                option->set_string(value);
            }
            else if (option->is_int())
            {
                // TODO: catch exceptions
                option->set_int(from_string<int>(value));
            }
            else if (option->is_float())
            {
                // TODO: catch exceptions
                option->set_float(from_string<float>(value));
            }
            else if (option->is_boolean())
            {
                bool boolean_value = false;
                if (value == "true" || value == "yes" || value == "1")
                {
                    boolean_value = true;
                }
                option->set_boolean(boolean_value);
            }
        }
        else
        {
            std::cerr << "Error: No such option: " << std::string(name) << std::endl;
            // print_help("blobdetective");
            return 1;
        }
    }

    if (configuration.get_option("verbose")->get_boolean())
    {
        std::cout << configuration << std::endl;
    }

    blobdetective::Application app(configuration);
    return app.run();
}
