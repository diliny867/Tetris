#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include <algorithm>

#include "../include/GL/Time.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 22

#define EMPTY_BLOCK 0
#define I_BLOCK 1
#define O_BLOCK 2
#define L_BLOCK 3
#define J_BLOCK 4
#define S_BLOCK 5
#define Z_BLOCK 6
#define T_BLOCK 7
#define FLOOR_BLOCK 8
#define LEFT_WALL_INTERSECTION 9
#define RIGHT_WALL_INTERSECTION 10

class Tetris {
public:
	// :skull_emoji:
	inline static glm::ivec2 BLOCK_SPAWN_STATES[7][4] ={{{0,0},{1,0},{2,0},{3,0}},
										{{0,0},{1,0},{0,1},{1,1}},
										{{0,0},{1,0},{2,0},{0,1}},
										{{0,0},{1,0},{2,0},{2,1}},
										{{0,1},{1,1},{1,0},{2,0}},
										{{0,0},{1,0},{1,1},{2,1}},
										{{0,0},{1,0},{2,0},{1,1}}};
	inline static glm::ivec2 BLOCK_ROTATE_STATES[7][4][4] ={{{{0,2},{1,2},{2,2},{3,2}},
														{{2,0},{2,1},{2,2},{2,3}},
														{{0,2},{1,2},{2,2},{3,2}},
														{{2,0},{2,1},{2,2},{2,3}}},
												{{{0,0},{1,0},{0,1},{1,1}},
														{{0,0},{1,0},{0,1},{1,1}},
														{{0,0},{1,0},{0,1},{1,1}},
														{{0,0},{1,0},{0,1},{1,1}}},
												{{{0,1},{1,1},{2,1},{0,2}},
														{{0,0},{1,0},{1,1},{1,2}},
														{{0,1},{1,1},{2,1},{2,0}},
														{{1,0},{1,1},{1,2},{2,2}}},
												{{{0,1},{1,1},{2,1},{2,2}},
														{{0,2},{1,0},{1,1},{1,2}},
														{{0,0},{0,1},{1,1},{2,1}},
														{{1,0},{1,1},{1,2},{2,0}}},
												{{{0,2},{1,2},{1,1},{2,1}},
														{{1,0},{1,1},{2,1},{2,2}},
														{{0,2},{1,2},{1,1},{2,1}},
														{{1,0},{1,1},{2,1},{2,2}}},
												{{{0,1},{1,1},{1,2},{2,2}},
														{{2,0},{2,1},{1,1},{1,2}},
														{{0,1},{1,1},{1,2},{2,2}},
														{{2,0},{2,1},{1,1},{1,2}}},
												{{{0,1},{1,1},{2,1},{1,2}},
														{{1,0},{1,1},{1,2},{0,1}},
														{{1,0},{1,1},{1,2},{2,1}},
														{{1,0},{1,1},{1,2},{0,1}}}};
	int last_spawned_piece = EMPTY_BLOCK;
	int rot_state = 0;
	glm::ivec2 curr_block_move_delta = {0,0};
	int field[FIELD_WIDTH][FIELD_HEIGHT+1];
	glm::ivec2* curr_block_pos = new glm::ivec2[4]; //0 0 is top left
	double tick_timeout = 0.1f; //in seconds
	double lastTickTime = 0;
	void spawnPiece() {
		curr_block_move_delta ={4,0};
		static int piece = EMPTY_BLOCK;
		while(last_spawned_piece == piece) {
			piece = 1+rand()%7;
		}
		curr_block_pos = BLOCK_SPAWN_STATES[piece-1];

		//switch(piece) {
		//case I_BLOCK:
		//	curr_block_pos = I_BLOCK_ROT::spawn_state;
		//	//curr_block_pos[0] ={3,0};
		//	//curr_block_pos[1] ={4,0};
		//	//curr_block_pos[2] ={5,0};
		//	//curr_block_pos[3] ={6,0};
		//	break;
		//case O_BLOCK:
		//	curr_block_pos = O_BLOCK_ROT::spawn_state;
		//	//curr_block_pos[0] ={4,0};
		//	//curr_block_pos[1] ={5,0};
		//	//curr_block_pos[2] ={4,1};
		//	//curr_block_pos[3] ={5,1};
		//	break;
		//case L_BLOCK:
		//	curr_block_pos = L_BLOCK_ROT::spawn_state;
		//	//curr_block_pos[0] ={4,0};
		//	//curr_block_pos[1] ={5,0};
		//	//curr_block_pos[2] ={6,0};
		//	//curr_block_pos[3] ={4,1};
		//	break;
		//case J_BLOCK:
		//	curr_block_pos = J_BLOCK_ROT::spawn_state;
		//	//curr_block_pos[0] ={4,0};
		//	//curr_block_pos[1] ={5,0};
		//	//curr_block_pos[2] ={6,0};
		//	//curr_block_pos[3] ={6,1};
		//	break;
		//case S_BLOCK:
		//	curr_block_pos = S_BLOCK_ROT::spawn_state;
		//	//curr_block_pos[0] ={4,1};
		//	//curr_block_pos[1] ={5,1};
		//	//curr_block_pos[2] ={5,0};
		//	//curr_block_pos[3] ={6,0};
		//	break;
		//case Z_BLOCK:
		//	curr_block_pos = Z_BLOCK_ROT::spawn_state;
		//	//curr_block_pos[0] ={4,0};
		//	//curr_block_pos[1] ={5,0};
		//	//curr_block_pos[2] ={5,1};
		//	//curr_block_pos[3] ={6,1};
		//	break;
		//case T_BLOCK:
		//	curr_block_pos = T_BLOCK_ROT::spawn_state;
		//	//curr_block_pos[0] ={4,0};
		//	//curr_block_pos[1] ={5,0};
		//	//curr_block_pos[2] ={6,0};
		//	//curr_block_pos[3] ={5,1};
		//	break;
		//}
		for(int i=0;i<4;i++) {
			field[curr_block_pos[i].x+curr_block_move_delta.x][curr_block_pos[i].y+curr_block_move_delta.y] = piece;
		}
		last_spawned_piece = piece;
	}
	int lowestCurrentBlock() const {
		int lowest = curr_block_pos[0].y;
		int index = 0;
		for(int i=1;i<4;i++) {
			if(curr_block_pos[i].y > lowest) {
				lowest = curr_block_pos[i].y;
				index = i;
			}
		}
		return index;
	}
	int highestCurrentBlock() const {
		int lowest = curr_block_pos[0].y;
		int index = 0;
		for(int i=1;i<4;i++) {
			if(curr_block_pos[i].y < lowest) {
				lowest = curr_block_pos[i].y;
				index = i;
			}
		}
		return index;
	}
	void removeRow(const int row) {
		memcpy(field+(FIELD_WIDTH*row),field+(FIELD_WIDTH*row-1),FIELD_WIDTH*row);
		memset(field,0,FIELD_WIDTH*row);
	}
	int checkBlockIntersects(const glm::ivec2* block) const {
		int intersection = EMPTY_BLOCK;
		for(int i=0;i<4;i++) {
			if(block[i].y+curr_block_move_delta.y>=FIELD_WIDTH) {
				return FLOOR_BLOCK;
			}
		}
		for(int i=0;i<4;i++) {
			int posx = block[i].x+curr_block_move_delta.x;
			int posy = block[i].y+curr_block_move_delta.y;
			if(posx<0) {
				return LEFT_WALL_INTERSECTION;
			}
			if(posx>=FIELD_WIDTH) {
				return RIGHT_WALL_INTERSECTION;
			}
			if(field[posy][posy]!=EMPTY_BLOCK) {
				if(intersection!=last_spawned_piece){
					intersection = field[posx][posy];
				}
			}
		}
		return intersection;
	}
	bool checkBlockBottomIntersection(const glm::ivec2* block) const {
		for(int i=0;i<4;i++) {
			if(field[block[i].x+curr_block_move_delta.x][block[i].y+curr_block_move_delta.y+1]!=EMPTY_BLOCK) {
				bool self_intersect = false;
				for(int j=0;j<4;j++) {
					if(block[i].x+curr_block_move_delta.x == block[j].x+curr_block_move_delta.x && block[i].y+curr_block_move_delta.y+1 == block[j].y+curr_block_move_delta.y) {
						self_intersect = true;
					}
				}
				if(!self_intersect){
					return true;
				}
			}
		}
		return false;
	}
	void rotateBlock(const bool left) { //TODO: TOP WALL COLLISION
		glm::ivec2 last_block_pos[4];
		memcpy(last_block_pos,curr_block_pos,4);
		int last_rot_state = rot_state;
		if(left) {
			rot_state--;
			if(rot_state<0){ rot_state+=4; }
		}else {
			rot_state++;
			if(rot_state>=4){ rot_state-=4; }
		}

		glm::ivec2* new_block_poses = BLOCK_ROTATE_STATES[last_spawned_piece][rot_state];
		glm::ivec2 last_block_move_delta = curr_block_move_delta;

		while(true){
			int intersection = checkBlockIntersects(new_block_poses);
			switch(intersection) {
			case EMPTY_BLOCK: //no intersection
				memcpy(curr_block_pos,new_block_poses,4);
				for(int i=0;i<4;i++) {
					field[last_block_pos[i].x][last_block_pos[i].y] = EMPTY_BLOCK;
				}
				for(int i=0;i<4;i++) {
					field[curr_block_pos[i].x][curr_block_pos[i].y] = last_spawned_piece;
				}
				return;
			case LEFT_WALL_INTERSECTION:
				curr_block_move_delta.x++;
				break;
			case RIGHT_WALL_INTERSECTION:
				curr_block_move_delta.x--;
				break;
			default: //intersection with floor or with blocks, don't rotate
				curr_block_move_delta = last_block_move_delta;
				rot_state = last_rot_state;
				return;
			}
		}
	}
	void speedUpBlock() {

	}
	void speedUpBlockSpawn() {

	}
	void tick() {
		if(checkBlockBottomIntersection(curr_block_pos)) {
			int lowest = lowestCurrentBlock();
			int highest = highestCurrentBlock();

			for(int i=highest;i<highest-lowest;i++) {
				bool row_complete = true;
				for(int j=0;j<FIELD_WIDTH;j++) {
					if(field[i][j] == EMPTY_BLOCK) {
						row_complete = false;
						break;
					}
				}
				if(row_complete) {
					removeRow(i);
				}
			}
			spawnPiece();
		}else{
			for(int i=0;i<4;i++) {
				field[curr_block_pos[i].x+curr_block_move_delta.x][curr_block_pos[i].y+curr_block_move_delta.y] = EMPTY_BLOCK;
			}
			curr_block_move_delta.y++;
			for(int i=0;i<4;i++) {
				field[curr_block_pos[i].x+curr_block_move_delta.x][curr_block_pos[i].y+curr_block_move_delta.y] = last_spawned_piece;
			}
		}
	}
public:
	Tetris() {
		memset(field,EMPTY_BLOCK,FIELD_WIDTH*FIELD_HEIGHT);
		for(int i=0;i<FIELD_WIDTH;i++) {
			field[i][FIELD_HEIGHT]=FLOOR_BLOCK;
		}
		srand(time(0));
		spawnPiece();
	}
	void Update() {
		if(Time::time-lastTickTime>=tick_timeout){
			tick();
			lastTickTime = Time::time;
		}
	}
};