#include "application.h"

static const int VIDEO_DEVICE_ID = 1;

int main(int argc, char** argv)
{
    blobdetective::Configuration configuration = blobdetective::Configuration();
    configuration.video_device_id = VIDEO_DEVICE_ID;
    blobdetective::Application app(configuration);
    return app.run();
}
