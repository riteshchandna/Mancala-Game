# Mancala-Game
Artificial Intelligence agent in C++ to play a game of Mancala against a human opponent

Mancala is a two-player game from Africa in which players moves stones around a board, trying to capture as many as possible. There are also two special pits on the board, called Mancalas, in which each player accumulates his or her captured stones.

On a player's turn, he or she chooses one of the pits on his or her side of the board (not the Mancala) and removes all of the stones from that pit. The player then places one stone in each pit, moving counterclockwise around the board, starting with the pit immediately next to the chosen pit, including his or her Mancala but NOT his or her opponents Mancala, until he or she has run out of stones. If the player's last stone ends in his or her own Mancala, the player gets another turn. If the player's last stone ends in an empty pit on his or her own side, the player captures all of the stones in the pit directly across the board from where the last stone was placed (the opponents stones are removed from the pit and placed in the player's Mancala) as well as the last stone placed (the one placed in the empty pit). The game ends when one player cannot move on his or her turn, at which time the other player captures all of the stones remaining on his or her side of the board.

Tasks:
Program to determine the next move by implementing the following algorithms:
Greedy

Minimax

Alpha-Beta

End game:
If a player cannot make any valid move, i.e. all of his pits (except Mancala) are empty, the game ends and the remaining stones are moved to the other player’s Mancala.
