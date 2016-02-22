#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <ostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <climits>

using namespace std;

/* Structure to store each pit details */
struct pitNode
{
	int noOfStones;
	string pitName;
};

/* Structure to maintain all board details */
struct moveNode
{
	vector<pitNode> pitDetailsP1;
	vector<pitNode> pitDetailsP2;
	vector <vector<pitNode> > pitDetailsP1Child;
	vector <vector<pitNode> > pitDetailsP2Child;
	int depth;
	string name;
	int parent;
	int cost;
	int nodeNumber;
	bool mancalaStone;
	bool flagOver;
	int playerId;
	bool nextState;
	moveNode()
	{
		mancalaStone = false;
		flagOver = false;
	}
};

/* Structure for maintaining log of the traversal of game tree */
struct traverseLog
{
	string nodeName;
	int value;
	int height;
};

bool sortLength(const moveNode & left, const moveNode & right)
{
	if (left.name.length() == right.name.length())
		return left.name < right.name;
	else
		return left.name.length() < right.name.length();
}

vector <moveNode> updatedBoard;

int user;
int algo;
int alpha = INT_MIN;
int beta = INT_MAX;

/* ****************** FUNCTION DECLARATIONS *************************** */
int maxValue(vector<int>, moveNode, map <string, string>, map <string, string>, int, int);
int minValue(vector<int>, moveNode, map <string, string>, map <string, string>, int, int);
int maxValueAlphaBeta(vector<int>, moveNode, map <string, string>, map <string, string>, int, int, int, int);
int minValueAlphaBeta(vector<int>, moveNode, map <string, string>, map <string, string>, int, int, int, int);
string output(int);
moveNode moveStones(vector<int>, map <string, string>, map <string, string>, moveNode, string, int);
vector<string> computeMoves(vector<int>, moveNode, map <string, string>, map <string, string>, int);
int evalFunc(vector<int>, moveNode);
void MiniMax(vector<int>, moveNode, map <string, string>, map <string, string>, int, int);
void MiniMaxAlphaBeta(vector<int>, moveNode, map <string, string>, map <string, string>, int, int);


string output(int val)
{
	if (val == INT_MAX)
	{
		return "Infinity";
	}

	else if (val == INT_MIN)
	{
		return "-Infinity";
	}

	else
	{
		stringstream ss;
		ss.clear();
		ss << val;
		return ss.str();
	}
}

