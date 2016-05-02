//
//  BufferToggle.hpp
//  MIPSSuperscalarSimulator
//
//  Created by Matt Zhu on 5/1/16.
//  Copyright © 2016 ZhuKim. All rights reserved.
//

#ifndef BufferToggle_hpp
#define BufferToggle_hpp

/* BufferToggle.hpp
 *    Toggle buffered input
 */

#include <termios.h>
#include <unistd.h>

class BufferToggle {
    
private:
    struct termios t;

public:
    
    /*
     * Disable buffered input
     */
    
    void off() {
        tcgetattr(STDIN_FILENO, &t); // retrieve the current terminal I/O structure
        t.c_lflag &= ~ICANON; // Manipulate the flag bits
        tcsetattr(STDIN_FILENO, TCSANOW, &t); // Set the new structure
    }
    
    /*
     * Enable buffered input
     */
    
    void on() {
        tcgetattr(STDIN_FILENO, &t);
        t.c_lflag |= ICANON;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
    }
};

#endif /* BufferToggle_hpp */
