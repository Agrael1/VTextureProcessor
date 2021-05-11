/**
 * @file EntryMain.cpp
 * @author Ilya Doroshenko (xdoros01), David Černý (xcerny74)
 * @brief Program entrypoint
 */
#include <UI/App.h>

/**
 * @brief Entrypoint to the application
 *
 * @param argc argument count
 * @param argv argument list
 * @return int return code of the application
 */
int main(int argc, char *argv[])
{
	return App{ argc, argv }.Start();
}

/**
 * @mainpage ICP project documentation
 * @tableofcontents
 *
 * @section about About
*/
