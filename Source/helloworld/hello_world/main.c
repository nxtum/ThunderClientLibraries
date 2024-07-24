/*
* If not stated otherwise in this file or this component's LICENSE file the
* following copyright and licenses apply:
*
* Copyright 2020 Metrological
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/


#include <ctype.h>
#include "helloworld.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#ifdef __GNUC__
#define VARIABLE_IS_NOT_USED __attribute__((unused))
#elif defined(_MSC_VER)
#define VARIABLE_IS_NOT_USED
#else
#define VARIABLE_IS_NOT_USED
#endif

#define Trace(fmt, ...)                                 \
    do {                                                \
        fprintf(stdout, "<< " fmt "\n", ##__VA_ARGS__); \
        fflush(stdout);                                 \
    } while (0)

#define BUFFER_LENGTH 15

void show_menu()
{
    printf("Enter\n"
           "\tS : Register for an event.\n"
           "\tU : Unregister from an event.\n"
           "\tL : Print Message\n"
           "\tQ : Quit\n");
}


int main(){

    show_menu();

    int character;

    do {
        character = toupper(getc(stdin));

        switch (character) {
        case 'S': {
            break;
        }
        case 'U': {
            break;
        }
        case 'L': {

            const char* buf = "hello matey";
            if(helloworld_print_stuff(buf) == 0)
            {
                Trace("Success");
            };
            break;
        }
        
        case '?': {
            show_menu();
            break;
        }

        default:
            break;
        }
    } while (character != 'Q');

    helloworld_dispose();

    return 0;
}
