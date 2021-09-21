#include <CppUTest/CommandLineTestRunner.h>
#include <stdio.h>

// main test programm
int main(int ac, char **av)
{
    int i;
    printf("Starting the Testrunner\nwith %d arguments:\n", ac);
    for (i = 0; i < ac; i++) {
        printf("%s\n", av[i]);
    }
    printf("---------------------------------------------\n \n \n");

    int result = CommandLineTestRunner::RunAllTests(ac, av);

    printf("---------------------------------------------\n");

    return result;
}

// imported tests!
IMPORT_TEST_GROUP(action_handler);
IMPORT_TEST_GROUP(event_detector);
IMPORT_TEST_GROUP(FSM_system);
