#pragma once

#include <glm/glm.hpp>

#include <iostream>
#include <algorithm>

#include "../include/GL/Time.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 22

#define I_BLOCK 1
#define O_BLOCK 2
#define L_BLOCK 3
#define J_BLOCK 4
#define S_BLOCK 5
#define Z_BLOCK 6
#define T_BLOCK 7
#define FLOOR_BLOCK 8

// :skull_emoji:
inline glm::ivec2 BLOCK_SPAWN_STATES[7][4] ={{{0,0},{1,0},{2,0},{3,0}},
										{{0,0},{1,0},{0,1},{1,1}},
										{{0,0},{1,0},{2,0},{0,1}},
										{{0,0},{1,0},{2,0},{2,1}},
										{{0,1},{1,1},{1,0},{2,0}},
										{{0,0},{1,0},{1,1},{2,1}},
										{{0,0},{1,0},{2,0},{1,1}}};
inline glm::ivec2 BLOCK_ROTATE_STATES[7][4][4] ={{{{0,2},{1,2},{2,2},{3,2}},
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
//struct I_BLOCK_ROT {
//	//inline static glm::ivec2 rot_states[4][4] ={{{0,2},{1,2},{2,2},{3,2}},
	//												{{2,0},{2,1},{2,2},{2,3}},
	//												{{0,2},{1,2},{2,2},{3,2}},
	//												{{2,0},{2,1},{2,2},{2,3}}};
//	inline static glm::ivec2 spawn_state[4] ={{0,0},{1,0},{2,0},{3,0}};
//};
//struct O_BLOCK_ROT {
//	//inline static glm::ivec2 rot_states[4][4] ={{{0,0},{1,0},{0,1},{1,1}},
	//											{{0,0},{1,0},{0,1},{1,1}},
	//											{{0,0},{1,0},{0,1},{1,1}},
	//											{{0,0},{1,0},{0,1},{1,1}}};
//	inline static glm::ivec2 spawn_state[4] ={{0,0},{1,0},{0,1},{1,1}};
//};
//struct L_BLOCK_ROT {
//	//inline static glm::ivec2 rot_states[4][4] ={{{0,1},{1,1},{2,1},{0,2}},
	//											{{0,0},{1,0},{1,1},{1,2}},
	//											{{0,1},{1,1},{2,1},{2,0}},
	//											{{1,0},{1,1},{1,2},{2,2}}};
//	inline static glm::ivec2 spawn_state[4] ={{0,0},{1,0},{2,0},{0,1}};
//};
//struct J_BLOCK_ROT {
//	//inline static glm::ivec2 rot_states[4][4] ={{{0,1},{1,1},{2,1},{2,2}},
	//											{{0,2},{1,0},{1,1},{1,2}},
	//											{{0,0},{0,1},{1,1},{2,1}},
	//											{{1,0},{1,1},{1,2},{2,0}}};
//	inline static glm::ivec2 spawn_state[4] ={{0,0},{1,0},{2,0},{2,1}};
//};
//struct S_BLOCK_ROT {
//	//inline static glm::ivec2 rot_states[4][4] ={{{0,2},{1,2},{1,1},{2,1}},
	//											{{1,0},{1,1},{2,1},{2,2}},
	//											{{0,2},{1,2},{1,1},{2,1}},
	//											{{1,0},{1,1},{2,1},{2,2}}};
//	inline static glm::ivec2 spawn_state[4] ={{0,1},{1,1},{1,0},{2,0}};
//};
//struct Z_BLOCK_ROT {
//	//inline static glm::ivec2 rot_states[4][4] ={{{0,1},{1,1},{1,2},{2,2}},
	//											{{2,0},{2,1},{1,1},{1,2}},
	//											{{0,1},{1,1},{1,2},{2,2}},
	//											{{2,0},{2,1},{1,1},{1,2}}};
//	inline static glm::ivec2 spawn_state[4] ={{0,0},{1,0},{1,1},{2,1}};
//};
//struct T_BLOCK_ROT {
//	//inline static glm::ivec2 rot_states[4][4] ={{{0,1},{1,1},{2,1},{1,2}},
	//											{{1,0},{1,1},{1,2},{0,1}},
	//											{{1,0},{1,1},{1,2},{2,1}},
	//											{{1,0},{1,1},{1,2},{0,1}}};
//	inline static glm::ivec2 spawn_state[4] ={{0,0},{1,0},{2,0},{1,1}};
//};


class Tetris {
private:
	int last_spawned_piece = 0;
	int rot_state = 0;
	glm::ivec2 curr_block_move_delta = {0,0};
	int field[FIELD_WIDTH][FIELD_HEIGHT+1];
	glm::ivec2* curr_block_pos; //0 0 is top left
	std::time_t tick_timeout = 100;
	void spawnPiece() {
		curr_block_move_delta ={0,0};
		int piece = 0;
		while(last_spawned_piece == piece) {
			piece = 1+rand()%7;
		}
		curr_block_pos = BLOCK_SPAWN_STATES[piece];
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
			field[curr_block_pos[i].x][curr_block_pos[i].y] = piece;
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
	void rotateBlock(const bool left) {
		for(int i=0;i<4;i++) {
			field[curr_block_pos[i].x][curr_block_pos[i].y] = 0;
		}
		glm::ivec2 last_block_pos[4];
		memcpy(last_block_pos,curr_block_pos,4);
		if(left) {
			rot_state--;
			if(rot_state<0){ rot_state+=4; }
			curr_block_pos = BLOCK_ROTATE_STATES[last_spawned_piece][rot_state];
			//check intersection, if are - revert
		}else {
			rot_state++;
			if(rot_state>=4){ rot_state-=4; }
		}
	}
	void speedUpBlock() {

	}
	void speedUpGame() {

	}
public:
	Tetris() {
		memset(field,0,FIELD_WIDTH*FIELD_HEIGHT);
		for(int i=0;i<FIELD_WIDTH;i++) {
			field[i][FIELD_HEIGHT]=FLOOR_BLOCK;
		}
	}
	void Tick() {
		if(field[curr_block_pos[0].x][curr_block_pos[0].y+1]>0 ||
			field[curr_block_pos[1].x][curr_block_pos[1].y+1]>0 ||
			field[curr_block_pos[2].x][curr_block_pos[2].y+1]>0 ||
			field[curr_block_pos[3].x][curr_block_pos[3].y+1]>0) 
		{ //solid under block
			int lowest = lowestCurrentBlock();
			int highest = highestCurrentBlock();

			for(int i=highest;i<highest-lowest;i++) {
				bool row_complete = true;
				for(int j=0;j<FIELD_WIDTH;j++) {
					if(field[i][j] == 0) {
						row_complete = false;
						break;
					}
				}
				if(row_complete) {
					removeRow(i);
				}
			}
		}else {
			for(int i=0;i<4;i++) {
				field[curr_block_pos[i].x][curr_block_pos[i].y] = 0;
				curr_block_move_delta.y++;
				//curr_block_pos[i].y++;
				field[curr_block_pos[i].x+curr_block_move_delta.x][curr_block_pos[i].y+curr_block_move_delta.y] = last_spawned_piece;
			}
		}
	}
};