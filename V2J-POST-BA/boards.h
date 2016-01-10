/* 
Copyright 2016 Christian Liebl. 

Licenced under the GNU GPL Version 3.

Contact: christian.liebl@live.de

Pin Configuration
*/

#ifndef boards_h
#define boards_h

// existing Boards and the latest Prototype

#define BOARD_V2H 1 // BA-Thesis Prototype Board
#define BOARD_V3C 2 // Latest PCB Design       

#define MB(board) (PCB==BOARD_##board)

#endif