/* Function to move stones when a particular pit is selected */
moveNode moveStones(vector<int> boardStateP1, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, moveNode initial, string pitSelected, int player)
{
	/* ************** Move stones if  a Player1 pit is selected ********** */
	if (player == 1)
	{
		bool flag = false;

		for (int i = 0; i < boardStateP1.size(); i++)
		{
			if (initial.pitDetailsP1[i].pitName == pitSelected)
			{
				int num = initial.pitDetailsP1[i].noOfStones;
				int leftStones1 = initial.pitDetailsP1[i].noOfStones;
				initial.pitDetailsP1[i].noOfStones = 0;
				int j = i + 1;
				int k = boardStateP1.size() - 1;
				for (int l = 0; l < num; l++)
				{
					if (flag == true)
					{
						if (k == 0)
						{
							flag = false;
							j = 0;
							k = boardStateP1.size() - 1;
							l--;
							continue;
						}
						initial.pitDetailsP2[k].noOfStones = initial.pitDetailsP2[k].noOfStones + 1;
						leftStones1--;
						k--;
						initial.mancalaStone = false;

					}

					else if ((initial.pitDetailsP1[j].noOfStones == 0 && j != boardStateP1.size() - 1 && leftStones1 == 1) || (initial.pitDetailsP1[j].noOfStones == 0 && j == i && leftStones1 == 1))
					{
						for (int m = 0; m < boardStateP1.size(); m++)
						{
							if (initial.pitDetailsP2[m].pitName == oppositePitsP1[initial.pitDetailsP1[j].pitName])
							{
								initial.pitDetailsP1[boardStateP1.size() - 1].noOfStones = initial.pitDetailsP1[boardStateP1.size() - 1].noOfStones + initial.pitDetailsP2[m].noOfStones + 1;
								leftStones1--;
								initial.pitDetailsP2[m].noOfStones = 0;
							}
						}
						initial.mancalaStone = false;

					}
					else
					{
						if (leftStones1 == 1 && j == boardStateP1.size() - 1)
						{
							initial.mancalaStone = true;
						}
						else
						{
							initial.mancalaStone = false;
						}
					
						initial.pitDetailsP1[j].noOfStones = initial.pitDetailsP1[j].noOfStones + 1;
						leftStones1--;
						if (j == boardStateP1.size() - 1)
						{
							flag = true;
							continue;
						}
						j++;
					}
				}
			}
		}
	}

	/* ************** Move stones if  a Player2 pit is selected ********** */
	else if (player == 2)
	{
		bool flag1 = false;

		for (int i = 0; i < boardStateP1.size(); i++)
		{
			if (initial.pitDetailsP2[i].pitName == pitSelected)
			{
				int num1 = initial.pitDetailsP2[i].noOfStones;
				int leftStones2 = initial.pitDetailsP2[i].noOfStones;
				initial.pitDetailsP2[i].noOfStones = 0;
				int j = i - 1;
				int k = 0;
				for (int l = 0; l < num1; l++)
				{
					if (flag1 == true)
					{
						if (k == boardStateP1.size() - 1)
						{
							j = boardStateP1.size() - 1;
							flag1 = false;
							l--;
							k = 0;
							continue;
						}
						leftStones2--;
						initial.pitDetailsP1[k].noOfStones = initial.pitDetailsP1[k].noOfStones + 1;
						k++;
						initial.mancalaStone = false;

					}

					else if ((initial.pitDetailsP2[j].noOfStones == 0 && j != 0 && leftStones2 == 1) || (initial.pitDetailsP2[j].noOfStones == 0 && j == i && leftStones2 == 1))
					{
						for (int m = 0; m < boardStateP1.size(); m++)
						{
							if (initial.pitDetailsP1[m].pitName == oppositePitsP2[initial.pitDetailsP2[j].pitName])
							{
								initial.pitDetailsP2[0].noOfStones = initial.pitDetailsP2[0].noOfStones + initial.pitDetailsP1[m].noOfStones + 1;
								leftStones2--;
								initial.pitDetailsP1[m].noOfStones = 0;
							}
						}
						initial.mancalaStone = false;

					}
					else
					{
						if (leftStones2 == 1 && j == 0)
						{
							initial.mancalaStone = true;
						}
						else
						{
							initial.mancalaStone = false;
						}
						
						initial.pitDetailsP2[j].noOfStones = initial.pitDetailsP2[j].noOfStones + 1;
						leftStones2--;
						if (j == 0)
						{
							flag1 = true;
							continue;
						}
						j--;
					}
				}
			}
		}	
	}

	/* Game Over condition: If all pits are empty of Player 1 */
	int k1 = 0;
	for (int j = 0; j < boardStateP1.size() - 1; j++)
	{
		if (initial.pitDetailsP1[j].noOfStones == 0)
			k1++;
	}
	if (k1 == boardStateP1.size() - 1)
	{
		
		for (int i = 1; i < boardStateP1.size(); i++)
		{
			initial.pitDetailsP2[0].noOfStones = initial.pitDetailsP2[0].noOfStones + initial.pitDetailsP2[i].noOfStones;
			initial.pitDetailsP2[i].noOfStones = 0;
		}
	}
	
	/* Game Over condition: If all pits are empty of Player 2 */
	int k2 = 0;
	for (int j = 1; j < boardStateP1.size(); j++)
	{
		if (initial.pitDetailsP2[j].noOfStones == 0)
			k2++;
	}
	if (k2 == boardStateP1.size() - 1)
	{
		
		for (int i = 0; i < boardStateP1.size() - 1; i++)
		{
			initial.pitDetailsP1[boardStateP1.size() - 1].noOfStones = initial.pitDetailsP1[boardStateP1.size() - 1].noOfStones + initial.pitDetailsP1[i].noOfStones;
			initial.pitDetailsP1[i].noOfStones = 0;
		}
	}
	
	if ((k2 == boardStateP1.size() - 1) || (k1 == boardStateP1.size() - 1))
		initial.flagOver = true;
	else
		initial.flagOver = false;

	initial.name = pitSelected;
	initial.playerId = player;
	return initial;
	
}

