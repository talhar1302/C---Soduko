//Name:Tal Harpaz  ID:208754622  Date: 03 August 2021
//----------------------------------

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NOT_FINISH 0
#define FINISH_SUCCESS 1
#define FINISH_FAILURE -1
#define FILLED 1
#define FAIL -1

typedef struct array
{
	short* arr;
	unsigned short size;
} Array;

typedef struct square
{
	short row;
	short col;
} Square;

typedef struct player
{
	char name[100];
	short sudokuBoard[][9];
	Array*** PossibleDigits;
} Player;

typedef struct squareLstNode
{
	Square* sq;
	struct Square* next;
} SquareLstNode;

typedef struct squareList
{
	SquareLstNode* head;
	SquareLstNode* tail;
} SquareList;


SquareList makeEmptySquareList()
{
	SquareList lst;
	lst.head = lst.tail = NULL;
	return lst;
}
bool isEmptySquareLst(SquareList* lst)
{
	return lst->head == NULL;
}
SquareLstNode* createNewSquareListNode(Square p, SquareLstNode* next)
{
	SquareLstNode* node = (SquareLstNode*)malloc(sizeof(SquareLstNode));
	node->sq = &p;
	node->next = next;
	return node;
}
void insertSquareNodeToTail(SquareList* lst, SquareLstNode* n)
{
	n->next = NULL;
	if (lst->head == NULL)
		lst->head = lst->tail = n;
	else
	{
		lst->tail->next = n;
		lst->tail = n;
	}
}


void removeSquareNode(SquareList* lst, SquareLstNode* beforeToDelete)
{
	if (beforeToDelete == lst->head)
		removeSquareNodeFromHead(lst);
	else if (beforeToDelete->next == lst->tail)
		removeSquareNodeFromTail(lst);
	else
		removeSquareNodeFromMid(lst);
}

void removeSquareNodeFromHead(SquareList* lst)
{
	SquareLstNode* tmp;
	if (lst->head == lst->tail)
	{
		free(lst->head);
		lst->head = lst->tail = NULL;
	}
	else
	{
		tmp = lst->head;
		lst->head = lst->head->next;
		free(tmp);
	}
}
void removeSquareNodeFromTail(SquareList* lst, SquareLstNode* n)
{
	free(n->next);
	n->next = NULL;
	lst->tail = n;
}
void removeSquareNodeFromMid(SquareLstNode* n)
{
	SquareLstNode* tmp = n->next;
	n->next = tmp->next;
	free(tmp);
}


bool checkingPossiblityAtLinesAndCols(short board[][9] , Square sq, int num)
{
	unsigned short i;
	unsigned short j;
	short row, col;
	bool possibleRow = true;
	bool possibleCol = true;

	row = sq.row;
	col = sq.col;

	for (i = col + 1; i < 9; i++)
	{
		if (board[row][i] == num)
		{
			possibleRow = false;
			break;
		}
	}

	for (i = col - 1; possibleRow == true && i > 0; i--)
	{
		if (board[row][i] == num)
		{
			possibleRow = false;
			break;
		}
	}

	for (i = row + 1; i < 9; i++)
	{
		if (board[i][col] == num)
		{
			possibleCol = false;
			break;
		}
	}

	for (i = row - 1; possibleCol == true && i > 0; i--)
	{
		if (board[i][col] == num)
		{
			possibleCol = false;
			break;
		}
	}
	return possibleRow && possibleCol;
}


bool checkingPossiblityAtTheSquare(Board[][9] board, Square sq, int num)
{
	unsigned short i;
	unsigned short j;
	short row, col;
	short rowReduce, colReduce;

	row = sq.row;
	col = sq.col;
	rowReduce = row % 3;
	colReduce = col % 3;

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			if (board[row - rowReduce + i][col - colReduce + j] == num)
				return false;
		}
	}
	return true;
}


short* arrayOfPossibleDigits(short board[][9] , unsigned short row, unsigned short col, unsigned short* length)
{
	Square* sq;
	short* arr;
	short Mai[9];
	unsigned short size = 0;
	unsigned short i;
	unsigned short counter = 0;
	unsigned short index = 0;

	sq = (Square*)malloc(sizeof(Square));

	sq.row = row;
	sq.col = col;

	for (i = 1; i <= 9; i++)
	{
		if (checkingPossiblityAtTheSquare(board, sq, i) && checkingPossiblityAtLinesAndCols(board, sq, i))
		{
			Mai[i] = 1;
			counter++;
		}
	}

	arr = (int*)malloc(counter * sizeof(int));

	for (i = 1; i <= 9; i++)
	{
		if (Mai[i] = 1)
		{
			arr[index] = i;
			index++;
		}
	}
	*length = counter;
	return arr;
}


Array*** PossibleDigits(short sudokuBoard[][9])
{
	Array* pointers[9][9];
	unsigned short i;
	unsigned short j;
	unsigned short possibleNumsLength;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (sudokuBoard[i][j] != -1)
				*pointers[i][j]->arr = NULL;
			else
			{
				*pointers[i][j]->arr = arrayOfPossibleDigits(sudokuBoard, i, j, &possibleNumsLength);
				*pointers[i][j].size = possibleNumsLength;
			}
		}
	}
	return pointers;
}

bool IsfullBoard(short board[][9])
{
	unsigned short i;
	unsigned short j;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (board[i][j] == -1)
				return false;
		}
	}
	return true;
}


void minSize(Array*** possibilities, int* x, int* y)
{
	unsigned short i;
	unsigned short j;
	unsigned short minsize = *possibilities[0][0]->size;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (*possibilities[i][j]->size < minsize)
			{
				minsize = *possibilities[i][j]->size;
				*x = i;
				*y = j;
			}
		}
	}
}

