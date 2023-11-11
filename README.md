# Project Description

## Agar.IQ - A First-Year Project at National Research University Higher School of Economics, Saint Petersburg

### Table of Contents

1. [Description](#description)
2. [About the Project](#about-the-project)
3. [Screenshots and Demo Videos](#screenshots-and-demo-videos)
4. [Dependencies](#dependencies)
5. [Installation](#installation)

---

## Description

**Agar.IQ** is a multiplayer 2D game inspired by Agar.io, created during the first year at the National Research University Higher School of Economics in Saint Petersburg. It combines the familiar controls of Agar.io with a unique gameplay element - the solution of mathematical problems to enhance players' mental arithmetic skills.

### About the Project

**Agar.IQ** closely resembles the classic mechanics of Agar.io, where players control circular cells that move freely around the playing field with the mouse. The game dynamics include:

- **Eating Food Items:** Similar to Agar.io, players can grow their cell size by consuming food items scattered throughout the field.

- **Choice of Math Problem Difficulty:** Players can customize the difficulty level of the mathematical problems they encounter, including the number and type of mathematical operations and the magnitude of numbers involved.
  
- **Growing by Eating Correct Answers:** In Agar.IQ, players can increase the size of their cell by providing correct answers to mathematical problems. Each correct answer rewards the player by making their cell grow larger, enhancing their ability to consume other players or food pellets. The complexity of the mathematical problems can be customized by the player, allowing for a challenging and educational experience.
  
- **Shrinking by Mistakes:** In contrast, providing incorrect answers or failing to answer a mathematical problem results in a reduction in the size of the player's cell. This creates a dynamic challenge, encouraging players to balance their gameplay with mental arithmetic accuracy.
  
- **Interacting with Other Players:** Players can engage with each other by absorbing smaller cells or avoiding larger ones. The competitive aspect adds a strategic layer to the gameplay.
  
- **Multiplayer Functionality Over a Network:** Agar.IQ supports multiplayer gaming, enabling players to compete with others over the network.
  
- **AI-Controlled Bots:** The addition of AI-controlled bots adds to the challenge and variety of gameplay.
  
- **Account Creation:** Players have the option to create their gaming accounts.
  
- **Character Customization:** Players can choose the color of their characters, personalizing their experience.
  
- **Dynamic Gameplay:** The size of a player's cell impacts their speed and competitiveness, creating an engaging and strategic experience.
  
- **Sound Effects:** The game incorporates sound effects to enhance the overall gaming atmosphere.


### Screenshots and Demo Videos

<div style="text-align: center;">
    autorisation page
    <img src="readme/screenshot1.png" style="margin-bottom: 10px;" />
    main menu page
    <img src="readme/screenshot2.png" style="margin-bottom: 10px;" />
    game window page
    <img src="readme/screenshot3.png" style="margin-bottom: 10px;" />
    settings page
    <img src="readme/screenshot4.png" style="margin-bottom: 10px;" />
    demo video
</div>

[![InVasion - Gameplay Footage](https://img.youtube.com/vi/7Ccrl1oAriY/0.jpg)](https://youtu.be/7Ccrl1oAriY)

### Dependencies:

- Client: Qt version 5.12.0
- Server: QtNetwork
- JSON handling: nlohmann/json

### Installation:

To run the application, you'll need to follow these steps:

1. Download QtCreator.
2. In the terminal or GitBash, enter the command: `git clone git@github.com:Valerii3/Agar.IQ.git`
3. Open the 'Agario.pro' file in Qt.