/* Function to compute legal moves: Pits that have stones left */
vector<string> computeMoves(vector<int> boardStateP1, moveNode initial, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, int player)
{
	moveNode obP1;
	vector <string> legalMoves;

	/* ************* Legal Moves ****************/
	if (player == 1)
	{
		for (int i = 0; i < boardStateP1.size() - 1; i++)
		{
			if (initial.pitDetailsP1[i].noOfStones > 0)
			{
				legalMoves.push_back(initial.pitDetailsP1[i].pitName);
			}
		}
	}

	else if (player == 2)
	{
		for (int i = 1; i < boardStateP1.size(); i++)
		{
			if (initial.pitDetailsP2[i].noOfStones > 0)
			{
				legalMoves.push_back(initial.pitDetailsP2[i].pitName);
			}
		}
	}
	return legalMoves;
}

/* Function to evaluate which pit should be selected as a next move */
int evalFunc(vector<int> boardStateP1, moveNode initial)
{
	int evalValue = 0;
	if (user == 1)
	{
		evalValue = initial.pitDetailsP1[boardStateP1.size() - 1].noOfStones - initial.pitDetailsP2[0].noOfStones;
	}
	
	else
	{
		evalValue = initial.pitDetailsP2[0].noOfStones - initial.pitDetailsP1[boardStateP1.size() - 1].noOfStones;
	}
	
	return evalValue;
}

/* ********************* MINIMAX ALGORITHM ********************* */
int minValue(vector<int> boardStateP1, moveNode initial, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, int cutOffDepth, int player)
{
	int currentCost = 0;
	int value = INT_MAX;
	vector <string> moves;
	moveNode returnedBoard;


	if ((initial.depth == cutOffDepth && initial.mancalaStone == false) || initial.flagOver == true)
	{
		currentCost = evalFunc(boardStateP1, initial);
		initial.cost = currentCost;
		updatedBoard[initial.nodeNumber].cost = currentCost;
		if (algo == 2)
		{
		ofstream oFile;

		oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
		oFile << endl << initial.name << "," << initial.depth << "," << output(currentCost);
		oFile.close();
		}
		return currentCost;
	}

	moves = computeMoves(boardStateP1, initial, oppositePitsP1, oppositePitsP2, player);

	
	for (int i = 0; i < moves.size(); i++)
	{
		returnedBoard = moveStones(boardStateP1, oppositePitsP1, oppositePitsP2, initial, moves[i], player);
		returnedBoard.nodeNumber = updatedBoard.size();
		returnedBoard.parent = initial.nodeNumber;
		if (updatedBoard[returnedBoard.parent].mancalaStone == true)
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth;
		}
		else
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth + 1;
		}
		updatedBoard.push_back(returnedBoard);

		if (returnedBoard.mancalaStone == true)
		{
			if (algo == 2)
			{
				ofstream oFile;

				oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
				oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value);

				oFile.close();
			}
			value= min(value, minValue(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, player));
		}

		else
		{
			if (algo == 2)
			{
				ofstream oFile;

				oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
				oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value);

				oFile.close();
			}

			value = min(value, maxValue(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, (player == 1) ? 2 : 1));
		}
	}
	initial.cost = value;
	updatedBoard[initial.nodeNumber].cost = value;
	if (algo == 2)
	{
		ofstream oFile;

		oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
		oFile << endl << initial.name << "," << initial.depth << "," << output(value);

		oFile.close();
	}
	return value;
}

