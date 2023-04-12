#pragma once

//SDL
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

//STL
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <any>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <fstream>
#include <stdexcept>
#include <stack>
#include <math.h>

//Macros
#define WORLD_SCALE 32
#define MAX_KEYBOARD_KEYS 350
#define DELTA_TIME 1.0f / 144.0f

//Typedefs
typedef void(*ButtonEvent)(std::vector<std::any>);
