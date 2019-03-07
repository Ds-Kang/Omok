#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <math.h>

typedef struct {
	int board[20][20];
	int score;
	int prep, preq;
}Board;

typedef struct {
	int block[20];
	int line[20];
}Linescore;

Board mainBoard;

void initboard();
void printboard(Board b);
int scorecal(Linescore score);
int rowscore(Board b, int p, int q);
int colscore(Board b, int p, int q);
int diagscore1(Board b, int p, int q);
int diagscore2(Board b, int p, int q);
int totalscore(Board b, Board preb, int p, int q);
int terminaltest(Board b);
Board maxval(Board b, int alpha, int beta, int depth);
Board minval(Board b, int alpha, int beta, int depth);
Board alphabeta(Board state);
void player();
void comp();


int main() {
	initboard();

	while (1) {
		player();
		printboard(mainBoard);
		comp();
		printboard(mainBoard);
	}

	return 0;
}

void initboard() {
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			mainBoard.board[i][j] = 0;
		}
	}
	mainBoard.score = 0;
}

void printboard(Board b) {
	//   system("cls");
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			switch (b.board[i][j])
			{
			case 1:
				printf("O");
				break;
			case 0:
				printf(" ");
				break;
			case -1:
				printf("X");
				break;
			default:
				printf("Unexpected board stone");
			}

		}
		printf("\n");
	}
}

int scorecal(Linescore score) {
	int sum = 0;
	for (int cnt = 0; score.line[cnt] != NULL; cnt++) {
		switch (score.block[cnt])
		{
		case 2:
			break;
		case 1:
			switch (score.line[cnt])
			{
			case 5:
				sum += 250;
				break;
			case 4:
				sum += 15;
				break;
			case 3:
				sum += 4;
				break;
			case 2:
				sum += 1;
				break;
			}
			break;
		case 0:
			switch (score.line[cnt])
			{
			case 5:
				sum += 250;
				break;
			case 4:
				sum += 50;
				break;
			case 3:
				sum += 15;
				break;
			case 2:
				sum += 4;
				break;
			}
			break;
		default:
			break;
		}
	}
	return sum;
}

int rowscore(Board b, int p, int q) {
	Linescore score = {NULL,NULL};
	int cnt = 0;
	int lscore;
	for (int i = 0; i < 19; i++) {
		if (b.board[i][q] != 0) {
			int block;
			int line = 1;
			int now = b.board[i][q];

			if (i == 0 || b.board[i - 1][q] != 0) block = 1;
			else block = 0;

			i++;
			while (line<5) {
				if (b.board[i][q] == 0 && b.board[i + 1][q] == 0) {
					break;
				}
				else if (now == -b.board[i][q]) {
					block++;
					break;
				}
				else {
					line++;
				}
				i++;
			}
			score.line[cnt] = line;
			score.block[cnt] = block;

			cnt++;
		}
	}
	lscore = scorecal(score);
	return lscore;
}

int colscore(Board b, int p, int q) {
	Linescore score = { NULL,NULL };
	int cnt = 0;
	int lscore;

	for (int i = 0; i < 19; i++) {
		if (b.board[p][i] != 0) {
			int block;
			int line = 1;
			int now = b.board[p][i];

			if (i == 0) block = 1;
			if (b.board[p][i - 1] != 0) block = 1;
			else block = 0;

			i++;
			while (line<5) {
				if (b.board[p][i] == 0 && b.board[p][i + 1] == 0) {
					break;
				}
				else if (now == -b.board[p][i]) {
					block++;
					break;
				}
				else {
					line++;
				}
				i++;
			}
			score.line[cnt] = line;
			score.block[cnt] = block;

			cnt++;
		}
	}
	lscore = scorecal(score);
	return lscore;
}

int diagscore1(Board b, int p, int q) {
	Linescore score = { NULL,NULL };
	int cnt = 0;
	int start, end, lscore;
	if (p + q < 19) {
		start = 0;
		end = p + q;
	}
	else {
		start = p + q - 18;
		end = 18;
	}

	for (int i = start; i <= end; i++) {
		if (b.board[i][p + q - i] != 0) {
			int block;
			int line = 1;
			int now = b.board[i][p + q - i];

			if (i == 0) block = 1;
			if (b.board[i - 1][p + q - i] != 0) block = 1;
			else block = 0;


			i++;
			while (line<5) {
				if (b.board[i][p + q - i] == 0 && b.board[i + 1][p + q - i] == 0) {
					break;
				}
				else if (now == -b.board[i][p + q - i]) {
					block++;
					break;
				}
				else {
					line++;
				}
				i++;
			}
			score.line[cnt] = line;
			score.block[cnt] = block;
			cnt++;
		}
	}
	lscore = scorecal(score);
	return lscore;
}

