#include "application.h"
#include "convert.h"
#include <algorithm>

static const char* PROGRAM_NAME = "blobdetective";
static const char* VERSION = "0.1.1";

static char* get_command_line_option_argument_1(char** begin, char** end,
        const std::string& option)
{
    char** iter = std::find(begin, end, option);
    if (iter != end && ++iter != end)
    {
        return (*iter);
    }
    return 0;
}

static char* get_command_line_option_argument_2(char** begin, char** end,
        const std::string& option)
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

static bool command_line_option_exists(char** begin, char** end,
        const std::string& option)
{
    return std::find(begin, end, option) != end;
}

static void print_help(const char* program_name)
{
    std::cout << "Usage: " << std::string(program_name) << " [options]"
            << std::endl;
    std::cout << " -h,--help Print help and exit" << std::endl;
    std::cout << " --version Print version and exit" << std::endl;
    std::cout << " -v,--verbose Set output to verbose" << std::endl;
    std::cout << " -l List options and exit" << std::endl;
    std::cout << " -d [value] Set the video device identifier. /dev/video0"
            << std::endl;
    std::cout << " -o [name] [value] Sets an option value" << std::endl;
}

int main(int argc, char** argv)
{
    blobdetective::Configuration configuration = blobdetective::Configuration();
    configuration.add_option_int("osc_send_port", 31340);
    configuration.add_option_int("osc_receive_port", 31444);
    configuration.add_option_string("osc_send_host", "127.0.0.1");
    configuration.add_option_int("identifier", 0);
    configuration.add_option_boolean("verbose", false);
    configuration.add_option_string("video_device_id", "0");
    configuration.add_option_int("video_fps", 60);
    configuration.add_option_int("video_width", 640);
    configuration.add_option_int("video_height", 480);

    // SimpleBlobDetector params:
    configuration.add_option_int("thresholdStep", 10);
    configuration.add_option_float("minThreshold", 0.0);
    //configuration.add_option_int("maxCentersDist", 10);
    configuration.add_option_float("maxThreshold", 50.0);
    configuration.add_option_float("minDistBetweenBlobs", 10.0);
    configuration.add_option_float("minCircularity", 0.2);
    configuration.add_option_boolean("filterByArea", true);
    configuration.add_option_int("minArea", 1500);
    configuration.add_option_int("maxArea", 10000);
    configuration.add_option_boolean("filterByCircularity", false);
    // configuration.add_option_int("defaultKeypointSize", 1);
    // configuration.add_option_boolean("computeRadius", true);
    configuration.add_option_boolean("filterByColor", true);
    // Use blobColor = 0 to extract dark blobs
    // and blobColor = 255 to extract light blobs.
    configuration.add_option_int("blobColor", 0);
    configuration.add_option_int("minRepeatability", 2);
    //configuration.add_option_boolean("isGrayscaleCentroid", false);
    //configuration.add_option_int("centroidROIMargin", 2);
    configuration.add_option_boolean("filterByInertia", true);
    configuration.add_option_float("minInertiaRatio", 0.1);
    configuration.add_option_boolean("filterByConvexity", false);
    configuration.add_option_float("minConvexity", 0.95);

    if (command_line_option_exists(argv, argv + argc, "-h") ||
        command_line_option_exists(argv, argv + argc, "--help"))
    {
        print_help(PROGRAM_NAME);
        return 0;
    }
    if (command_line_option_exists(argv, argv + argc, "--version"))
    {
        std::cout << PROGRAM_NAME << " " << VERSION << std::endl;
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

    const std::vector<std::string> names = configuration.list_options();
    std::vector<std::string>::const_iterator iter;
    for (iter = names.begin(); iter < names.end(); ++iter)
    {
        std::string name = (*iter);
        blobdetective::Option *option = configuration.get_option(name.c_str());

        if (command_line_option_exists(argv, argv + argc, name.c_str()))
        {
            char* value = get_command_line_option_argument_1(
                    argv, argv + argc, name.c_str());
            if (value == 0)
            {
                std::cerr << "Error: option should have a value" << std::endl;
                print_help("blobdetective");
                return 1;
            }
            if (option->is_string())
            {
                if (configuration.get_option("verbose")->get_boolean())
                {
                    std::cout << "SET VALUE " << name << "=" << value << std::endl;
                }
                option->set_string(value);
            }
            else if (option->is_int())
            {
                // TODO: catch exceptions
                option->set_int(blobdetective::from_string<int>(value));
            }
            else if (option->is_float())
            {
                // TODO: catch exceptions
                option->set_float(blobdetective::from_string<float>(value));
            }
            else if (option->is_boolean())
            {
                option->set_boolean(blobdetective::from_string<bool>(value));
            }
        }
        // else
        // {
        //     std::cerr << "Error: No such option: " << std::string(name)
        //             << std::endl;
        //     // print_help("blobdetective");
        //     return 1;
        // }
    }

    if (configuration.get_option("verbose")->get_boolean())
    {
        std::cout << configuration << std::endl;
    }

    blobdetective::Application app(configuration);
    return app.run();
}

