namespace blobspy {

struct Configuration
{
    public:
        Configuration();
        int video_device_id;
};

class Application
{
    private:
        Configuration _configuration;
    public:
        Application(Configuration& configuration);
        int run();
};

} // end of namespace