int diagscore2(Board b, int p, int q) {
	Linescore score = {NULL,NULL};
	int cnt = 0;
	int start, end, lscore;
	if (p < q) {
		start = 0;
		end = p - q + 18;
	}
	else {
		start = p - q;
		end = 18;
	}

	for (int i = start; i <= end; i++) {
		if (b.board[i][q - p + i] != 0) {
			int block;
			int line = 1;
			int now = b.board[i][q - p + i];

			if (i == 0) block = 1;
			if (b.board[i - 1][q - p + i] != 0) block = 1;
			else block = 0;

			i++;
			while (line<5) {
				if (b.board[i][q - p + i] == 0 && b.board[i + 1][q - p + i] == 0) {
					break;
				}
				else if (now == -b.board[i][q - p + i]) {
					block++;
					break;
				}
				else {
					line++;
				}
				i++;
			}
			score.line[cnt] = line;
			score.block[cnt] = block;

			cnt++;
		}
	}
	lscore = scorecal(score);
	return lscore;
}

int totalscore(Board b, Board preb, int p, int q) {
	b.score += rowscore(b, p, q) + colscore(b, p, q) + diagscore1(b, p, q) + diagscore2(b, p, q);
	b.score -= rowscore(preb, p, q) + colscore(preb, p, q) + diagscore1(preb, p, q) + diagscore2(preb, p, q);

	return b.score;
}

int terminaltest(Board b) {
	for (int i = 0; i < 19; i++) {
		for (int j = 0; j < 19; j++) {
			if (b.board[i][j] == 0) return 0;
		}
	}
	return 1;
}

int taken_check(Board b,int p,int q) 
{
	if (b.board[p][q] != 0) return 1;
	return 0;
}

Board maxval(Board b, int alpha, int beta,int depth) {
	Board v, mv;
	depth++;
	if (depth == 3) return b;
	if (terminaltest(b)) return b;
	v.score = -1000000;
	for (int p = b.prep-3; p < b.prep + 3; p++) {
		for (int q = b.preq - 3; q < b.preq + 3; q++) {
			Board preBoard = mainBoard;
			if (p < 0 || p>18 || q < 0 || q>18) {
				continue;
			}
			else if (taken_check(b, p, q)) continue;
			b.prep = p;
			b.preq = q;
			b.board[p][q] = -1;
			printboard(b);
			b.score += totalscore(b, preBoard, p, q);
			mv = minval(b, alpha, beta, depth);
			if (v.score < mv.score) v = mv;
			if (v.score >= beta) return v;
			alpha = max(alpha, v.score);
		}
	}
	return v;
}

Board minval(Board b, int alpha, int beta, int depth) {
	Board v, mv;
	depth++;
	if (depth == 3) return b;
	if (terminaltest(b)) return b;
	v.score = 1000000;
	for (int p = b.prep - 3; p < b.prep + 3; p++) {
		for (int q = b.preq - 3; q < b.preq + 3; q++) {
			Board preBoard = mainBoard;
			if (p < 0 || p>18 || q < 0 || q>18) continue;
			else if (taken_check(b, p, q)) continue;
			b.prep = p;
			b.preq = q;
			b.board[p][q] = 1;
			b.score -= totalscore(b, preBoard, p, q);
			mv = maxval(b, alpha, beta, depth);
			if (v.score > mv.score) v = mv;
			if (v.score <= alpha) return v;
			beta = min(beta, v.score);
		}
	}
	return v;
}

Board alphabeta(Board state) {
	Board v = maxval(state, -10000000000, 10000000000,0);
	return v;
}

void player() {
	int p, q;
	Board preBoard = mainBoard;
	printf("Enter row and column number: ");
	scanf_s("%d %d", &p, &q);
	mainBoard.prep = p;
	mainBoard.preq = q;
	mainBoard.board[p][q] = 1;
	mainBoard.score += totalscore(mainBoard, preBoard, p, q);
}

void comp() {
	Board abBoard = alphabeta(mainBoard);
	mainBoard.prep = abBoard.prep;
	mainBoard.preq = abBoard.preq;
	printf("%d %d\n", abBoard.prep, abBoard.preq);
	mainBoard.board[abBoard.prep][abBoard.preq] = -1;
}