int maxValue(vector<int> boardStateP1, moveNode initial, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, int cutOffDepth, int player)
{
	int currentCost = 0;
	int value = INT_MIN;
	vector <string> moves;
	moveNode returnedBoard;


	if ((initial.depth == cutOffDepth && initial.mancalaStone == false) || initial.flagOver == true)
	{
		currentCost = evalFunc(boardStateP1, initial);
		initial.cost = currentCost;
		updatedBoard[initial.nodeNumber].cost = currentCost;
		if (algo == 2)
		{
			ofstream oFile;

			oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
			oFile << endl << initial.name << "," << initial.depth << "," << output(currentCost);

			oFile.close();
		}
		return currentCost;
	}

	moves = computeMoves(boardStateP1, initial, oppositePitsP1, oppositePitsP2, player);
	for (int i = 0; i < moves.size(); i++)
	{
		returnedBoard = moveStones(boardStateP1, oppositePitsP1, oppositePitsP2, initial, moves[i], player);
		returnedBoard.nodeNumber = updatedBoard.size() ;
		returnedBoard.parent = initial.nodeNumber; 
		if (updatedBoard[returnedBoard.parent].mancalaStone == true)
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth;
		}
		else
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth + 1;
		}
		updatedBoard.push_back(returnedBoard);

		if (returnedBoard.mancalaStone == true)
		{
			if (algo == 2)
			{
				ofstream oFile;

				oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
				oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value);

				oFile.close();
			}
			value = max(value, maxValue(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, player));
		}
	
		else
		{
			if (algo == 2)
			{
				ofstream oFile;

				oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
				oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value);

				oFile.close();
			}
			value = max(value, minValue(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, (player == 1) ? 2 : 1));
		}
	}
	initial.cost = value;
	updatedBoard[initial.nodeNumber].cost = value;
	if (algo == 2)
	{
		ofstream oFile;

		oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
		oFile << endl << initial.name << "," << initial.depth << "," << output(value);
		oFile.close();
	}
	return value;
}

