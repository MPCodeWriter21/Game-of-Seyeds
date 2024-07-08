#include "gui/main.hpp"
#include "utils/utils.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int main(void)
{
    srand(time(0));
    init_curl();

    try
    {
        MainApp app(1280, 590, true, "Game of Seyeds");
        app.main_loop();
    }
    catch (std::runtime_error &error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Huh!\n";
    }

    clean_curl();
    return 0;
}
