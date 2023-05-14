#pragma once

#include <glm/glm.hpp>

#include <iostream>

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
#define CEILING_BLOCK 9
#define LEFT_WALL_BLOCK 10
#define RIGHT_WALL_BLOCK 11

class Tetris {
private:
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
														{{0,1},{1,1},{2,1},{1,0}},
														{{1,0},{1,1},{1,2},{2,1}}}};
	int last_spawned_piece = EMPTY_BLOCK;
	int rot_state = 0;
	glm::ivec2 curr_block_move_delta = {0,0};
	int field[FIELD_WIDTH][FIELD_HEIGHT+1];
	glm::ivec2* curr_block_pos = new glm::ivec2[4]; //0 0 is top left
	double tick_timeout = 0.2f; //in seconds
	double last_tick_time = 0;
	bool game_over = false;
	void clearCurrBlock() {
		for(int i=0;i<4;i++) {
			field[curr_block_pos[i].x+curr_block_move_delta.x][curr_block_pos[i].y+curr_block_move_delta.y] = EMPTY_BLOCK;
		}
	}
	void fillCurrBlock() {
		for(int i=0;i<4;i++) {
			field[curr_block_pos[i].x+curr_block_move_delta.x][curr_block_pos[i].y+curr_block_move_delta.y] = last_spawned_piece;
		}
	}
	void spawnPiece() {
		curr_block_move_delta ={3,0};
		static int piece = EMPTY_BLOCK;
		rot_state = 0;
		while(last_spawned_piece == piece) {
			piece = 1+rand()%7;
		}
		piece=1;
		curr_block_pos = BLOCK_SPAWN_STATES[piece-1];
		last_spawned_piece = piece;

		if(checkBlockIntersects(curr_block_pos)!=EMPTY_BLOCK) {
			game_over = true;
		}

		fillCurrBlock();
	}
	int lowestCurrentBlock() const {
		int lowest = curr_block_pos[0].y+curr_block_move_delta.y;
		for(int i=1;i<4;i++) {
			if(curr_block_pos[i].y+curr_block_move_delta.y > lowest) {
				lowest = curr_block_pos[i].y+curr_block_move_delta.y;
			}
		}
		return lowest;
	}
	int highestCurrentBlock() const {
		int highest = curr_block_pos[0].y+curr_block_move_delta.y;
		for(int i=1;i<4;i++) {
			if(curr_block_pos[i].y+curr_block_move_delta.y < highest) {
				highest = curr_block_pos[i].y+curr_block_move_delta.y;
			}
		}
		return highest;
	}
	void removeRow(const int row) {
		for(int i=row;i>0;i--) {
			for(int j=0;j<FIELD_WIDTH;j++) {
				field[j][i] = field[j][i-1];
			}
		}
		for(int i=0;i<FIELD_WIDTH;i++) {
			field[i][0]=EMPTY_BLOCK;
		}
		//memcpy(field+(FIELD_WIDTH*row),field+(FIELD_WIDTH*row-1),FIELD_WIDTH*row); //this removes column
		//memset(field,EMPTY_BLOCK,FIELD_WIDTH*row);
	}
	int checkBlockIntersects(const glm::ivec2* block) const {
		int intersection = EMPTY_BLOCK;
		for(int i=0;i<4;i++) {
			if(block[i].y+curr_block_move_delta.y>=FIELD_HEIGHT) {
				return FLOOR_BLOCK;
			}
		}
		for(int i=0;i<4;i++) {
			if(block[i].y+curr_block_move_delta.y<0) {
				return CEILING_BLOCK;
			}
		}
		for(int i=0;i<4;i++) {
			int posx = block[i].x+curr_block_move_delta.x;
			int posy = block[i].y+curr_block_move_delta.y;
			if(posx<0) {
				return LEFT_WALL_BLOCK;
			}
			if(posx>=FIELD_WIDTH) {
				return RIGHT_WALL_BLOCK;
			}
			if(field[posx][posy]!=EMPTY_BLOCK) {
				intersection = field[posx][posy];
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
	void speedUpBlock() {

	}
	void speedUpBlockSpawn() {

	}
	void tick() {
		if(checkBlockBottomIntersection(curr_block_pos)) {
			int highest = highestCurrentBlock(); //0 is the highest
			int lowest = lowestCurrentBlock();
			int removed_rows_count = 0;
			for(int i=0;i<lowest-highest+1;i++) {
				bool row_complete = true;
				for(int j=0;j<FIELD_WIDTH;j++) {
					if(field[j][lowest-i+removed_rows_count] == EMPTY_BLOCK) {
						row_complete = false;
						break;
					}
				}
				if(row_complete) {
					removeRow(lowest-i+removed_rows_count);
					removed_rows_count++;
				}
			}
			dc=lowest-highest+1;

			spawnPiece();
		}else{
			clearCurrBlock();
			curr_block_move_delta.y++;
			fillCurrBlock();
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
	void MoveCurrBlock(const bool left) {
		clearCurrBlock();
		int last_block_move_deltax = curr_block_move_delta.x;
		if(left) {
			curr_block_move_delta.x--;
		}else {
			curr_block_move_delta.x++;
		}
		if(checkBlockIntersects(curr_block_pos)!=EMPTY_BLOCK) {
			curr_block_move_delta.x=last_block_move_deltax;
		}
		fillCurrBlock();
	}
	void RotateCurrBlock(const bool left) {
		clearCurrBlock();
		int last_rot_state = rot_state;
		if(left) {
			rot_state--;
			if(rot_state<0){ rot_state+=4; }
		}else {
			rot_state++;
			if(rot_state>=4){ rot_state-=4; }
		}

		glm::ivec2* new_block_poses = BLOCK_ROTATE_STATES[last_spawned_piece-1][rot_state];
		glm::ivec2 last_block_move_delta = curr_block_move_delta;

		while(true){
			int intersection = checkBlockIntersects(new_block_poses);
			switch(intersection) {
			case EMPTY_BLOCK: //no intersection
				curr_block_pos = new_block_poses;
				fillCurrBlock();
				return;
			case CEILING_BLOCK:
				curr_block_move_delta.y++;
				break;
			case LEFT_WALL_BLOCK:
				curr_block_move_delta.x++;
				break;
			case RIGHT_WALL_BLOCK:
				curr_block_move_delta.x--;
				break;
			default: //intersection with floor or with blocks, don't rotate
				curr_block_move_delta = last_block_move_delta;
				rot_state = last_rot_state;
				fillCurrBlock();
				return;
			}
		}
	}
	void Update() {
		if(game_over) {
			return;
		}
		if(Time::time-last_tick_time>=tick_timeout){
			tick();
			last_tick_time = Time::time;
		}
		for(int i=0;i<FIELD_HEIGHT+1;i++) {
			for(int j=0;j<FIELD_WIDTH;j++) {
				if(field[j][i]==EMPTY_BLOCK) {
					std::cout<<' ';
				}
				else {
					std::cout<<'#';
				}
			}
			std::cout<<std::endl;
		}
		std::system("cls");
	}
};