void MiniMax(vector<int> boardStateP1, moveNode initial, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, int cutOffDepth, int player)
{
	int returnCost = 0;
	initial.nodeNumber = 0;
	initial.depth = 0;
	initial.parent = -1;
	bool stateFlag = false;
	moveNode printState;
	initial.name = "root";
	updatedBoard.push_back(initial);
	if (algo == 2)
	{
		ofstream oFile;
		oFile.open("traverse_log.txt", ofstream::out | ofstream::trunc);
		oFile << "Node,Depth,Value";
		oFile.close();
	}
	returnCost = maxValue(boardStateP1, initial, oppositePitsP1, oppositePitsP2, cutOffDepth, player);

	vector<moveNode> tempBoard;
	for (int i = 0; i < updatedBoard.size(); i++)
	{
		if (updatedBoard[i].cost == returnCost && updatedBoard[i].playerId == user && updatedBoard[i].parent == 0)
		{

			if (updatedBoard[i].mancalaStone == false || (updatedBoard[i].mancalaStone == true && updatedBoard[i].flagOver == true))
			{
				updatedBoard[i].nextState = false;
				tempBoard.push_back(updatedBoard[i]);
			}

			else if (updatedBoard[i].mancalaStone == true && updatedBoard[i].flagOver == false)
			{
				updatedBoard[i].nextState = true;

				tempBoard.push_back(updatedBoard[i]);
			}
		}
	}
			sort(tempBoard.begin(), tempBoard.end(), sortLength);
			if (tempBoard[0].nextState == false)
			{
				ofstream oFile1;
				oFile1.open("next_state.txt", ofstream::out | ofstream::trunc);

				for (int j = 1; j < boardStateP1.size(); j++)
				{
					oFile1 << tempBoard[0].pitDetailsP2[j].noOfStones << " ";
				}

				oFile1 << endl;

				for (int j = 0; j < boardStateP1.size() - 1; j++)
				{
					oFile1 << tempBoard[0].pitDetailsP1[j].noOfStones << " ";
				}

				oFile1 << endl;

				oFile1 << tempBoard[0].pitDetailsP2[0].noOfStones << endl;

				oFile1 << tempBoard[0].pitDetailsP1[boardStateP1.size() - 1].noOfStones;

				oFile1.close();
			}
			else if (tempBoard[0].nextState == true)
			{
				for (int i = 0; i < updatedBoard.size(); i++)
				{
					if (updatedBoard[i].name == tempBoard[0].name && updatedBoard[i].cost == returnCost && updatedBoard[i].playerId == user && updatedBoard[i].parent == 0)
					{
						for (int k = 0; k < updatedBoard.size(); k++)
						{
							if (updatedBoard[k].parent == updatedBoard[i].nodeNumber && updatedBoard[k].mancalaStone != true && updatedBoard[k].cost == returnCost  || (updatedBoard[k].mancalaStone == true && updatedBoard[k].flagOver == true && updatedBoard[k].parent == updatedBoard[i].nodeNumber  && updatedBoard[k].cost == returnCost))
							{
								printState = updatedBoard[k];
								break;
							}

							else if (updatedBoard[k].parent == updatedBoard[i].nodeNumber && updatedBoard[k].mancalaStone != false && updatedBoard[k].cost == returnCost)
							{
								i = k;
								printState = updatedBoard[k];

								continue;
							}
						}
					}
				}

					ofstream oFile1;
					oFile1.open("next_state.txt", ofstream::out | ofstream::trunc);

					for (int j = 1; j < boardStateP1.size(); j++)
					{
						oFile1 << printState.pitDetailsP2[j].noOfStones << " ";
					}

					oFile1 << endl;

					for (int j = 0; j < boardStateP1.size() - 1; j++)
					{
						oFile1 << printState.pitDetailsP1[j].noOfStones << " ";
					}

					oFile1 << endl;

					oFile1 << printState.pitDetailsP2[0].noOfStones << endl;

					oFile1 << printState.pitDetailsP1[boardStateP1.size() - 1].noOfStones;

					oFile1.close();
			}
}


/* ********************** ALPHA BETA ALGORITHM **************************** */

int minValueAlphaBeta(vector<int> boardStateP1, moveNode initial, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, int cutOffDepth, int player, int alpha, int beta)
{
	int currentCost = 0;
	int value = INT_MAX;
	vector <string> moves;
	moveNode returnedBoard;


	if ((initial.depth == cutOffDepth && initial.mancalaStone == false) || initial.flagOver == true)
	{
		currentCost = evalFunc(boardStateP1, initial);
		initial.cost = currentCost;
		updatedBoard[initial.nodeNumber].cost = currentCost;
		
		ofstream oFile;
		oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
		oFile << endl << initial.name << "," << initial.depth << "," << output(currentCost) << "," << output(alpha) << "," << output(beta);
		oFile.close();

		return currentCost;
	}

	moves = computeMoves(boardStateP1, initial, oppositePitsP1, oppositePitsP2, player);


	for (int i = 0; i < moves.size(); i++)
	{
		returnedBoard = moveStones(boardStateP1, oppositePitsP1, oppositePitsP2, initial, moves[i], player);
		returnedBoard.nodeNumber = updatedBoard.size();
		returnedBoard.parent = initial.nodeNumber;
		if (updatedBoard[returnedBoard.parent].mancalaStone == true)
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth;
		}
		else
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth + 1;
		}
		updatedBoard.push_back(returnedBoard);

		if (returnedBoard.mancalaStone == true)
		{
			ofstream oFile;
			oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
			oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value) << "," << output(alpha) << "," << output(beta);

			oFile.close();
			value = min(value, minValueAlphaBeta(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, player, alpha, beta));
		}

		else
		{
			ofstream oFile;
			oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
			oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value) << "," << output(alpha) << "," << output(beta);

			oFile.close();

			value = min(value, maxValueAlphaBeta(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, (player == 1) ? 2 : 1, alpha, beta));
		}
		if (value <= alpha)
		{
			initial.cost = value;
			ofstream oFile;

			oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
			oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value) << "," << output(alpha) << "," << output(beta);

			oFile.close();
			updatedBoard[initial.nodeNumber].cost = value;
			return value;

		}
		beta = min(beta, value);
	}
	initial.cost = value;
	updatedBoard[initial.nodeNumber].cost = value;

	ofstream oFile;
	oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
	oFile << endl << initial.name << "," << initial.depth << "," << output(value) << "," << output(alpha) << "," << output(beta);
	oFile.close();

	return value;
}

