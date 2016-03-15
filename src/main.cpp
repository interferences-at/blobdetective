#include "application.h"

static const int VIDEO_DEVICE_ID = 1;

int main(int argc, char** argv)
{
    blobspy::Configuration configuration = blobspy::Configuration();
    configuration.video_device_id = VIDEO_DEVICE_ID;
    blobspy::Application app(configuration);
    return app.run();
}
