# Simple Take-6-Card-Game

Introduction : Final project of computer programming course.

### A. Basic Information
- An simulation of the card game "Take 6 誰是牛頭王".
- A stand-alone game which the player can play with multiple computer agents.
- Language : C
 
### B. Files : 
- main.c ( main program of the game )
- interface.h ( some text graphic for the main program to use )
- Makefile

### C. Game Instruction
#### 可供玩家設定的部分
1. Number of computer agents
    - 玩家可選擇要跟幾台電腦對戰，可選擇數字: 1 - 9
2. Game difficulty
    - 分為EASY (1) 與HARD (2) ，可選擇數字: 1、2
    - EASY -> 每位玩家於遊戲一開始獲得5張手牌
    - HARD -> 每位玩家於遊戲一開始獲得10張手牌
    - 想法 : 手牌越多張出牌的時要考慮越多，也越容易吃到牛頭
3. Computer agent level
    - Two levels are provided : 1、2
    - Level 1: random出牌
    - Level 2: 由手牌數字由小到大出牌
#### 遊戲規則
1. 回和數 \
   EASY: 5回、HARD: 10回
2. 每位玩家手牌數 \
   EASY: 5張、HARD: 10張
3. 桌面上牌堆: 4排
4. 遊戲開始:
  - 每回合每位玩家從手牌中選出一張牌並同時翻開
  - 根據牌上數字由小到大放入牌堆中
  - 每張牌必須放入最後一張牌數字離它最近，且比它小的牌堆中
  - 若找不到任何一堆將牌放入(玩家的牌比任一牌堆最後一張牌數字還大)，玩家必須選一堆吃掉，玩家的牌成為該堆第一張牌
  - 若欲放入的牌堆已經滿五張，玩家必須將該堆全部的牌吃掉，玩家的牌成為該堆第一張牌
  - 遊戲結束後，吃掉的牌總共的牛頭數越大的人越輸

### D. How to Run
- 在terminal輸入指令
  ``` c = 
  $ make
  ```
- 在terminal輸入指令
  ``` c = 
  $ ./play
  ```
- 按下ENTER鍵進入遊戲 (按下SPACE鍵結束遊戲)
- 輸入computer agent數量 (1-9)
- 輸入遊戲難度 (1、2)
- 輸入computer agent等級 (1、2)
- 開始遊戲
- 遊戲結束: 按下TAB鍵再玩一次 (按下SPACE鍵結束遊戲)

### E. Importtant Notice
- 當程式要求輸入數字時，請勿輸入數字以外的東西，否則將進入無窮迴圈。
- 若不慎輸入錯誤，請按ctrl+C 終止程式再重新執行。

### F. Copyright Claim
- This code is written by Shih-Jieh Chen.
- All rights reserved.
- Date : 2020/06/30