int maxValueAlphaBeta(vector<int> boardStateP1, moveNode initial, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, int cutOffDepth, int player, int alpha, int beta)
{
	int currentCost = 0;
	int value = INT_MIN;
	vector <string> moves;
	moveNode returnedBoard;


	if ((initial.depth == cutOffDepth && initial.mancalaStone == false) || initial.flagOver == true)
	{
		currentCost = evalFunc(boardStateP1, initial);
		initial.cost = currentCost;
		updatedBoard[initial.nodeNumber].cost = currentCost;
		
		ofstream oFile;
		oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
		oFile << endl << initial.name << "," << initial.depth << "," << output(currentCost) << "," << output(alpha) << "," << output(beta);
		oFile.close();
		
		return currentCost;
	}

	moves = computeMoves(boardStateP1, initial, oppositePitsP1, oppositePitsP2, player);
	for (int i = 0; i < moves.size(); i++)
	{
		returnedBoard = moveStones(boardStateP1, oppositePitsP1, oppositePitsP2, initial, moves[i], player);
		returnedBoard.nodeNumber = updatedBoard.size();
		returnedBoard.parent = initial.nodeNumber;
		if (updatedBoard[returnedBoard.parent].mancalaStone == true)
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth;
		}
		else
		{
			returnedBoard.depth = updatedBoard[returnedBoard.parent].depth + 1;
		}
		updatedBoard.push_back(returnedBoard);

		if (returnedBoard.mancalaStone == true)
		{
			ofstream oFile;
			oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
			oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value) << "," << output(alpha) << "," << output(beta);
			oFile.close();
			
			value = max(value, maxValueAlphaBeta(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, player, alpha, beta));
		}

		else
		{
			ofstream oFile;
			oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
			oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value) << "," << output(alpha) << "," << output(beta);
			oFile.close();

			value = max(value, minValueAlphaBeta(boardStateP1, returnedBoard, oppositePitsP1, oppositePitsP2, cutOffDepth, (player == 1) ? 2 : 1, alpha, beta));
		}
		if (value >= beta)
		{
			initial.cost = value;
			ofstream oFile;
			oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
			oFile << endl << updatedBoard[returnedBoard.parent].name << "," << updatedBoard[returnedBoard.parent].depth << "," << output(value) << "," << output(alpha) << "," << output(beta);

			oFile.close();
			updatedBoard[initial.nodeNumber].cost = value;
			return value;

		}
		alpha = max(alpha, value);
	}
	initial.cost = value;
	updatedBoard[initial.nodeNumber].cost = value;
	ofstream oFile;

		oFile.open("traverse_log.txt", ofstream::out | ofstream::app);
		oFile << endl << initial.name << "," << initial.depth << "," << output(value) << "," << output(alpha) << "," << output(beta);
		oFile.close();
	
	return value;
}

