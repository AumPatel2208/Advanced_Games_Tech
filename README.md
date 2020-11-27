# Advanced Games Tech

## top priority
- [ ] change the enemy models to lower poly to reduce file size, as will not have enough space later on


## TO-DO


## Checklist

- [x] intro screen
  - [ ]? top down camera view pan into the gameplay view
  - [ ]? interactible buttons?
- [ ] Primitive based objects [1/3]
  - [ ] with multiple transformations
  - [ ] could have a power up move around the map/scale up and down, stay there for a bit and move around more afterwards
  - [ ] Power Up Effect
    - [ ] texture
- [ ] Skybox and terrain textures
  - [x] skybox
  - [x] terrain
- [ ] Camera motion technique
  - [x] mostly done
  - [x] just need to limit to a radius/square/cube thing
  - [ ] Add  the camera transition from menu to player
- [ ] Mesh-based object
  - [x] Level
    - [ ] add collision
      - [ ] collision with wall using off roading and a fixed path
    - [ ] add texture
  - [ ] add a door?
    - [ ] door (opens and closes) - make it into a gate maybe, opened and closed with a switch
  - [ ] make multiple enemies with movement and different scales and animations
  - [ ] NPC dialogue
    - [x] record dialogue
    - [ ] Fix multiple calls of the dialogue
    - [ ] add yes/no gui
  - [ ] maybe torches
- [ ] HUD
  - [ ] Health
  - [ ] Ammo/Stamina
- [ ] Report
- [ ] Source Code Design, organization and coding style
- [ ] Light
  - [ ] have a light source around the player, lighting up the scene.
- [ ] Effects
  - [ ] on collision have blood or sparks based on what they hit
  - [ ] have a different coloured blood when the player is hit
  - [ ] add fog on the outside
- [ ] mesh based objects
  - [ ] Level
    - [ ] 
  - [ ] 
  - [ ] 
  - [ ] friendly NPC with dialogue
  - [ ] enemies
  - [ ] boss
- [ ] difficulty
  - [ ] it could just have a bell/switch you can activate that ups the health and the attack damage of the enemies
    - [ ] change HP/AP when bell is interacted with
  - [ ] it could just up the difficulty and reset you to the beginning of the level
    - [ ] can be attached to a stop watch showing how long the player has survived in the game.
    



### Dialogue for the NPC

- "hello there"  `greeting_1`
- "hmm, what's that smell" `greeting_2` "you been killing 'em beasts'" `greeting_3`
  - Yes
    - "ahh, a hunter are you, same here" `yes_1`
    - "I think you could help me out with something here" `yes_2`
    - "would you mind going through those doors and killing that big guy in there?" `yes_3`
      - REPEAT
  - No
    - "must be from me then, beasts all over the shop"`no_1`
    - "Would you like to do something for me?" `no_2`
    - "I need that guy on the other side of that door dead? Be a good laddie and help a man out, would ya." `no_3`
      - REPEAT
- After beating the guy. "Here have this." `end_1` {Gives item} "Many thanks for beating him, scary fella he is." `end_2`



## Documentation checklist
- finite state machines for the enemies
- maybe describe the animation handler or something better if i do come up with something
- Had to get rid of death and getting hit animations from minitour enemy for size reasons

## Souls-Like Game

- Player
  - weapons (2 weapons)
    - light attack
    - heavy attack
      - charged attack (stretch)
  - shield
    - parry (sound effect when parry)
  - **health**
  - **stamina**
  - **inventory (active item is highlighted)**

- item pickups
  - key items
  - **XP/souls**
  - **power ups**  
    - stamina boost
    - attack boost
- enemies
  - Types
    - Brute
      - killing brute drops a special weapon
    - Fodder
    - Boss Fight
  - Ragdoll
  - Attack player
    - heavy
    - light
  - block attacks
    - can be staggered if they block too much
  - wait before attacking
    - have it on a random time thing maybe
  - Goal
    - find the key to access the door to the boss fight
  - make something cool happen after killing boss
  
- DIFFICULTY
  - enemy
    - health increase
    - damage increase
    - detection vision cone increases
- CHEAT CODE
  - god mode
- **HUD**
  - **all bold things above**
  - **mini map of sorts**


#### Procedurally generated trees
- https://weigert.vsos.ethz.ch/2020/10/19/transport-oriented-growth-and-procedural-trees/

 



Skybox          

negy = bottom

posy = top

negx = left

posx = right

negz = back

posz = front
