#include "application.h"

static const int VIDEO_DEVICE_ID = 1;

int main(int argc, char** argv)
{
    blobdetective::Configuration configuration = blobdetective::Configuration();
    configuration.add_option_int("osc_send_port", 31340);
    configuration.add_option_string("osc_send_host", "127.0.0.1");
    configuration.add_option_string("identifier", "default");
    configuration.add_option_boolean("verbose", true);

    configuration.add_option_float("minThreshold", 0.0);
    configuration.add_option_float("maxThreshold", 50.0);
    configuration.add_option_float("minDistBetweenBlobs", 10.0);
    configuration.add_option_boolean("filterByArea", true);
    configuration.add_option_int("minArea", 1500);
    configuration.add_option_int("maxArea", 10000);
    configuration.add_option_boolean("filterByCircularity", false);
    configuration.add_option_boolean("filterByConvexity", false);

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