void MiniMaxAlphaBeta(vector<int> boardStateP1, moveNode initial, map <string, string> oppositePitsP1, map <string, string> oppositePitsP2, int cutOffDepth, int player)
{
	int returnCost = 0;
	initial.nodeNumber = 0;
	initial.depth = 0;
	bool stateFlag = false;
	initial.parent = -1;
	moveNode printState;
	initial.name = "root";
	updatedBoard.push_back(initial);

	ofstream oFile;
	oFile.open("traverse_log.txt", ofstream::out | ofstream::trunc);
	oFile << "Node,Depth,Value,Alpha,Beta";
	oFile.close();

	returnCost = maxValueAlphaBeta(boardStateP1, initial, oppositePitsP1, oppositePitsP2, cutOffDepth, player, alpha, beta);
	vector<moveNode> tempBoard;
	for (int i = 0; i < updatedBoard.size(); i++)
	{
		if (updatedBoard[i].cost == returnCost && updatedBoard[i].playerId == user && updatedBoard[i].parent == 0)
		{

			if (updatedBoard[i].mancalaStone == false || (updatedBoard[i].mancalaStone == true && updatedBoard[i].flagOver == true))
			{
				updatedBoard[i].nextState = false;
				tempBoard.push_back(updatedBoard[i]);
			}

			else if (updatedBoard[i].mancalaStone == true && updatedBoard[i].flagOver == false)
			{
				updatedBoard[i].nextState = true;

				tempBoard.push_back(updatedBoard[i]);
			}
		}
	}
	sort(tempBoard.begin(), tempBoard.end(), sortLength);
	if (tempBoard[0].nextState == false)
	{
		ofstream oFile1;
		oFile1.open("next_state.txt", ofstream::out | ofstream::trunc);

		for (int j = 1; j < boardStateP1.size(); j++)
		{
			oFile1 << tempBoard[0].pitDetailsP2[j].noOfStones << " ";
		}

		oFile1 << endl;

		for (int j = 0; j < boardStateP1.size() - 1; j++)
		{
			oFile1 << tempBoard[0].pitDetailsP1[j].noOfStones << " ";
		}

		oFile1 << endl;

		oFile1 << tempBoard[0].pitDetailsP2[0].noOfStones << endl;

		oFile1 << tempBoard[0].pitDetailsP1[boardStateP1.size() - 1].noOfStones;

		oFile1.close();
	}
	else if (tempBoard[0].nextState == true)
	{
		for (int i = 0; i < updatedBoard.size(); i++)
		{
			if (updatedBoard[i].name == tempBoard[0].name && updatedBoard[i].cost == returnCost && updatedBoard[i].playerId == user && updatedBoard[i].parent == 0)
			{
				for (int k = 0; k < updatedBoard.size(); k++)
				{
					if (updatedBoard[k].parent == updatedBoard[i].nodeNumber && updatedBoard[k].mancalaStone != true && updatedBoard[k].cost == returnCost || (updatedBoard[k].mancalaStone == true && updatedBoard[k].flagOver == true && updatedBoard[k].parent == updatedBoard[i].nodeNumber  && updatedBoard[k].cost == returnCost))
					{
						printState = updatedBoard[k];
						break;
					}

					else if (updatedBoard[k].parent == updatedBoard[i].nodeNumber && updatedBoard[k].mancalaStone != false && updatedBoard[k].cost == returnCost)
					{
						i = k;
						printState = updatedBoard[k];

						continue;
					}
				}
			}
		}

		ofstream oFile1;
		oFile1.open("next_state.txt", ofstream::out | ofstream::trunc);

		for (int j = 1; j < boardStateP1.size(); j++)
		{
			oFile1 << printState.pitDetailsP2[j].noOfStones << " ";
		}

		oFile1 << endl;

		for (int j = 0; j < boardStateP1.size() - 1; j++)
		{
			oFile1 << printState.pitDetailsP1[j].noOfStones << " ";
		}

		oFile1 << endl;

		oFile1 << printState.pitDetailsP2[0].noOfStones << endl;

		oFile1 << printState.pitDetailsP1[boardStateP1.size() - 1].noOfStones;

		oFile1.close();
	}
}


