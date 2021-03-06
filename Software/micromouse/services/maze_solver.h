/*
 * maze_solver.h
 *
 *  Created on: Mar 21, 2014
 *      Author: Christopher Franzwa
 */

#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H
#include "stdint.h"
typedef struct {
	uint8_t Flags; //Flags are: Visited, Up Wall, Right Wall, Down Wall, Left Wall. 0b000VURDL This is a measure of absolute space.
	uint8_t distance;
	uint8_t VisitedCount;
	int8_t x,y;
}cNodeInfo_t;

typedef struct {
	cNodeInfo_t NodeInfo;
	cNodeInfo_t* previous;
}cNode_t;

//Set the maze dimensions here
#define xSize 16
#define ySize 16

//Use to create the initial maze map. Call it only once. If you want to reset the map, call maze_clear.
void maze_solver_init(void);

//Use to ready the initial map for the floodfill algorithm.
void maze_init_ff(void);

//Use this to reset the maze map. The function will reset all relevant data about walls and paths.
void maze_clear(void);

//This can optionally be used to change the starting point of the mouse. Defaults at 0,0
void maze_set_start_point(uint8_t x, uint8_t y);

//This can be called to set the initial rotation. Use increments of 90 degrees Ex: 0,90,180, 270 Defaults at 0
void maze_set_start_rotation(uint16_t rotation);

//Call this each time you arrive at a new node. The input "Flags" is a uuint8_t_8 that has 5 relevant flags.
//The flags are 0b000VURDL
//V indicates whether a node has been visited. This is changed automatically regardless of your input.
//U is sensor Up
//R is sensor Right
//D is sensor Down
//L is sensor Left
//Input based on local space. The module handles conversions implicitly.
void maze_update_node(uint8_t Flags);

//Call this to get instructions for the next step the mouse should take.
//Function returns one of 5 values
//0 means move straight
//1 means turn 90 degrees to the right then move straight
//2 means turn around 180 degrees then move straight
//3 means turn 90 degrees to the left then move straight
//-1 means that the entire maze has been searched and the mouse has returned to the home position
int8_t maze_next_direction_dfs(void);

//Call this to get instructions for the next step the mouse should take.
//Function returns one of 5 values
//0 means move straight
//1 means turn 90 degrees to the right then move straight
//2 means turn around 180 degrees then move straight
//3 means turn 90 degrees to the left then move straight
//-1 means that the entire maze has been searched and the mouse has returned to the home position
int8_t maze_next_direction_ff(void);

//This function returns the current rotation of the micro-mouse.
uint16_t maze_get_rotation(void);

//This function returns the current x and y position of the micro-mouse.
void maze_get_position(uint8_t *X, uint8_t *Y);

//Returns a pouint8_ter to an uint8_teger array. Input the starting x and y (Likely 0,0 here) and the desired end pouint8_t. Ensure that the mouse has
//been reset to a rotation of zero or set initial rotation to the desired rotation.
int8_t * maze_dijkstras_algorithm(uint8_t startX, uint8_t startY,uint8_t endX, uint8_t endY);
#endif /* MAZE_SOLVER_H_ */
