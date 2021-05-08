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