/* ******************* MAIN ************************ */
int main(void)
{
	int task;
	int player;
	int cutOffDepth;
	vector<int> boardStateP2;
	vector<int> boardStateP1;
	int stonesP2;
	int stonesP1;
	map <string, string> oppositePitsP1;
	map <string, string> oppositePitsP2;
	vector <string> legalMovesP1;
	vector <string> legalMovesP2;

	moveNode initial;

	ifstream inFile;
	inFile.open("input.txt");

	string taskStr;
	getline(inFile, taskStr);
	task = atoi(taskStr.c_str());

	string playerStr;
	getline(inFile, playerStr);
	player = atoi(playerStr.c_str()); 
	user = player;
	string cutOffDepthStr;
	getline(inFile, cutOffDepthStr);
	cutOffDepth = atoi(cutOffDepthStr.c_str());

	string boardStateP2Str;
	getline(inFile, boardStateP2Str);
	
	string boardStateP1Str;
	getline(inFile, boardStateP1Str);
	stringstream ssP1(boardStateP1Str);
	while (ssP1 >> boardStateP1Str)
	{
		boardStateP1.push_back(atoi(boardStateP1Str.c_str()));
	}
	
	string stonesP2Str;
	getline(inFile, stonesP2Str);
	stonesP2 = atoi(stonesP2Str.c_str());

	string stonesP1Str;
	getline(inFile, stonesP1Str);
	stonesP1 = atoi(stonesP1Str.c_str());

	boardStateP1.push_back(stonesP1);

	boardStateP2.push_back(stonesP2);
	stringstream ssP2(boardStateP2Str);
	while (ssP2 >> boardStateP2Str)
	{
		boardStateP2.push_back(atoi(boardStateP2Str.c_str()));
	}

	/* Vector initial.pitDetailsP1 stores pit name, no. of stones for Player 1*/
	for (int i = 0; i < boardStateP1.size(); i++)
	{
		initial.pitDetailsP1.push_back(pitNode());
		stringstream sstm;
		sstm << "B" << i+2;
		initial.pitDetailsP1[i].pitName = sstm.str();
		initial.pitDetailsP1[i].noOfStones = boardStateP1[i];
		sstm.clear();
	}
	
	/* Vector pitDetailsP2 stores pit name, no. of stones for Player 2*/
	for (int i = 0; i < boardStateP2.size(); i++)
	{
		initial.pitDetailsP2.push_back(pitNode());
		stringstream sstm;
		sstm << "A" << i + 1;
		initial.pitDetailsP2[i].pitName = sstm.str();
		initial.pitDetailsP2[i].noOfStones = boardStateP2[i];
		sstm.clear();
	}

	/* ************* Map for storing adjacent pits ************* */
	for (int i = 0; i < boardStateP1.size()-1; i++)
	{
		oppositePitsP1[initial.pitDetailsP1[i].pitName] = initial.pitDetailsP2[i+1].pitName;
	}
	map<string, string>::iterator itr;

	for (int i = 0; i < boardStateP1.size() - 1; i++)
	{
		oppositePitsP2[initial.pitDetailsP2[i + 1].pitName] = initial.pitDetailsP1[i].pitName;
	}

	algo = task;
	if (algo == 1)
		cutOffDepth = 1;

	if (algo == 1 || algo == 2)
	MiniMax(boardStateP1, initial, oppositePitsP1, oppositePitsP2, cutOffDepth, player);

	if (algo == 3)
	{
		MiniMaxAlphaBeta(boardStateP1, initial, oppositePitsP1, oppositePitsP2, cutOffDepth, player);
	}

	getchar();
	return 0;
}


