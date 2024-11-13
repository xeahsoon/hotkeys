#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>

// Key mapping table
std::map<std::string, WORD> keyMap = {
    // Modifier keys
    {"Ctrl", VK_CONTROL},
    {"LCtrl", VK_LCONTROL},
    {"RCtrl", VK_RCONTROL},
    {"Shift", VK_SHIFT},
    {"LShift", VK_LSHIFT},
    {"RShift", VK_RSHIFT},
    {"Alt", VK_MENU},
    {"LAlt", VK_LMENU},
    {"RAlt", VK_RMENU},
    {"Win", VK_LWIN},
    {"RWin", VK_RWIN},

    // Arrow keys
    {"Left", VK_LEFT},
    {"Right", VK_RIGHT},
    {"Up", VK_UP},
    {"Down", VK_DOWN},

    // Alphabet keys
    {"A", 'A'}, {"B", 'B'}, {"C", 'C'}, {"D", 'D'},
    {"E", 'E'}, {"F", 'F'}, {"G", 'G'}, {"H", 'H'},
    {"I", 'I'}, {"J", 'J'}, {"K", 'K'}, {"L", 'L'},
    {"M", 'M'}, {"N", 'N'}, {"O", 'O'}, {"P", 'P'},
    {"Q", 'Q'}, {"R", 'R'}, {"S", 'S'}, {"T", 'T'},
    {"U", 'U'}, {"V", 'V'}, {"W", 'W'}, {"X", 'X'},
    {"Y", 'Y'}, {"Z", 'Z'},

    // Number keys (main keyboard)
    {"0", '0'}, {"1", '1'}, {"2", '2'}, {"3", '3'},
    {"4", '4'}, {"5", '5'}, {"6", '6'}, {"7", '7'},
    {"8", '8'}, {"9", '9'},

    // Function keys
    {"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4},
    {"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8},
    {"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},
    
    // Numpad number keys
    {"Num0", VK_NUMPAD0}, {"Num1", VK_NUMPAD1}, {"Num2", VK_NUMPAD2}, {"Num3", VK_NUMPAD3},
    {"Num4", VK_NUMPAD4}, {"Num5", VK_NUMPAD5}, {"Num6", VK_NUMPAD6}, {"Num7", VK_NUMPAD7},
    {"Num8", VK_NUMPAD8}, {"Num9", VK_NUMPAD9},

    // Numpad operation keys
    {"NumLk", VK_NUMLOCK},
    {"Div", VK_DIVIDE},
    {"Mul", VK_MULTIPLY},
    {"Sub", VK_SUBTRACT},
    {"Add", VK_ADD},
    {"Dec", VK_DECIMAL},

    // Control keys
    {"Esc", VK_ESCAPE},
    {"Tab", VK_TAB},
    {"Caps", VK_CAPITAL},
    {"Space", VK_SPACE},
    {"Enter", VK_RETURN},
    {"Bksp", VK_BACK},

    // Editing keys
    {"Ins", VK_INSERT},
    {"Del", VK_DELETE},
    {"Home", VK_HOME},
    {"End", VK_END},
    {"PgUp", VK_PRIOR},
    {"PgDn", VK_NEXT},

    // Punctuation keys
    {"SColon", VK_OEM_1},        // ;
    {"Plus", VK_OEM_PLUS},       // +
    {"Comma", VK_OEM_COMMA},     // ,
    {"Minus", VK_OEM_MINUS},     // -
    {"Dot", VK_OEM_PERIOD},      // .
    {"Slash", VK_OEM_2},         // /
    {"Tilde", VK_OEM_3},         // ~
    {"LBrack", VK_OEM_4},        // [
    {"Bslash", VK_OEM_5},        // \
    {"RBrack", VK_OEM_6},        // ]
    {"Quote", VK_OEM_7},         // '

    // Multimedia keys
    {"MediaNext", VK_MEDIA_NEXT_TRACK},  // Next track
    {"MediaPrev", VK_MEDIA_PREV_TRACK},  // Previous track
    {"MediaStop", VK_MEDIA_STOP},        // Stop
    {"MediaPlayPause", VK_MEDIA_PLAY_PAUSE},  // Play/Pause
    {"VolumeMute", VK_VOLUME_MUTE},      // Mute
    {"VolumeUp", VK_VOLUME_UP},          // Increase volume
    {"VolumeDown", VK_VOLUME_DOWN},      // Decrease volume

    // Other special keys
    {"PrtSc", VK_SNAPSHOT},
    {"Scroll", VK_SCROLL},
    {"Pause", VK_PAUSE},
    {"Menu", VK_APPS},            // Application Menu (context menu)
};

// Helper function to press keys
void PressKey(const std::vector<WORD>& keys) {
    std::vector<INPUT> inputs;

    // Create a press event for each key
    for (WORD key : keys) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        inputs.push_back(input);
    }

    // Create a release event for each key
    for (WORD key : keys) {
        INPUT input = {};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = key;
        input.ki.dwFlags = KEYEVENTF_KEYUP;
        inputs.push_back(input);
    }

    // Send press and release events
    SendInput(inputs.size(), inputs.data(), sizeof(INPUT));
	
    // Ensure the system detects the key press
    Sleep(60);
}

// Parse and simulate key press events
void SimulateKeyPress(const std::string& keySequence) {
    std::stringstream ss(keySequence);
    std::string group;

    // Split multiple key combinations by comma
    while (std::getline(ss, group, ',')) {
        std::vector<WORD> keys;
        std::stringstream keyStream(group);
        std::string key;

        // Split each combination by plus sign
        while (std::getline(keyStream, key, '+')) {
            key = key.substr(key.find_first_not_of(' ')); // Remove leading whitespace
            if (keyMap.count(key)) {
                keys.push_back(keyMap[key]);
            } else {
                std::cerr << "Unknown key: " << key << std::endl;
            }
        }

        if (!keys.empty()) {
            PressKey(keys);
        }
    }
}

int main(int argc, char* argv[]) {
    // Check if command-line arguments are provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <key sequence>" << std::endl;
        return 1;
    }

    // Read the command-line argument as the key sequence
    std::string inputSequence = argv[1];
    SimulateKeyPress(inputSequence);
    return 0;
}