int OneStage(short board[][9], Array*** possibilities, int* x, int* y)
{
	unsigned short i;
	unsigned short j;
	bool isOneOption = false;

	while (!IsfullBoard(board))
	{
		for (i = 0; i < 9; i++)
		{
			for (j = 0; j < 9; j++)
			{
				if (*possibilities[i][j]->size == 1)
				{
					board[i][j] = *possibilities[i][j]->arr[0];
					*possibilities[i][j]->arr = NULL;//Mai asks - FREE????
					possibilities = PossibleDigits(board);
					isOneOption = true;
				}
			}
		}
		if (!IsfullBoard(board) && !isOneOption)
		{
			minSize(possibilities, x, y);
			return NOT_FINISH;
		}
		isOneOption = false;
	}

	if (IsfullBoard(board))
		return FINISH_SUCCESS;
	else
		return FINISH_FAILURE;
}


bool isLegalOption(Array array, short choice)
{
	unsigned short i;

	for (i = 0; i < array.size; i++)
	{
		if (array->arr[i] == choice)
			return true;
	}
	return false;
}

int FillBoard(short board[][9], Array*** possibilities)
{
	unsigned short i;
	int row;
	int col;
	int status;
	short choice;

	status = OneStage(board, possibilities, &row, &col);

	while (status == NOT_FINISH)
	{
		printf("Cell number [%d,%d] currently holds the minimum number of possible values, select one of the below:\n", row, col);
		for (i = 0; i < *possibilities[row][col]->size; i++)
			printf("%d. %d\n", i + 1, *possibilities[row][col]->arr[i]);

		scanf("%d", &choice);
		if (isLegalOption(*possibilities[row][col], choice))
		{
			board[row][col] = choice;
			possibilities = PossibleDigits(board);
			status = OneStage(board, possibilities, row, col);
		}
		else
			return FAIL;
	}
	if (status == FINISH_SUCCESS)
		return FILLED;
	else
		return FAIL;
			//OneStage(board, possibilities, int* x, int* y)
}


SquareLstNode* findSquare(SquareList* sqlst,Square* square)
{
	SquareLstNode* prevNode = NULL;
	SquareLstNode* curNode;

	curNode = sqlst->head;

	if (curNode->sq == square)
		return curNode;

	while (curNode != NULL)
	{
		if (curNode->sq == square)
			return prevNode;
		prevNode = curNode;
		curNode = curNode->next;
	}
	return NULL;
}


short** BuildBoard(Array*** legalOptions)
{
	short row,col,num;
	unsigned short N,K,i,j;
	unsigned short size = 81,lengthArr;
	Square* sq;
	short board[9][9];
	SquareList lst;
	SquareLstNode* sqlstnode;
	Array*** legalOptions;
	short* optionsArr;


	lst = makeEmptySquareList();

	for (i = 0; i < 10; i++)
	{ 
		for (j = 0; j < 10; j++)
		{ 
		    board[i][j] = -1;
			sq = (Square*)malloc(sizeof(Square));
			sq->row = i;
			sq->col = j;
			sqlstnode = createNewSquareListNode(*sq, NULL);
			insertSquareNodeToTail(&lst, sqlstnode);
			free(sq);
		}
	}

	N = rand() % 20 + 1;

	for (i = 0; i < N; i++)
	{
		K = rand() % size + 1;
		size--;
		//num = rand() % 9 + 1;
		row = (K-1) / 9;
		col = (K - 1) % 9;

		sq = (Square*)malloc(sizeof(Square));
		checkMemoryAllocation(sq);
		sq->row = row;
		sq->col = col;

		sqlstnode = findSquare(&lst, sq);
		removeSquareNode(&lst, sqlstnode);
		free(sq);

		optionsArr = arrayOfPossibleDigits(board, row, col, &lengthArr);

	    num = optionsArr[rand() % lengthArr];
		board[row][col] = num;

		legalOptions = PossibleDigits(board);
		//Mai wants to check
	}
	return board;
}


void checkMemoryAllocation(void* ptr)
{
	if (ptr == NULL)
	{
		puts("The memory allocation has failed!!");
		exit(1);
	}
}



void sudokoPrintBoard(short board[][9])
{
	unsigned short i, j;

	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
			printf("%d ", board[i][j]);

		printf("\n");
	}
}






void main()
{
	short board[9][9] =

	{ 5,-1, 4,-1, 7,-1,-1, 1,-1,

	 6,-1, 2, 1,-1,-1, 3,-1,-1,

	 1,-1, 8,-1, 4,-1,-1, 6,-1,

	-1, 5,-1,-1, 6,-1,-1, 2,-1,

	-1, 2,-1, 8,-1, 3,-1,-1,-1,

	-1,-1,-1,-1,-1, 4,-1, 5, 6,

	-1, 6, 1, 5, 3, 7, 2, 8, 4,

	-1, 8, 7,-1, 1, 9,-1, 3,-1,

	-1,-1,-1, 2, 8,-1,-1,-1, 9 };


	Array*** possible_solutions;


	printf("Initial board\n");

	sudokoPrintBoard(board);



	printf("Press enter to start playing...\n");

	getchar();

	possible_solutions = PossibleDigits(board);



	if (FillBoard(board, possible_solutions) == -1)

		printf("User's selections led to duplications\n");

	else

	{

		sudokoPrintBoard(board);

		printf("Board filled successfully\n");

	}

	// Don't forget to free memory!
}

