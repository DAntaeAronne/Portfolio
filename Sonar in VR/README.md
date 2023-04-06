Written in: C#   
   
As our Senior Design project, my team decided to work on ways of allowing those in the blind and visually imparied (BVI) community to have access/ease-of-use for virtual reality by using multi-modal orientation.    
    
To achieve this, we implemented 4 means of orientation: partial vision, haptic, audio, and sonar. What is in this folder is the sonar tool as this is what I was in charge of.       
    
How it works is that the player would hold the left trigger in which a 60-degree cone of a raycast would shoot out in whichever direction the player was looking in and detect the closest object. Once the closest object is determined a note will be selected for playback based on the height of the object. Afterwards, the note is repeatedly played for however long the player holds the trigger, with the rate at which the beeps play varying depending on the distance from the player to the closest object. Upon letting go of the trigger the sounds will stop and will no longer look for the closest object. The closest object and note selected will be constantly updated to fit what is in front of the player whenever the trigger is being held as well.
