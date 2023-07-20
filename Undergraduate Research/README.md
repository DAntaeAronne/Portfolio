During my undergraduate research, I was given the opportunity to create an AI that can solve a Raven Progressive Matrix (RPM).

An RPM is a 3x3 matrix in which the rows follow the same specific pattern though the shapes, size, and/or color of the shapes may vary accordingly. Only the first 2 rows are completely filled out with the 3rd row only having the first 2 columns filled. The last space is left empty as it is to be filled with one of the 8 answer choices, the correct answer choice would be the one that best fits the 3rd row following the pattern. The focus of RPM's being on pattern recognition, problem solving, and adaptability.

The original plan was to have the AI read images of RPM's that it would then solve. Though it worked with simple RPM's, it was not consistently accurate enough to rely upon for the AI to have a proper understanding of the problem. This was especially true for more complex matrices.

To work around this, instead of having the AI read in images it reads in json files containing descriptions of the RPM's.

In depth descriptions on how each code works will be explained in their respective folders
