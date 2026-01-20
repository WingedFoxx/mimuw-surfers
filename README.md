# MIMUW-Surfers 

The main objective of this task was to implement a Game in Unreal Engine of our own choice (or extend the one presented during the labs).
We've decided to implement our game -- **a variation of Subway Surfers** -- from scratch with selected elements that should have been included in the final solution. 
The extensions that we've decided to include in our game are pointed out in the `Task` section below. 

Additionally we've included presentation with introduction of these implemented elements that includes screenshots of these elements, but it is available only in Polish language.

Project was made in cooperation of: 

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore -->
| [<img src="https://avatars.githubusercontent.com/u/90320028?v=4" width="75px;"/><br /><sub><b>Agnieszka Suszko</b></sub>](https://github.com/WingedFoxx)<br />  | [<img src="https://avatars1.githubusercontent.com/u/100534891?v=4" width="75px;"/><br /><sub><b>Konrad Mocarski</b></sub>](https://github.com/mocar27)<br /> 
|:----------------------------------------------------------------------------------------------------------------------------------------------------------:|:----------------------------------------------------------------------------------------------------------------------------------------------------------------:|
<!-- ALL-CONTRIBUTORS-LIST:END -->

## Task

Develop a game with Unreal Engine 5/C++ -- either extend the one presented during laboratories or come up with your own. 
The game should include the following requirements of your own choice (in the parenthesis there is our :

- [x] Build + playability
- [x] Input
  - W/Space bar - Jump,
  - A - Switch to left lane
  - D - Switch to right lane
  - S - Dodge
  - ESC - go back to main menu
- [x] Scenes loading
  - Dynamic map loading -- always different level, as level is generated during player position based on levels blueprints as the player traverse through the map
- [x] Animations
  - We've added animation for player to be able to dodge under high obstacles and loot animation on coins available for the player to collect to increase the final score
- [x] Collision detection
  - As in classic Subway Surfers, player loses the run/game if he hits any obstacle in front of him
- [x] Particles
  - Upon player collision with the obstacle, the player explodes and related Niagara effect is displayed
  - Each coin on the map is reflective shiny and it rotates
- [x] Advanced materials
  - We've added Subway Surfers available sound to play in the background while playing
  - Additionally we've added sound effects to almost everything in the game - dodge, coin collecting, death, etc.
  - We've also added graphics for different materials to make the game feel more alive (trains, coins, player, obstacles, floor, boosters)
- [x] Interactive UI
  - Play and Exit button available in the game's main menu
  - Retry button appears upon loss to let player play again
  - Player's distance and coin amount is saved at the end, along with the highest earned score by the player
- [x] Chosen mechanics
  - As our chosen mechanics, we've added coins collecting logic for player to increase his score
  - We've additionally added boosters for player to collect along the way, such as x2 coins multiplier and boots that allow the player to jump higher, each of the boosts lasts 5 seconds
  
## Compilation and running 

Project can be cloned or downloaded as .zip from this Github repository and opened in Unreal Engine using `.uproject` file. 
Unreal Engine will configure all the neccessary binaries. Then it can be opened using UE in JetBrains Rider IDE (or other preferred IDE by the user - we've user Rider)
for C++ impementation and file edition. Project can be run inside UE Editor for testing and can be exported to `.exe` application from the Unreal dashboard as the final product.
