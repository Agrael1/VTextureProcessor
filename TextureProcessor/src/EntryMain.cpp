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
 * This program implements a node editor for blending textures using
 * input nodes (that generate constant shapes), operation nodes (that
 * are used to modify the textures) and output nodes that can be used to ePort
 * textures to files.
 * @image html screenshot.png width=750px
*/
