Written in: JavaScript   
   
Full Game with code, instructions, and images/json files creadted available here: https://github.com/jakenorris1124/COP3441-Project3-Group1
   
For my Processes of Object-Oriented Software class, my team was tasked with making a puzzle game. The game consisted of trying to get a ball on one side of the level to get to the goal on the other side using different parts to avoid obstacles for this I was put in charge of animation, both in terms of drawing/creating the assets and putting them into the game in which certain conditions would change the animation smoothly. The file above is one such instance of adding the sprite into the game and having it change depending on the conditions.  
   
The sprite of the ball is first created and scaled down to an appropriate size for the levels. From there it is given a body and has its physical properties set for the desired effects upon collision with obstacles. The ball also has 3 different states it can be in: regular, light, and heavy. The state of the ball is controlled inside the level script and is dependent on if it runs into a transformer with the same name respective to the state the ball would turn to.   
