> Knights Embers by Aum Patel (ACZG896)

# Documentation

> help

## Asset Listing/References

#### Audio

NPC Dialogue : self made, recorded with friends.

Music: from https://freemusicarchive.org/music/Lobo_Loco

Artist: **Lobo Loco**

Songs: 
- 1969 Moonwalk: https://freemusicarchive.org/music/Lobo_Loco/Mr_Tachyon/1969_Moonwalk_ID_1261
- Silentfilm Part A: https://freemusicarchive.org/music/Lobo_Loco/around/silentfilm-part-a-id-1294mp3

License Creative Commons: https://creativecommons.org/licenses/by-nc-nd/4.0/

#### Models

From Turbo Squid (TurboSquid 3D model License https://blog.turbosquid.com/turbosquid-3d-model-license/):

- Argon: https://www.turbosquid.com/3d-models/3d-humanoid-robot-character-1479200
- Rigged and Animated Minotaur: https://www.turbosquid.com/3d-models/character-minotaur-3ds/1044341
- Mannequin Figure: https://www.turbosquid.com/3d-models/free-mannequin-male-3d-model/1005602

#### Textures  

Sky Box : Artist: http://www.humus.name , License: http://creativecommons.org/licenses/by/3.0/

Sea Rock Terrain: https://3dtextures.me/2020/08/24/sea-rock-001/



## Implementation

### Part 1

#### Intro Screen

I implemented an intro screen by creating image textures in photoshop for all the views in the menu (main, help). I then created a thin cuboid and placed it into the scene and fixed the camera to display only the menu. The menu is interactable using key presses. So if the player **presses 1** it will start the game, if they **press 2** it will change the menu object's texture to the help image, and they can press **escape** to exit the game. When then player selects to start the game, I then set the camera behind the player and call the player's ```updateCamera()``` method.

The keyboard and mouse controls are shown in the help menu.

#### Primitive Objects and Skybox and Terrain textures

I have created **2** primitive objects: Octahedron, Bullet Shape.

The Octahedron has 8 sides(8 triangles) and 6 vertices; when creating an instance of the shape, you only have to pass in the scale of the octahedron, and it will create the shape accordingly.

<ADD USES AND TRANSFORMATIONS>

The Bullet Shape has 14 triangles and 9 vertices; when creating an instance of the shape, you pass in the height, the width, and the length of the shape, creating that shape accordingly.

<ADD USES AND TRANSFORMATIONS>

<ADD ANOTHER PRIMITIVE AND CHANGE FIRST LINE>

I have changed both skybox and terrain textures. I have not had the opportunity to work on making the terrain texture repeat rather than stretch.

#### Audio

I have added a friendly NPC into the game that gives you a quest of killing the boss and returning back to receive a reward, there is appropriate audio to make the player understand the quest. There is also a branching path in that the player can answer `yes` or `no` to the question of whether they are a hunter or not which is followed by different dialogue based on the option you choose. The final dialogue in the two branches will loop until the boss has been defeated, and then the player will receive the new dialogue and receive the item reward. I recorded this dialogue with a few friends of mine and tried to make it funny to provide some entertaining value to the player.

I have added 2 music tracks into the game (I wrote the music selection menu code to make it easily scalable to more tracks being added). This menu can be opened by pressing `M` and then choose whichever track by pressing `1` or `2` , or choose `0` to stop all music. 

#### Heads up display (HUD)

I have implemented a few HUD elements into the game.

**Health**: The health shows up as text as a number value from 0-100, and is changed as the player gets damaged.

**Stamina**: this number goes down as the player takes actions to attack. The recharge is set on a timer. this is also a value from 0-100.

**'E to Interact'**: this shows up when there is something the player can interact with within the scene. Example the FriendlyNPC can be interacted with and this will show up when in range. This is changed to `1. yes, 2. no` when the player has to answer to the NPC.

### Part 2

#### Camera Motion Technique

I have implemented a 3rd person and 1st person view. I have changed the **3rd person** template code by mapping the X mouse movement to turning and the Y mouse movement to tilt the camera behind the player. I still need to make it so that the camera movement is limited and the camera does not get to a crazy high position. I implemented the **1st person** camera by fixing the camera in  front of the player object's head. It turns with the player and you can also look up and down.

I also transform the camera to display the menu screen at the beginning of the game.

I have limited the third person camera so it does not go too high up or below the ground. The camera will still clip through object which I felt was not worth improving on as it provided a clearer camera view to the player as it is now.

#### Mesh based Objects

I have implemented multiple mesh based objects.

Minotaur Enemy: <add the implemenation notes here>. The bigger the minotaur, the slower it moves and vice versa. When it gets close enough to the player, it shouts, increasing its size (only the first time it reaches the player), and then starts attacking. These enemies have a melee attack that has a 5% chance every *timestep* to reduce 5 points of the players HP,  I played around with it and this amount of damage seemed fair. When the enemies are not moving and not aggroed, they can be in an idle state. 

There are multiple animations that these mesh based object and multiple levels of transformation (scale and position towards the player). I implemented an `animationHandler()` in Enemy.cpp to change the animation based on what the enemy is doing, this works seamlessly. The enemies also textured.

>  This has collision detection with other collision objects. Allowing the player to damage them.

Level 1: 

Player: 

Friendly NPC : I changed the pose of the rigged mannequin mesh and added it as a static mesh based object into the game.

Boss: This is a Robot style enemy that has multiple movements and multiple attacks, where the enemy stays open for counter attacking by the player after each attack.

> SEE AI SECTION FOR AI IMPLEMENTATION: Add finite state machine in the AI Part

#### Lighting

> One Player transformed light source

There is a point based light source based on the players position (+1.f on the y axis), this is not noticeable as the environment light is on however I implemented it based on the code provided in the tutorial and transformed it based on the player's position.

> have another when an explosion happens 

There is the direction light in the scene as provided in the template code, I had no reason to change this.

#### Special Effects

I have used the billboard special effect, 

> and > cross fade for damage indication



### Part 3

#### Game Physics

> Gravity: bullet drop on the bullets coming but they also interact with the items the player throws at it.



> Add a telekinises/half life style pickup and throw mechanic to damage enemies



#### NPC and AI

> draw branching decision tree for the simple friendlyNPC dialogue



> Show the finite state machine for the boss enemy



> add some sort of flocking algorithm for multiple enemies maybe



#### Gameplay elements

> Health pickup

> fast stamina recovery

> score multiplier



## Reflection

