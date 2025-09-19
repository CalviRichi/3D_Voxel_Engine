README for Calvin Richards 3D engine project

A project made with the intention of cloning the features of minecraft to create a basic voxel engine. 
So far there is a world block system that uses chunks to localize world update management. There is block placing and breaking, along with a highlight on the block in view.
There is optimization to not draw the hidden faces of blocks within chunks, * but I need to implement this to function across chunks * (ACCOMPLISHED)
I also need to * add an AABB collision system *

I hope to go so far as adding a *terrain generation system * but I believe that is something for the future and will be a more involved process. 
ALSO, * reformat chunks to contain their own vertex and index buffers that get renderered individually (ACCOMPLISHED)

Inside of the isAir function, there should be a checklist for if there if there is a block above grass, and if there is, then it should draw the sides with the dirt texture (basically did this)
