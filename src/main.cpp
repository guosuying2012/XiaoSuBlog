#include "ApplicationService.h"
#include <cppcms/service.h>
#include <cppcms/applications_pool.h>
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<ApplicationService>());
        srv.run();
        return 0;
    }
    catch (std::exception const &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
